---
layout: page
title: Working with Nodes in Klang
permalink: /working-with-nodes-in-klang/
index: 30
---

*Klang* uses processing units, with *inputs* and *ouputs*, called *nodes* connected through *connections* to form signal networks called *patches*.

## The Patch-Node-Connection Model

the following *node* features just a single input `IN00` and a single output `OUT00`:

```
      [ NODE             ]       
      +------------------+       
      |                  |       
IN00--| SIGNAL    SIGNAL |--OUT00
      |                  |       
      +------------------+       
```

*nodes* are connect from *output* to *input* ( `OUT00` to `IN00` ):

```
      [ NODE_A           ]                     [ NODE_B           ]       
      +------------------+                     +------------------+       
      |                  |                     |                  |       
IN00--| SIGNAL    SIGNAL |--OUT00 -----> IN00--| SIGNAL    SIGNAL |--OUT00
      |                  |                     |                  |       
      +------------------+                     +------------------+       
```

most *nodes* receive an input signal through their *input*, do some calculations and then pass on the signal through the *output* to a connected *node*. however, some *nodes* can be thought of as *generators* in so far as they only generate a signal internally without the need for an input:

```
      [ NODE_GENERATOR   ]       
      +------------------+       
      |                  |       
      |           SIGNAL |--OUT00
      |                  |       
      +------------------+       
```

a common type of generator node is the oscillator (VCO)[^1] which generates a periodic signal at a specific frequency and amplitude.

another special type of node is the output node that has just one or more inputs and no outputs:

```
      [ NODE_OUTPUT      ]       
      +------------------+       
      |                  |       
IN00--| SIGNAL           |
      |                  |       
      +------------------+       
```

in *Klang* the only node with this feature is the *Digital Analog Converter* (DAC) node[^2] which collects and prepares an audio signal for the audio hardware output.

a minimal *patch* that would be capable of emitting a sound would be a VCO connected to a DAC:

```
      [ NODE_VCO         ]                     [ NODE_DAC         ]       
      +------------------+                     +------------------+       
      |                  |                     |                  |       
      | SIGNAL    SIGNAL |--OUT00 -----> IN00--| SIGNAL    SIGNAL |
      |                  |                     |                  |       
      +------------------+                     +------------------+       
```

although *Klang* is programmed in a *text-based* fashion, it is absolutely helpful to think of *patches* as visual models. the above patch would look, translated into text-based code, something like this:

```c
NodeVCOWavetable mVCO;
NodeDAC mDAC;

Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, Node::CH_IN_SIGNAL);
```

but more on this matter later.

speaking of visual model, it is also helpful to think of the signals flowing through the patch from left to right. the VCO on the left produces a signal which then flows *downstream* to the right into the DAC node[^3].

## Connecting and Disconnecting Nodes

the most common way to connect nodes is through the function `Klang::connect()`. it allows to connect a single output of a node with a single input of another node:

```c
CONNECTION_ID connect(Node out, CHANNEL_ID out_ch, Node in, CHANNEL_ID in_ch)
CONNECTION_ID connect(Node out, Node in)
```

both functions create a connection object and return its ID. similarly `Klang::disconnect()` disconnects two nodes:

```c
bool disconnect(Node out, CHANNEL_ID out_ch, Node in, CHANNEL_ID in_ch)
bool disconnect(CONNECTION_ID id)
```

both functions attempt to disconnect two nodes and return if it was successful.

### Multiple connections

note that the input channel of one node can never be connected to multiple output channels.

likewise the output channel of one node should not be connected to multiple input channels. however, if it required to split the signal from an output to connect to multiple inputs `NodeBuffer` can be used to store the signal of all upstream nodes and thereby facilitating the connection of multiple inputs.

## Anatomy of a Node

nodes have a series of common functions as well as properties and follow certain conventions. let us take a look at [`NodeVCOWavetable`]({{ site.baseurl }}{% link _klang/NodeVCOWavetable.md %}):

```
      [ NODE_VCO_WAVETABLE      ]       
      +-------------------------+       
      |                         |       
IN00--| FREQ             SIGNAL |--OUT00
IN01--| AMP                     |       
      |                         |       
      +-------------------------+       
```

the output channel of the VCO emits a periodic signal. although the VCO node is a *generator* it has two input channels. the input channels allow the two properties frequency and amplitude to be set by another node[^4]. 

### Input and Output Channels

input and output channel names follow a naming convention. `NodeVCOWavetable` defines the following channel names:

```c
CH_IN_FREQ
CH_IN_AMP
CH_OUT_SIGNAL
```

### Getter and Setter Functions

furthermore most nodes have properties that can be changed and queried via `set_` and `get_` functions. the VCO has the following functions:

```c
void        set_frequency(SIGNAL_TYPE pFrequency)
SIGNAL_TYPE get_frequency()
void        set_amplitude(SIGNAL_TYPE pAmplitude)
SIGNAL_TYPE get_amplitude()
void        set_offset(SIGNAL_TYPE pOffset)
SIGNAL_TYPE get_offset()
void        set_waveform(WAVEFORM pWaveform)
```

note, that the VCO node features functions to set frequency and amplitude despite the fact that these properties can also be controlled by input signals. this way of setting the properties becomes relevant when frequency or amplitude input channels are not connected to other nodes.

consult the documentation or the source code of the nodes for detailed information. some nodes have additional functions to configure their behavior.

### Commands

lastly, most nodes implement a low-level way of setting properties via a *command* mechanism:

```c
void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad);
```

this mechanism is used for configuring nodes through low-level interfaces.

