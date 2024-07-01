import { exportSTL } from './openscadMain.js';
import { returnSTL } from './openscadMain.js';
import { renderSTL } from './stlViewer.js';



async function renderFunction() {
    console.log("Render function is called");
    const code = window.getEditorContent();
    console.log(code);
    const stlArrayBuffer = await returnSTL(code); // Ensure this is awaited
    console.log('STL ArrayBuffer:', stlArrayBuffer);

    if (stlArrayBuffer) {
        renderSTL(stlArrayBuffer); // Directly pass the ArrayBuffer to renderSTL
    } else {
        console.error("Failed to generate STL");
    }
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