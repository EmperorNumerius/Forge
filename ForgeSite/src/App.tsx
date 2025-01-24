import { useEffect, useRef } from 'react'
import { AppBar, Toolbar, Typography, Button, Box } from '@mui/material'
import forgeLogo from './assets/forgeLogo.png'
import './App.css'
import ModelViewer from './components/ModelViewer'

function App() {
  return (
    <>
      <AppBar position="fixed" sx={{ background: 'transparent', boxShadow: 'none' }}>
        <Toolbar sx={{ justifyContent: 'space-between', py: 2 }}>
          <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
            <img src={forgeLogo} alt="Forge Logo" style={{ height: '40px' }} />
            <Typography 
              variant="h4" 
              sx={{ 
                fontFamily: 'Grenze, serif',
                color: '#F95E1D',
                fontWeight: 'bold'
              }}
            >
              Forge
            </Typography>
          </Box>

          <Box sx={{ display: 'flex', gap: 4, alignItems: 'center' }}>
            <Button 
              href="#" 
              sx={{ 
                color: '#F95E1D',
                textDecoration: 'underline',
                textUnderlineOffset: '4px',
                '&:hover': {
                  textUnderlineOffset: '5px',
                  background: 'transparent'
                }
              }}
            >
              Home
            </Button>
            <Button sx={{ color: '#EEE' }} href="#">Gallery</Button>
            <Button 
              sx={{ color: '#EEE' }} 
              href="https://github.com/EmperorNumerius/Forge"
            >
              Github
            </Button>
            <Button 
              variant="contained"
              href="https://forge.hackclub.com/editor/"
              sx={{
                bgcolor: '#F95E1D',
                color: '#222',
                px: 4,
                '&:hover': {
                  bgcolor: '#F95E1DEE'
                }
              }}
            >
              Editor
            </Button>
          </Box>
        </Toolbar>
      </AppBar>

      <div className="relative">
        <main className="min-h-screen">
          <Box sx={{ 
            display: 'flex', 
            flexDirection: 'row',
            alignItems: 'center',
            gap: 4,
            pt: 12,
            px: 8
          }}>
            <Box sx={{ flex: '0 0 40%' }}>
              <h1 className="title text-[#F95E1D] text-5xl font-extrabold glow mb-3">
                Forge
              </h1>
              <p className="text-[#EEE] text-base leading-relaxed max-w-sm">
                Forge is an open-source, hackable 3D printer<br />
                & OpenSCAD-based editor made by teens<br />
                at Hack Club. If you're a high schooler<br />
                (or younger), you can get one for free!
              </p>
            </Box>
            <Box sx={{ flex: '0 0 60%', height: '700px' }}>
              <ModelViewer />
            </Box>
          </Box>
        </main>
      </div>
    </>
  )
}

export default App