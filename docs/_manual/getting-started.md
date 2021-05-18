---
layout: page
title: Getting Started
permalink: /getting-started/
index: 10
---

this guide assumes that you want to run an application on a `KLST_TINY` board. see [How to Upload or Run Applications]({{ site.baseurl }}{% link _manual/how-to-upload-or-run-applications.md %}) for other platforms.

- connect audio connector `HEADPHONES+MIC` to headphones or speakers
- connect USB port `DEVICE+PWR` to computer

a white LED located next to the USB port lights up.

- install the *Klangstrom* hardware library, see [Installation]({{ site.baseurl }}{% link _pages/installation.md %}) for detailed instructions. 
- (re)start the Arduino IDE
- select platform: `Tools > Board > Klangstrom (MCU) > Klangstrom (STM32)`
- select board: `Tools > Board part number > KLST_TINY(STM32F446RE)`
- select upload method: `Tools > Upload Method > STM32CubeProgrammer (DFU)`. 

after selecting a platform all examples are available under `File > Examples > Klangstrom > …`.

- open example: `File > Examples > Klangstrom > klangstrom > KLST--Blink`
- compile + upload sketch: `Sketch > Upload`

once the example is uploaded `LED_00` blinks and a single 110Hz note plays. now try running the same example in the simulator:

- select platform: `Tools > Board > Klangstrom (DESKTOP) > Klangstrom (SDL)`
- compile + upload sketch: `Sketch > Upload`

once the example runs a window appears with also `LED_00` blinking and a single 110Hz note playing.

## Where to go from here?

the project is structured into different sections:

- Klangstrom ( Application )
- Klang ( Sound + Digital Signal Processing )
- Strom ( Composition + Methods + Structure ) @todo(currently not distributed)

browsing, running and modifying examples is most playful way to learn about *Klangstrom*. a more structured approach is to browse through the *Library Documentation* and the *Operation Manual*.
