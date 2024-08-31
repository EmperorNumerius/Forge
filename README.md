# Forge, Hack Club's 3D Printer

## WIP Disclaimers

* This is an potential upcoming [Hack Club](https://hackclub.com) You Ship, We Ship (YSWS) program.

> **This project is not currently endorsed by Hack Club and isn't an offical program.**

* Things are still under heavy development as the Forge Editor and the Forge Printer are WIP (Work in Progress) projects
* All information on this README is subject to change at any moment
* Submission requirements, images, and links are not final. Websites URLs are placeholders and may not be functional
* If you have any questions about the Development of Forge (and are a teen under 18), feel free to visit [#forge-dev](https://hackclub.slack.com/archives/C078GBDKC03) on the [Hack Club Slack!](https://hackclub.com/arcade/?param=slack)

**[Landing Page](https://forge.hackclub.com)** | **[Forge Editor](https://forge.hackclub.com/editor)** | **[Gallery](https://forge.hackclub.com/gallery)**

Find problems in the world and use programatic 3D modeling to solve them. Submit your solutions to the Gallery and get a Forge 3D Printer
Forge is planned to be a **You Ship, We Ship** project from [Hack Club](https://hackclub.com).

Forge is **fully open source**. The online editor can be freely used (or modified) by anyone, and we encourage all to submit to the gallery through PRs! However, you must be a teenager or younger to receive a free printer.

Building a model in the Forge editor is easy to learn, hard to master. You need abosultely no CAD or programming knowledge to get started. Since the Forge Editor is based on the [OpenSCAD](https://openscad.org) modeling language, it presents a challenge to even the most experienced parametric modelers.

## What is Forge exactly?

The Forge YSWS Project can be considered 4 major sections:

* Forge Printer - the fixed-bed cantilevered 3D Printer
* Forge Editor - the OpenSCAD-based online modeling software and slicer designed for the Forge Printer
* Forge Mainboard - the custom-enginnered control board powered by a STM32F405 and TMC2209s. Also includes the interface board
* Forge Firmware - The custom-engineered firmware that powers the Forge Mainboard

## About the Forge Printer

The printer is built around the [Flatpack](https://github.com/eponra/flatpack). However, this printer is signififcantly different internally than the Flatpack. Featuring a STM32 F405 series MCU and TMC2209 drivers on a custom Hack Club-engineered control board. The printer is designed to fit inside a filament box when folded, only taking up 220x210x75mm (not including the power supply or spool holder). Forge is the ultimate tool for competitions, club fairs, and hackathons.

## Specifications

**These are Forge's specification as of August 24, 2024. As we begin test manufacturing Forge, these specifications may change.**

* STM32F405RGT6 32-Bit MCU
* TMC2209 Stepper Drivers
  * Reduced Motor Noise
  * Sensorless XY homing  
* TZ-E3 Hotend
  * 300°C Maximum Temperature
  * Designed around the Bambu Lab X1C hotend
  * 30mm³/s max flow rate
* 120x110x110mm Build Volume
  * Up to 100°C heated bed temperature
  * Magnetic Textured PEI Plate
  * A small portion of print area is occupied by the Klicky Probe system.
* 220x210x75mm Folded Volume
  * Small enough to fit in a filament box! 
* MGN9C Linear Rails on all axes
* OLED and Rotary Encoder interface
* Sherpa Mini Direct Drive Extruder (DDE)
* Klicky Probe
  * Fully Automatic Bed Leveling (ABL)
  * Z offset is currently manual
* PLA, PETG, TPU capable
  * ABS and ASA are posssible but require an enclosure and active ventilation
* Aluminum and 3D Printed ASA frame
* 3010 Axial Fan for Heatbreak cooling and 4010 "Blower-style" fan for part cooling
* Up to 300mm/s Print Velocity*
   *This number is an estimate, real print speeds will be updated as Forge's development continues
* SD1306 1.92" OLED and Rotary Encoder for the interface

## Firmware

* The Forge Mainbaord will run on custom, open source Forge Firmware (more information coming out soon!)

* **The Forge Mainboard (2 layer PCB) is supported by Klipper**
  * Using the Forge Printer with Klipper will be possible! The latest revision of the Forge Mainboard (2 layer PCB version) runs a STM32F405 which is fully compatible with Klipper! Additionally, we'll be providing a Klipper [WiP configuration file](https://github.com/blazecoding2009/Forge/blob/main/Firmware/klipper) and OrcaSlicer profiles (Coming Soon) for you!
  * SBC Info:
    * **A Raspberry Pi Zero 2W will be required for *Klipper*. You can also use another SBC (that can run MainSailOS or Fluidd) with the same form factor. However, the Zero W (1st Gen) is highly not recommended due to performance limitations. Larger SBCs like the Raspberry Pi 1/2/3/4/5 boards are not compatible due to size limatations. We'll be providing a Raspberry Pi Zero Bracket available to print that still allows the printer to fold.** The Forge Mainboard connects to the SBC via GPIO to GPIO, thereforem, SPI touchscreens are not recommended.
    
## Additional notes

* The full Forge Printer and Mainboard BOM will (soon be) at the bottom of this doc. The Forge Printer BOM is not currently available until we begin prototyping. However, the BOM for the Forge Mainbaord is currently available as a CSV in `Electronics/Motherboard/billofmaterials.csv`.
* The estimated value of the Forge Printer is $200-$250, but this estimate will change over time.
  * However, Forge will be **100% Free for teens 18 and under after designing 5 models and submiting a PR**
* Currently, Forge's parts and components are planned to be manufactured by [Siboor](https://www.siboor.com), a well established supplier of 3D Printing components.
* Forge is designed in Onshape. [Check out the 3D model!](https://cad.onshape.com/documents/490fa34c5c188f9b01dad5d1/w/4ce61de39bd6c276033d903d/e/7a262062418efbefd9181a13?renderMode=0&uiState=6696ce6038c5ba5455f5be75)

## Licensing
* **Since Flatpack is GPLV3 (General Public License), the Forge Project is also GPLV3**
* The design uses a Annex Engineering Sperpa Mini Extruder. In compliance to their guidelines, the CAD models of the Forge Printer will use a placeholder provided by Annex Engineering.
   - [Sherpa Mini GitHub](https://github.com/Annex-Engineering/Sherpa_Mini-Extruder/tree/master)
* Anybody is allowed to freely use, advertise, modify, copy, or distribute any Forge designs or software as needed with the condition that any public distribution or use of Forge software/design must also be open source and use GPL.
* For more information about this license, check out this [link](https://www.gnu.org/licenses/gpl-3.0.en.html)
  
## Support the Project!

* Forge's banking is fully transparent: [Check it out now!]()
* [Donate to the Forge Project](https://hcb.hackclub.com/donations/start/forge), any dollar counts!
* **Forge is now offically [fiscally sponsored](https://en.wikipedia.org/wiki/Fiscal_sponsorship#:~:text=Fiscal%20sponsorship%20refers%20to%20the,and%20an%20established%20non-profit.) by Hack Club (a 501(c)(3) NPO with EIN 81-2908499)!  ** 


## How do I get a Forge Printer?

### 1) Find a problem

Look around! Find problems that 3D printing can fix. From a carabiner to a phone case!  

### 2) Design a solution

Using the Forge Editor, follow the turorials (link coming soon!) and program your solutions in OpenSCAD.  

### 3) Submit your design

The submission rules to get a Forge Printer for a PR are (not available, the Forge design guidelines will be accessable at launch). **You must be a teenager (or younger to qualify for a free machine)** but anyone can make a model and make a PR. Currently, we are requiring 5 Blot-level (of quality) models. This may change as the project progresses. While the full guidelines are not accessable, here's the basics of what you should(n't) do with your Forge project:
* Note that exporting your design to an STL is an option if you want to test your model on another 3D printer before you submit an PR.

#### Dos and Don'ts

* A PR should have a .scad and .stl file
* Don't copy OpenSCAD code from other websites/generate code with AI (ChatGPT)
  * Doing so is considered plagirism. Anyone submitting fradulent code will be ineligible for a Forge Printer and may result in a violation of the Hack Club Code of Conduct
* Don't build your model in Parametric CAD software (Fusion, OnShape, Shapr3D, ect)
* Your model should be both functional and have some asthetic quality.
  * TL:DR, make it look nice
* Your model should have effort (roughly 8 hours per model)

### 4) Receive the parts to build your own Forge Printer

The bill of materials for the Forge Printer will be coming soon here as we start finalizing the printer. An assembly guide will also be available here (coming soon!).
Also note that, currently, the BOMs are not polished as we communicate with Siboor and test manufacture.


## Forge Printer BOM
* Coming soon! We have the finished BOM for the MK1 Prototype from Siboor which we just need to clean up before posting here! 


## Forge Project Credits:

### Forge Team:
* Krishna Meda (@EmperorNumerius) - Project Lead, Head of Communications, Head of Forge Editor, Promotional Materials
* Arnnav Kudale (@blazecoding2009) - Project Lead, Head of Sourcing, Head of Electrical Engineering, Forge Firmware/Klipper Config Developer
* Michael Panait (@mikeymascatu) - Head of Design Engineering, Promotional Materials
* Brendan Conover (@AGB556) - Design Engineer
* Malav Patel (@astro-develops) - Forge Editor/Landing Page Frontend Developer, Promotional Materials
* Arthur Beck (@AverseABFun) - Head of Firmware

### Special Thanks:
* Aaron Wong (@aaronw-dev) - Former Project Lead, Firmware Dev, Artist, Electrical Engineer
* Engineering and Sales teams at Siboor - This project wouldn't be possible without them <3
* Patricio (@Patcybermind) - Former Forge Editor Developer
* Max Wofford (@maxwofford) - Hack Club HQ Representative, Forge Donator
* Alex Ren (@qcoral) - Thanks for the help with licensing!
* Annex Engineering - Thanks for helping with Sherpa Mini copyright! 
* Jonathan Dong (@Dongathan-Jong) - Forge Art, Promotional Materials
* Beenana (@Beenana02) - Promotional Materials

### CAD Models and reference designs:
* [Flatpack](https://github.com/eponra/flatpack)
* [Sherpa Mini by Annex Engineering](https://github.com/Annex-Engineering/Sherpa_Mini-Extruder)
