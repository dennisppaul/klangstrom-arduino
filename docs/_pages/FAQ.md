---
layout: page
title: FAQ
permalink: /faq/
---

## What if the board does not enter DFU mode?

occasionally the board cannot enter programming or DFU mode. this is usually due to the fact that the board has crashed. 

the easiest option is to make the board enter DFU mode *manually*. this can be achieve by pressing + holding down the `BOOT` button, then pressing + releasing the `RESET` button and then releasing the `BOOT` button again ( it takes a bit of practice because the buttons are so small). if the procedure was executed correctly the board is now in DFU mode.

## Why does the USB serial port disappear during upload?

when a program is uploaded to the board the virtual USB serial port is temporarily deactivated. unfortunately, the Ardunio IDE emits an error messages everytime this happens. 

a workaround is to close the *Serial Monitor* window right before uploading a new programming. 

## Why is my headset ( headphone + microphone ) not working properly?

there are two things to check:

first, make sure that the correct input line is selected. with `option(KLST_OPTION_AUDIO_INPUT, KLST_MIC)` called in `setup()` the microphone is selected as input ( the other option is the line input `option(KLST_OPTION_AUDIO_INPUT, KLST_LINE_IN)` ).

secondly, confirm that the headset confirms to the OMTP standard ( TIP: left, RING#1: right, RING#2: microphone, RING#3: ground ). if a CTIA/AHJ headset is used ( e.g Apple headsets ) an adapter is required ( to flip RING#2 and RING#3 ).
