import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';

// three js docs https://threejs.org/docs/

// camera
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 90, 1, 0.1, 1000 );
const viewportSize = window.innerWidth /3;
const renderer = new THREE.WebGLRenderer();
renderer.setSize( viewportSize, viewportSize ); // make sure to also change ratio in camera
document.body.appendChild( renderer.domElement );



// orbit controls setup https://threejs.org/docs/?q=orbit#examples/en/controls/OrbitControls
//const controls = new OrbitControls( camera, renderer.domElement );
//controls.rotateSpeed = 1.5;
camera.position.set( 5, 6, 0 ); // this seems to be buggy
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
scene.background = new THREE.Color(0xaaaaaa);

// guides
const gridHelper = new THREE.GridHelper( 10, 10 );
scene.add( gridHelper );

// shadowplane
var planeMaterial = new THREE.ShadowMaterial();
planeMaterial.opacity = .25;
var newPlaneGeometry = new THREE.PlaneGeometry( 100, 100 );
var planeMesh = new THREE.Mesh( newPlaneGeometry, planeMaterial );
planeMesh.receiveShadow = true;
planeMesh.rotation.x = - Math.PI / 2;
scene.add( planeMesh );

// Binary files
const stlMaterial = new THREE.MeshPhongMaterial( { color: 0xAAAA00, specular: 0x000000, shininess: 20 } );
//
//loader.load( './models/test.stl', function ( geometry ) {
//
//    const mesh = new THREE.Mesh( geometry, stlMaterial );
//
//    mesh.position.set( 0, 6, - 0.6 );
//    mesh.rotation.set( - Math.PI / 2, 0, 0 );
//    mesh.scale.set( 2, 2, 2 );
//    
//
//    mesh.castShadow = true;
//    mesh.receiveShadow = true;
//
//    scene.add( mesh );
//
//} );


camera.position.z = 5;

function animate() {
    //requestAnimationFrame(animate);
	//controls.update();
    
    
	renderer.render( scene, camera );
}

renderer.setAnimationLoop(animate);

//export function renderSTL(stl) {
//    loader.load( stl, function ( geometry ) {
//        const mesh = new THREE.Mesh( geometry, stlMaterial );
//        scene.add( mesh );
//    } );
//}
//const loader = new STLLoader();

export function renderSTL(stlArrayBuffer) {
    console.log("renderSTL called with ArrayBuffer:", stlArrayBuffer);
    const loader = new STLLoader();
    try {
        loader.parse(stlArrayBuffer, function (geometry) {
            console.log("loader.parse called");
            console.log('Geometry:', geometry);
            const mesh = new THREE.Mesh(geometry, stlMaterial);
            console.log('Mesh:', mesh);
            mesh.castShadow = true;
            mesh.receiveShadow = true;
            scene.add(mesh);
            console.log('Mesh added to scene');

            // Adjust the position and scale if necessary
            mesh.position.set(0, 0, 0);
            mesh.scale.set(1, 1, 1);
            console.log('Mesh position and scale set');

            // Ensure the camera is positioned to view the model
            camera.position.set(5, 6, 10);
            camera.lookAt(mesh.position);
            console.log('Camera position set and looking at mesh');

            // Update controls
            //controls.update();
            console.log('Controls updated');

            // Log camera and mesh positions
            console.log('Camera Position:', camera.position);
            console.log('Mesh Position:', mesh.position);

            // Render the scene
            renderer.render(scene, camera);
            console.log('Scene rendered');
        });
    } catch (error) {
        console.error("Error parsing STL:", error);
    }
}

