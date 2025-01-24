import { Canvas } from '@react-three/fiber'
import { OrbitControls, useGLTF, Edges } from '@react-three/drei'
import { useEffect, useRef } from 'react'
import { STLLoader } from 'three/addons/loaders/STLLoader.js'
import { useLoader } from '@react-three/fiber'

const ForgeModel = () => {
  const geometry = useLoader(STLLoader, '/models/anvil.stl')
  return (
    <mesh geometry={geometry}>
      <meshStandardMaterial color="#F95E1D" />
      <Edges
        scale={1}
        threshold={15}
        color="#ffffff"
      />
    </mesh>
  )
}

const ModelViewer = () => {
  return (
    <Canvas
      camera={{ position: [0, 0, 5], fov: 50 }}
      style={{ background: 'transparent' }}
    >
      <ambientLight intensity={0.5} />
      <pointLight position={[10, 10, 10]} />
      
      <ForgeModel />

      <OrbitControls enablePan={true} enableZoom={true} enableRotate={true} />
    </Canvas>
  )
}

export default ModelViewer