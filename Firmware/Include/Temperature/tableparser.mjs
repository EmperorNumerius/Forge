"use strict";

import { readFileSync } from "fs";
import { stdout } from "process";
import { format } from "util";

var file = readFileSync("table").toString("utf-8").split("\n");
var table = {};

file.forEach((value) => {
    var splitted = value.split(" ");
    table[splitted[0].replace(".0", "")] = parseFloat(splitted[5])*100000;
});

console.log("static const uint16_t _TemperatureKeyTable[%d] = { // To regenerate, cd to this directory and `node tableparser.mjs`", Object.keys(table).length);
stdout.write("\t");
Object.keys(table).forEach((key, i) => {
    stdout.write("\t");
    if (i == Object.keys(table).length-1) {
        stdout.write("          ");
        stdout.write(key.padStart(3, " "));
    } else {
        stdout.write(format("%s, ", key.padStart(3, " ")));
    }
    if (i%5 == 4 || i == Object.keys(table).length-1) {
        console.log("");
        if (i%5 == 4 && i != Object.keys(table).length-1) {
            stdout.write("\t");
        }
    }
});
console.log("};");

console.log("");

console.log("static const float32_t _TemperatureValueTable[%d] = { // To regenerate, cd to this directory and `node tableparser.mjs`", Object.keys(table).length);
stdout.write("\t");
Object.values(table).forEach((value, i) => {
    if (i == Object.keys(table).length-1) {
        stdout.write("                          ");
        stdout.write(value.toFixed(7).padStart(11, " "));
    } else {
        stdout.write(format("%s, ", value.toFixed(7).padStart(11, " ")));
    }
    if (i%5 == 4 || i == Object.keys(table).length-1) {
        console.log("");
        
        if (i%5 == 4 && i != Object.keys(table).length-1) {
            stdout.write("\t");
        }
    }
});
console.log("};");
