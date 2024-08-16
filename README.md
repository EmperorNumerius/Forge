# Forge, Hack Club's 3D Printer

## WIP Disclaimers

* This is an potential upcoming [Hack Club](https://hackclub.com) You Ship, We Ship (YSWS) program.
      - This project is not currently endorsed by Hack Club. **We're proud to announce that we're in talks with Hack Club to get this project off the ground**
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

## What is Forge exactly?

The Forge YSWS Project can be considered 2 major sections:

* Forge Printer - the fixed-bed cantilevered 3D Printer
* Forge Editor - the OpenSCAD-based online modeling software and slicer designed for the Forge Printer
  * Note that exporting to STLs is an option if you want to test your model on another 3D printer before you submit an PR.  

## About the Forge Printer

The printer is built after the [Flatpack](https://github.com/eponra/flatpack). However, this printer is signififcantly different internally than the Flatpack. Featuring a STM32 F405 series MCU and TMC2209 drivers on a custom Hack Club-engineered control board. The printer is designed to fit inside a filament box when folded, only taking up 220x210x75mm (not including the power supply or spool holder). Forge is the ultimate tool for competitions, club fairs, and hackathons.

## Specifications

**These are Forge's specification as of August 10, 2024. As we begin test manufacturing Forge, these specifications may change.**

* STM32F405RGT6 32-Bit MCU
* TMC2209 Stepper Drivers
  * Reduced Motor Noise
  * Sensorless XY homing  
* TZ-E3 Hotend
  * 300°C Maximum Temperature
  * Designed around the Bambu Lab X1C hotend
  * 30mm³/s max flow rate
* 100x110x110 Heated Build Plate
  * Up to 100°C Print Temperature
  * Magnetic Textured PEI Plate
  * A small portion of print area is occupied by the Klicky Probe system
* MGN9C Linear Rails on all axes
* OLED and Rotary Encoder interface
* Sherpa Mini Direct Drive Extruder (DDE)
* Klicky Probe
  * Fully Automatic Bed Leveling (ABL)
  * Z offset is currently manual
* PLA, PETG, TPU capable
  * ABS and ASA are posssible but require an enclosure
* Aluminum and 3D Printed ASA frame
* 300mm/s  Print Velocity*
  * *This number is an estimate, real print speeds will be updated as Forge's development continues

## Firmware

Forge will run on custom, open source Forge Firmware

* Forge 2 layer PCB is **supported by Klipper**
  * Using forge with Klipper will be possible! The latest 2 layer PCB version of our motherboard runs a STM32F405 which is fully compatible with klipper! We've also made the [configuration](https://github.com/blazecoding2009/Forge/blob/main/Firmware/klipper) for you! (No macros are there right now as we are in early stages of development) The bracket will still allow the printer to fold properly.

## Additional notes

* The BOM is at the bottom of this doc, and available as a CSV in `Electronics/Motherboard/billofmaterials.csv` for a more detailed view.
* The estimated value of the Forge Printer is $200-$250, but this estimate will change over time.
  * However, Forge will be **100% Free for teens 18 and under after designing 5 models and submiting a PR**
* Currently, Forge's parts and components are planned to be manufactured by [Siboor](https://www.siboor.com), a well established supplier of 3D Printing components.
* Forge is designed in Onshape. [Check out the 3D model!](https://cad.onshape.com/documents/490fa34c5c188f9b01dad5d1/w/4ce61de39bd6c276033d903d/e/7a262062418efbefd9181a13?renderMode=0&uiState=6696ce6038c5ba5455f5be75)

## How do I get a Forge Printer?

### 1) Find a problem

Look around! Find problems that 3D printing can fix. From a carabiner to a phone case!  

### 2) Design a solution

Using the Forge Editor, follow the turorials (link coming soon!) and program your solutions in OpenSCAD.  

### 3) Submit your design

The submission rules to get a Forge Printer for a PR are (not available, the Forge design guidelines will be accessable at launch). **You must be a teenager (or younger to qualify for a free machine)** but anyone can make a model and make a PR. Currently, we are requiring 5 Blot-level (of quality) models. This may change as the project progresses. While the full guidelines are not accessable, here's the basics of what you should(n't) do with your Forge project:

#### Dos and Don'ts

* A PR should have a .scad and .stl file
* Don't copy OpenSCAD code from other websites/generate code with AI (ChatGPT)
  * Doing so is considered plagirism. Anyone submitting fradulent code will be ineligible for a Forge Printer and may result in a violation of the Hack Club Code of Conduct
* Don't build your model in Parametric CAD software (Fusion, OnShape, Shapr3D, ect)
* Your model should be both functional and have some asthetic quality.
  * TL:DR, make it look nice
* Your model should have effort (roughly 8 hours per model)

### 4) Receive the parts to build your own Forge Printer

The bill of materials can be found here and is available as a CSV in `Electronics/Motherboard/billofmaterials.csv` and an assembly guide here (coming soon!).
Also note that, currently, the BOM is not polished as we communicate with Siboor.
