import { exportSTL } from './openscadMain.js';
import { returnSTL } from './openscadMain.js';
//import { renderSTL } from './stlViewer.js';
import { addSTLToQueue} from './stlViewer.js';

document.getElementById("console").innerHTML = "No errors yet!";

async function renderFunction() {
    console.log("Render function is called");
    const code = window.getEditorContent();
    console.log(code);
    const stl = await returnSTL(code); // Ensure this is awaited
    //console.log('STL ArrayBuffer:', stlArrayBuffer);
    addSTLToQueue(stl);

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