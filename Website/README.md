# Forge editor

This is the forge editor site. Forge is a work in progress 3D printer YSWS(You Ship We Ship) project being worked on by teens at [hackclub](https://hackclub.com). You can find the official forge repository [here](https://github.com/EmperorNumerius/Forge), and if you wish to contribute, you should join the hackclub slack and visit the channel #forge-dev.

## Goal

The current plan is to have Microsoft's Monaco Editor with a OpenSCAD program in it, and a viewport that renders the current model. This is what has been implemented in the website already.

It is intended to look kind of like [this](https://ochafik.com/openscad2/).

## Run instructions

First off, cd into the website's directory(the website directory in the repository root), and run `npm install` to install all dependencies. After that, you can just run `npx vite` to properly run everything. Note that other servers(including the live server extension to VS Code) won't work for whatever reason. Also, apparently sometimes rendering doesn't work and you need to restart vite.
