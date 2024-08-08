import { exportSTL } from './openscadMain.js';
import { returnSTL } from './openscadMain.js';
import { addSTLToQueue} from './stlViewer.js';

document.getElementById("console").innerHTML = "No errors yet!";

async function renderFunction() {
    console.log("render function is called");
    const code = window.getEditorContent();
    console.log(code);
    const stl = await returnSTL(code); // Ensure this is awaited
    addSTLToQueue(stl);

}

function exportFunction() {
    console.log("export function is called");
    const code = window.getEditorContent();
    console.log(code);
    exportSTL(code);
}

function downloadFunction() { // downloads a .scad
    console.log("download function is called");
    const filename = 'script.scad';
    const content = window.getEditorContent();
    const blob = new Blob([content], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = filename;
    link.click();
    URL.revokeObjectURL(url);
}


document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('renderBtn').addEventListener('click', renderFunction);
});
document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('exportBtn').addEventListener('click', exportFunction);
});
document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('downloadBtn').addEventListener('click', downloadFunction);
});