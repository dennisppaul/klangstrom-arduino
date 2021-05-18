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
