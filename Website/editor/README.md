# Forge editor

This is the forge editor site. 

Forge is a work in progress 3d printing ysws project being worked on by teens at [hackclub](https://hackclub.com)
You can find the official forge/project-moonlight repository [here](https://github.com/EmperorNumerius/Project-Moonlight)

If you wish to contribute you can join the hackclub slack and join #forge-dev

# Goal
The current plan is top have an openscad editor on the left that uses microsoft's monaco-editor and have a viewport on the right that lets
you see what your current openscad program looks like.

[openscad playground][https://ochafik.com/openscad2/]

kind of like this 

this will be as easy to use as possible

# to run:
First install all the dependencies in the /website directory with npm:
## dependencies:
    three.js
    monaco-editor
## to run the server:

run npx vite in the terminal assuming its installed

I tried the vscode live-server but the three.js code doesnt run, I would need to change the imports and things

cd into /website/editor and run npx vite and open it on your browser