import { defineConfig } from 'vite'
import fs from 'fs'

export default defineConfig({
  server: {
    host: '0.0.0.0',
    port: 5173,
    strictPort: true,
    https: {
      key: fs.readFileSync('/certs/key.pem'),
      cert: fs.readFileSync('/certs/cert.pem')
    },
    proxy: {
      '/api': {
        target: 'https://backend:3000', // ton backend HTTPS
        changeOrigin: true,
        secure: false, // permet de bypasser le self-signed
      }
    }
  }
})

