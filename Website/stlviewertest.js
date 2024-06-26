import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
// three js docs https://threejs.org/docs/

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, 1, 0.1, 1000 );

const viewportSize = window.innerWidth /3;
const renderer = new THREE.WebGLRenderer();
renderer.setSize( viewportSize, viewportSize ); // ame sure to also change ratio in camera
document.body.appendChild( renderer.domElement );

// orbit controls setup https://threejs.org/docs/?q=orbit#examples/en/controls/OrbitControls
const controls = new OrbitControls( camera, renderer.domElement );
controls.enableDamping = true;
controls.dampingFactor = 1;
controls.rotateSpeed = 0.75;

const geometry = new THREE.BoxGeometry( 1, 1, 1 );
const material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
const cube = new THREE.Mesh( geometry, material );
scene.add( cube );

camera.position.z = 5;

function animate() {
    //cube.rotation.y += 0.01;
    //cube.rotation.x += 0.01;
    
	controls.update();
    
	renderer.render( scene, camera );
}

renderer.setAnimationLoop( animate );