## An Example Patch

below is a simple patch that connects the output of three wavetable oscillators to the inputs of a low-pass filter which is connected to an ADSR envelope that is connected to a DAC output:

```
          [ NODE_WAVETABLE ]                                                                                                        
          +----------------+                                                                                                        
          |                |                                                                                                        
    IN00--| FREQ    SIGNAL |--OUT00 >-+                                                                                             
    IN01--| AMP            |          |                                                                                             
          |                |          |                                                                                             
          +----------------+          |                                                                                             
                                      |         [ NODE_FILTER_MOOG ]                  [ NODE_ADSR       ]                  [ NODE_DAC        ]
          [ NODE_WAVETABLE ]          |         +------------------+                  +-----------------+                  +-----------------+ 
          +----------------+          |         |                  |                  |                 |                  |                 |
          |                |          +-> IN00--| SIGNAL    SIGNAL |--OUT00 >-> IN00--| SIGNAL   SIGNAL |--OUT00 >-> IN00--| SIGNAL(_LEFT)   |
    IN00--| FREQ    SIGNAL |--OUT00 >---> IN01--| CUTOFF           |                  |                 |            IN01--| SIGNAL_RIGHT    |
    IN01--| AMP            |          +-> IN02--| RESONANCE        |                  +-----------------+                  |                 |
          |                |          |         |                  |                                                       +-----------------+
          +----------------+          |         +------------------+                                                                
                                      |                                                                                             
          [ NODE_WAVETABLE ]          |                                                                                             
          +----------------+          |                                                                                             
          |                |          |                                                                                             
    IN00--| FREQ    SIGNAL |--OUT00 >-+                                                                                             
    IN01--| AMP            |                                                                                                        
          |                |                                                                                                        
          +----------------+                                                                                                        
```

below is a translation of the above schematic into c++ source code:

```c
#include "Nodes.hpp"

using namespace klang;

NodeVCFMoogLP mFilter00;
NodeVCOWavetable mOsc01;
NodeVCOWavetable mOsc02;
NodeVCOWavetable mOsc03;
NodeADSR mADSR04;
NodeDAC mDAC05;

void setup() {
    Klang::connect(mOsc01,    0, mFilter00, 0);
    Klang::connect(mOsc02,    0, mFilter00, 1);
    Klang::connect(mOsc03,    0, mFilter00, 2);
    Klang::connect(mFilter00, 0, mADSR04,   0);
    Klang::connect(mADSR04,   0, mDAC05,    0);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, 
                SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft, 
                SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
```

note, that the oscillators and the filter are not configured in a meaningful way e.g frequency, amplitude, cutoff frequency would need some tweaking. see the examples or the node documentations for details. also note, that the channels are identified by literal numbers instead of the more commonly used constants ( e.g `Node::CH_OUT_SIGNAL` ).

## Writing Custom Nodes

the easiest way to create custom nodes is by extending the class [`NodeKernel`]({{ site.baseurl }}{% link _klang/NodeKernel.md %}).

### Extending `NodeKernel`

`NodeKernel` has an output channel, an input channel and the function `SIGNAL_TYPE kernel(SIGNAL_TYPE s)`. this function needs to be implemented to perform computations on the incoming signals. the following example shows how to implement a node which amplifies and clamps an incoming signal and send it to output:

```c
class MNodeKernel : public NodeKernel {
public:
    float amplitude = 3.0;
    const float clamp = 0.4;
    
    SIGNAL_TYPE kernel(SIGNAL_TYPE s) {
        SIGNAL_TYPE t = s * amplitude;
        return t > clamp ? clamp : (t < -clamp ? -clamp : t);
    }
};
```

### Writing Nodes from Scratch

it is also possible to write nodes from scratch. each node must be derived from the base class `Node`:

```c
class NodeKernel : public Node {
};
```

then the following functions need to be implemented:

```c
void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock)
bool connect(Connection* pConnection, CHANNEL_ID pInChannel)
bool disconnect(CHANNEL_ID pInChannel)
void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad)
```

@todo(this segment needs some extra work. explain functions in detail)

#### `update`

- check if updated. if not, …
    - allocate audio blocks
    - process upstream nodes connected to input channels
    - release audio blocks
    - flag updated
- fill to requested audio block with output channel buffer
- in cases where the result of the frame is not stored the request audio buffer is filled inline

#### `connect` + `disconnect`

#### `set_command`

#### `set_` + `get_`

--------------------------------------------------------------------------------

[^1]: the abbreviation *VCO* refers to a *Voltage Controlled Oscillator*. this is a reference to the world of analog synthesizers where oscillators were controlled by voltages and would emit voltages. so in this sense it more an analogy than a technical term. however, these terms are quite common in the audio signal processing world. you will find these analogies in many places ( e.g *Voltage Controlled Amplifier* (VCA) or *Voltage Controlled Filter* (VCF) ). as a matter of even the term *patch* hints towards cable-monsters known from analog synthesizer setups. note that the actual VCO node is more complex and even exists in different variants.
[^2]: note, that the term DAC in this context is used rather *metaphorically*. it eludes to the function of the node rather than the actual connection to audio hardware. this happens on a *lower level*.
[^3]: although the *downstream* model is the most intuitive way to visualize the inner workings of a patch, programmatically speaking the audio signals are processed the other way around: from right to left or upstream i.e the DAC request new signal data from the VCO.
[^4]: a quite common setup is to connect the output of one VCO ( at very low frequency ) to the frequency or amplitude input of another VCO thus creating a vibrato or tremolo effect.