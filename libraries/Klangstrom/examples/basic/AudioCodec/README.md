# AudioCodec

This example demonstrates how to use the audiocodec.

the simplest way to initialize the audiocodec is to use `system_init_audiocodec()`, this creates and starts
the default audio codec with the default settings. alternatively, the audio codec can be intialized manually
by intializing it with an `AudioInfo` struct. e.g:

```cpp
AudioDevice* audiodevice;
AudioInfo audioinfo;
audioinfo.sample_rate     = 48000;
audioinfo.output_channels = 2;
audioinfo.input_channels  = 2;
audioinfo.block_size      = 128;
audioinfo.bit_depth       = 16;
audiodevice               = audiodevice_init_audiocodec(&audioinfo);
audiodevice_resume(audiodevice);
```

once the audiocodec is started ( with `audiodevice_resume(audiodevice);` ) a callback method is called whenever 
a new block of sample data is required for output and available from the input. by default the callback is set
to `void audioblock(AudioBlock* audio_block) {}`. the audio block struct contains the input and output buffers
as well as further information about the audio device and the sample data. this can be useful when for example 
multiple audio devices are used in a single system.

note, for KLST_PANDA and KLST_CATERPILLAR sample rate and bit depth are currenty fixed to 48000 Hz and 16 bit respectively.

## NERD SECTION

the audio codec is configured to run in direct-memory-access (DMA) mode and acquires and supplies audio data in blocks.
by default the block size is set to 128 samples. the buffer size can be changed by setting the macro 
`KLST_AUDIO_CODEC_AUDIO_BUFFER_SIZE` to the desired size. the block size is a trade-off between latency and overhead. at
a buffer size of 128 and a sampling rate of 48 kHz the latency is 2667 µsec i.e a block is requested every 2667 µsec.
