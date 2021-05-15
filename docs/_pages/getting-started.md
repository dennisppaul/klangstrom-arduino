---
layout: page
title: Getting Started
permalink: /getting-started/
---

first, install the *Klangstrom* hardware library, see [Installation]({% link _pages/installation.md %}) for detailed instructions. 

once the hardware library is installed and the Arduino IDE is (re)started a *Klangstrom* platform can be selected under `Tools > Board > Klangstrom (…)`. make sure to select the correct board options e.g the board under `Tools > Board part number` and the desired upload method `Tools > Upload Method`. 

after selecting a platform all examples are available under `File > Examples > Klangstrom > …`. next run one of the examples. a good first example to choose is the `KLST--Blink` example which makes the LEDs blink and plays a single note.

optionally, try running the example on different *Klangstrom* platforms ( e.g `MCU` + `DESKTOP` ).

although browsing, running and modifying examples is by far the best way to learn about *Klangstrom* a more structured approach can be found under *Library Documentation*. note, that the project is structured into different sections:

- Klangstrom ( Application )
- Klang ( Sound + Digital Signal Processing )
- Strom ( Composition + Methods + Structure ) @todo(currently not distributed)
