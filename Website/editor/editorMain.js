import { exportSTL } from './openscadMain.js';
import { returnSTL } from './openscadMain.js';

function renderFunction() {
    console.log("Render function is called");
    const code = window.getEditorContent();
    console.log(code);
    exportSTL(code);
}


document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('renderBtn').addEventListener('click', renderFunction);
});



function exportFunction() {
    console.log("export function is called");
    const code = window.getEditorContent();
    console.log(code);
    exportSTL(code);
}


document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('exportBtn').addEventListener('click', exportFunction);
});