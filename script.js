// Initialize CodeMirror
const editor = CodeMirror(document.getElementById('editor'), {
    mode: 'text/x-scad',
    theme: 'dracula',
    lineNumbers: true,
    matchBrackets: true,
    autoCloseBrackets: true,
    tabSize: 2
});

// Initialize Three.js
let scene = new THREE.Scene();
let camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
let renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth / 2, window.innerHeight);
document.getElementById('viewport').appendChild(renderer.domElement);

let geometry = new THREE.BoxGeometry();
let material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
let cube = new THREE.Mesh(geometry, material);
scene.add(cube);

camera.position.z = 5;

let controls = new THREE.OrbitControls(camera, renderer.domElement);

function animate() {
    requestAnimationFrame(animate);
    controls.update();
    renderer.render(scene, camera);
}

animate();

// Handle window resize
window.addEventListener('resize', () => {
    renderer.setSize(window.innerWidth / 2, window.innerHeight);
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
});

// Render Button Functionality
document.getElementById('render-btn').addEventListener('click', () => {
    const code = editor.getValue();
    openSCADWrapper.render(code);
});

// Slice Button Functionality
document.getElementById('slice-btn').addEventListener('click', () => {
    const code = editor.getValue();
    openSCADWrapper.slice(code);
});

// Download Button Functionality
document.getElementById('download-stl').addEventListener('click', () => {
    const blob = openSCADWrapper.downloadSTL();
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'model.stl';
    a.click();
    URL.revokeObjectURL(url);
});

document.getElementById('download-gcode').addEventListener('click', () => {
    const blob = openSCADWrapper.downloadGCode();
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'model.gcode';
    a.click();
    URL.revokeObjectURL(url);
});
