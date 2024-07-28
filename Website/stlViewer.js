import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';

// three js docs https://threejs.org/docs/

// camera
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 60, 1, 0.1, 1000 ); // ratio is 1 and first is fov
const renderer = new THREE.WebGLRenderer();
// Assuming there's a div with id="myDiv"
const myDiv = document.getElementById('viewer');
// Get the dimensions of the div
const divWidth = myDiv.clientWidth;
const divHeight = myDiv.clientHeight;
// Set the renderer size to match the div's dimensions
renderer.setSize(divWidth, divHeight);
// Update the camera's aspect ratio to match the new size
camera.aspect = divWidth / divHeight;
camera.updateProjectionMatrix();
// Append the renderer's DOM element to the div instead of document.body
myDiv.appendChild(renderer.domElement);



// orbit controls setup https://threejs.org/docs/?q=orbit#examples/en/controls/OrbitControls
const controls = new OrbitControls( camera, renderer.domElement );
//controls.rotateSpeed = 1.5;
camera.position.set( 0, 10, 20 ); // this seems to be buggy
//controls.update();

// lighting
const ambientLight = new THREE.AmbientLight(0xffffff, 1);
scene.add(ambientLight);
// Enable shadows in the renderer
renderer.shadowMap.enabled = true;
const directionalLight = new THREE.DirectionalLight(0xffffff, 3);
directionalLight.position.set(10, 10, 10);
directionalLight.castShadow = true;
scene.add(directionalLight);
const directionalLight2 = new THREE.DirectionalLight(0xffffff, 2);
directionalLight2.position.set(0, -10, -10);
scene.add(directionalLight2);

// background
scene.background = new THREE.Color(0x151515);

// guides
const gridHelper = new THREE.GridHelper( 10, 10 );
scene.add( gridHelper );

// shadowplane
var planeMaterial = new THREE.ShadowMaterial();
planeMaterial.opacity = .5; // darkness of the shadow
var newPlaneGeometry = new THREE.PlaneGeometry( 100, 100 );
var planeMesh = new THREE.Mesh( newPlaneGeometry, planeMaterial );
planeMesh.receiveShadow = true;
planeMesh.rotation.x = - Math.PI / 2;
scene.add( planeMesh );


const stlMaterial = new THREE.MeshPhongMaterial( { color: 0xAAAA00, specular: 0x000000, shininess: 20 } );

let readyToRender = null;

function animate() {
	//controls.update();
    renderSTL(readyToRender);
    /*if (divWidth !== myDiv.clientWidth || divHeight !== myDiv.clientHeight) {
        // Get the dimensions of the div
        const divWidth = myDiv.clientWidth;
        const divHeight = myDiv.clientHeight;

        // Set the renderer size to match the div's dimensions
        renderer.setSize(divWidth, divHeight);

        // Update the camera's aspect ratio to match the new size
        camera.aspect = divWidth / divHeight;
        camera.updateProjectionMatrix();
    }*/
	renderer.render( scene, camera );

}
renderer.setAnimationLoop(animate);


export function addSTLToQueue(stlArrayBuffer) {
    readyToRender = stlArrayBuffer;
}

let currentMesh = null;

function renderSTL(stlArrayBuffer) {
    if (stlArrayBuffer === null) {
    } else {
        readyToRender = null;
        console.log("renderSTL called with ArrayBuffer:", stlArrayBuffer);
        const loader = new STLLoader();
        loader.load( stlArrayBuffer, function ( geometry ) {
            console.log("loader entered")
            // Remove the previous mesh if it exists
            if (currentMesh) {
                scene.remove(currentMesh);
                currentMesh.geometry.dispose();
                currentMesh.material.dispose();
                currentMesh = null;
            }
            const mesh = new THREE.Mesh(geometry, stlMaterial);    
            mesh.rotation.set(-Math.PI / 2, 0, 0);
            mesh.castShadow = true;
            mesh.receiveShadow = true;
            scene.add(mesh);
            console.log("mesh added to scene");

            // Update the current mesh reference
            currentMesh = mesh;

        });
    }
    
}


