import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';

// three js docs https://threejs.org/docs/

// camera
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, 1, 0.1, 1000 );
const viewportSize = window.innerWidth /3;
const renderer = new THREE.WebGLRenderer();
renderer.setSize( viewportSize, viewportSize ); // make sure to also change ratio in camera
document.body.appendChild( renderer.domElement );
camera.position.set( 20,20,20 );


// orbit controls setup https://threejs.org/docs/?q=orbit#examples/en/controls/OrbitControls
const controls = new OrbitControls( camera, renderer.domElement );
controls.rotateSpeed = 1;

camera.position.set( 20, 20, 20 );

// lighting
const ambientLight = new THREE.AmbientLight(0xffffff, 1);
scene.add(ambientLight);

const directionalLight = new THREE.DirectionalLight(0xffffff, 3);
directionalLight.position.set(10, 10, 10);
scene.add(directionalLight);
const directionalLight2 = new THREE.DirectionalLight(0xffffff, 2);
directionalLight2.position.set(0, -10, -10);
scene.add(directionalLight2);

// background
scene.background = new THREE.Color(0x666666);

// guides
const gridHelper = new THREE.GridHelper( 10, 10 );
scene.add( gridHelper );

// Binary files
const loader = new STLLoader();

const stlMaterial = new THREE.MeshPhongMaterial( { color: 0xAAAA00, specular: 0x111111, shininess: 200 } );

loader.load( './models/test.stl', function ( geometry ) {

    const mesh = new THREE.Mesh( geometry, stlMaterial );

    mesh.position.set( 0, - 0.37, - 0.6 );
    mesh.rotation.set( - Math.PI / 2, 0, 0 );
    mesh.scale.set( 2, 2, 2 );

    mesh.castShadow = true;
    mesh.receiveShadow = true;

    scene.add( mesh );

} );


camera.position.z = 5;

function animate() {
    
	controls.update();
    
	renderer.render( scene, camera );
}

renderer.setAnimationLoop( animate );