# Forge, Hack Club's 3D Printer 


# WIP Disclaimers 
* This is an upcoming [Hack Club](https://) You Ship, We Ship (YSWS) program.
* Things are still under heavy development as the Forge Editor and the Forge Printer are WIP (Work in Progress) projects
* All information on this README is subject to change at any moment
* Submission requirements, images, and links are not final. Websites URLs are placeholders and may not be functional
* If you have any questions about the Development of Forge (and are a teen under 18), feel free to visit #forge-dev on the [Hack Club Slack!](https://hackclub.com/arcade/?param=slack)

**[Landing Page](https://forge.hackclub.com)** | **[Forge Editor](https://forge.hackclub.com/editor)** | **[Gallery](https://forge.hackclub.com/gallery)**

Find problems in the world and use programatic 3D modeling to solve them. Submit your solutions to the Gallery and get a Forge 3D Printer
Blot is a **You Ship, We Ship** project from [Hack Club](https://hackclub.com). 

> Forge is **fully open source**. The online editor can be freely used (or modified) by anyone, and we encourage all to submit to the gallery through PRs! However, you must be a teenager or younger to receive a free printer.

![image](https://github.com/Patcybermind/Project-Moonlight/assets/97562509/da8007c4-607d-4d73-ad87-6afdcd655585)

Building a model in the Forge editor is easy to learn, hard to master. You need abosultely no CAD or programming knowledge to get started. Since the Forge Editor is based on the [OpenSCAD](https://openscad.org) modeling language, it presents a challenge to even the most experienced parametric modelers. 

# What is Forge exactly?

The Forge YSWS Project can be considered 2 major sections
* Forge Printer - the fixed-bed cantilevered 3D Printer 
* Forge Editor - the OpenSCAD-based modeling software and slicer designed for the Forge Printer

# About the Forge Printer

The printer is built after the [Flatpack](https://github.com/eponra/flatpack). However, this printer is signififcantly different internally than the Flatpack. Featuring a STM32 H5 series MCU and TMC2209 drivers on a custom Hack Club-engineered control board. The printer is designed to fit inside a filament box when folded, only taking up 220x210x75mm (not including the power supply or spool holder). Forge is the ultimate tool for competitions, club fairs, or hackathons.

## Specifications:

### These are Forge's specification as of July 16, 2024. As we begin test manufacturing Forge, these specifications may change.

 * STM32H563VITX 32-Bit MCU
 * TMC2209 Stepper Drivers
     - Reduced Motor Noise
     - Sensorless XY homing  
 * TZ-E3 Hotend
     - 300°C Maximum Temperature
     - Designed around the Bambu Lab X1C hotend
     - 33mm³/s max flow rate
 * 120x114x110 Heated Build Plate
       - Up to 100°C Print Temperature
       - Magnetic Textured PEI Plate
       - A small portion of print area is occupied by the Klicky Probe system
 * MGN9C Linear Rails on all axes
 * OLED and Rotary Encoder interface
 * Sherpa Mini Direct Drive Extruder (DDE)
 * Klicky Probe
      - Fully Automatic Bed Leveling (ABL)
      - Z offset is currently manual
 * PLA, PETG, TPU capable
      - ABS and ASA are posssible but require ventilation and an enclosure
 * Aluminum and 3D Printed ASA frame
 * 250mm/s Tested Print Velocity
      - This number is an estimate, real print speeds will be updated as Forge's development continues

## Firmware

 Forge runs on custom, open source Forge Firmware
    - Forge is **not supported by Klipper** Using forge with Klipper will require **replacing the mainboard** with an BigTreeTech SKR Pico and a Raspberry Pi Zero 2W alongside a seperately printed bracket (files will be provided at launch). Additionaly, no Klipper configuration files will be provided at this point. The bracket will still allow the printer to fold properly. 

## Additional notes

* The BOM will be on the repo (coming soon!) for a more detailed view. 
* The estimated value of the Forge Printer is $190-$250.
   - However, Forge will be **100% Free for teens 18 and under after designing 5 models and submiting a PR**
*   Currently, Forge's parts and components are planned to be manufactured by [Siboor](https://www.siboor.com)
*   Forge is designed in [OnShape](https://cad.onshape.com/documents/490fa34c5c188f9b01dad5d1/w/4ce61de39bd6c276033d903d/e/7a262062418efbefd9181a13?renderMode=0&uiState=6696ce6038c5ba5455f5be75)

# How do I get a Forge Printer

### 1) Find a problem

Look around! Find problems that 3D printing can fix 

### 2) Submit that art to the public gallery.

The [submission rules to get a Blot for a PR are here](https://github.com/hackclub/blot/blob/main/docs/GET_A_BLOT.md). **You must be a teenager (or younger to qualify for a free machine)** but anyone can make art and make a PR.

[<img width="500" src="https://github.com/hackclub/blot/assets/27078897/5666011a-089e-44d9-8956-5c283f00ff14"/>](https://blot.hackclub.com/gallery)

### 3) Receive the parts to build your own Blot that can draw that art in real life.

The [bill of materials can be found here](/docs/BOM.toml) and an [assembly guide here](/docs/ASSEMBLY.md).

<img width="500" alt="all-parts" src="https://github.com/hackclub/blot/assets/27078897/8559466d-fd36-4126-bf9c-45cb913be6da" />

<img width="500" alt="drawing" src="https://github.com/hackclub/blot/assets/27078897/87f7e5b3-1aee-4082-8ae5-36006ec1ab0d" />

<!-- <img height="341" alt="drawing" src="https://github.com/hackclub/blot/assets/27078897/2f7c9c17-3b67-4674-b2ca-2a4dbd26a3d1" /> -->

# Blot is...

**A custom CNC drawing machine** designed from scratch to introduce you to digital fabrication. 
It's made of 6 unique 3D printed parts with a custom control board and easy to understand firmware, which can be interfaced with through JavaScript in the browser.
Blot is designed in OnShape. Check out the [3D model of the project](https://cad.onshape.com/documents/0bcd2f50d2614ea26189f43b/w/23913e7defc94fc29f7833e6/e/72ea852bfc1822955e506e37?renderMode=0&uiState=6538235d42737a70b1996741).
You can find the [bill of materials here](https://blot.hackclub.com/assembly) and, if you'd like to 3D print your Blot, [the parts are here](https://github.com/hackclub/blot/tree/main/hardware/mechanical/drawing-thing-v4). The entire build costs about $150 dollars. If you're a teen we will give you all the parts for a PR with your an art piece you coded yourself.

**A web based editor for programmatic art** that we designed specifically for pen plotting with a [custom geometry library](/docs/DOCUMENTATION.md).
We practiced programmatic pen plotting with [some](https://github.com/LingDong-) [amazing artists](https://static1.squarespace.com/static/63fbc39db5b01b5fa30423db/t/649b424d33b2ce3e0d5b63a5/1687896656015/June+Cohort+Zine.pdf). 
There is so much to learn and explore on the programming and the aesthetic side. 

<img width="300" alt="parts" src="https://github.com/hackclub/blot/assets/27078897/04ab7345-03fa-4b60-9870-64a99327e8cd">

<img width="300" src="https://github.com/hackclub/blot/assets/27078897/0ffc0ca8-516b-4f9a-b34e-4f09218e41cd"/>




