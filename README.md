# Forge, Hack Club's 3D Printer 

## This is an upcoming Hack Club You Ship, We Ship (YSWS) program. 
## Things are still under heavy development as the Forge Editor and the Forge Printer are WIP (Work in Progress) projects
## If you have any questions about the Development of Forge (and are a teen under 18), feel free to visit #forge-dev on the [Hack Club Slack!](https://hackclub.com/arcade/?param=slack)

**[Landing Page](https://forge.hackclub.com)** | **[Forge Editor](https://forge.hackclub.com/editor)** | **[Gallery](https://forge.hackclub.com/gallery)**

Find problems in the world and use programatic 3D modeling to solve them. Submit your solutions to the Gallery and get a Forge 3D Printer
Blot is a **You Ship, We Ship** project from [Hack Club](https://hackclub.com). 

> Forge is **fully open source**. The online editor can be freely used (or modified) by anyone, and we encourage all to submit to the gallery through PRs! However, you must be a teenager or younger to receive a free printer.

![image](https://github.com/Patcybermind/Project-Moonlight/assets/97562509/da8007c4-607d-4d73-ad87-6afdcd655585)

Building a model in the Forge editor is easy to learn, hard to master. You need abosultely no CAD or programming knowledge to get started. Since the Forge Editor is based on the [OpenSCAD](https://openscad.org) modeling language, it presents a challenge to even the most experienced parametric modelers. 

# About the Forge Printer

The printer is built after the [Flatpack](https://github.com/eponra/flatpack). However, this printer is signififcantly different internally than the Flatpack. Featuring a STM32 H5 series MCU and TMC2209 drivers on a custom Hack Club-engineered control board for near silent stepper motors and sensorless homing. The printer is designed to fit inside a filament box when folded, only taking up 220x210x75mm (not including the power supply or spool holder). Forge is the ultimate tool for competitions, club fairs, or hackathons.

## Specifications:

### These are Forge's specification as of July 16, 2024. As we begin test manufacturing Forge, these specifications may change.

 * STM32H563VITX 32-Bit MCU
 * TMC2209 Stepper Drivers
     - Reduces Motor Noise
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

