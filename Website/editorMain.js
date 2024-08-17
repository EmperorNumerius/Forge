import { exportSTL } from './openscadMain.js';
import { returnSTL } from './openscadMain.js';
import { addSTLToQueue} from './stlViewer.js';
import { setMonacoEditorContent } from './monacoEditorMain.js';

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

function openFunction() {
    console.log("open function is called");

    // Create an input element of type file
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.scad'; // Accept only .scad files

    // Add an event listener to handle file selection
    input.addEventListener('change', (event) => {
        const file = event.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = function(e) {
                const content = e.target.result;
                console.log("File content:", content);
                // You can now use the content variable as needed
                // For example, you can set it to the editor content
                setMonacoEditorContent(content);
            };
            reader.readAsText(file);
        }
    });

    // Trigger the file input click
    input.click();
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
document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById('openBtn').addEventListener('click', openFunction);
});