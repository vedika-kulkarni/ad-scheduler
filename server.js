const http = require('http');
const fs = require('fs');
const path = require('path');
const { execFile } = require('child_process');

const PORT = 3000;
const BINARY = process.platform === 'win32'
  ? path.join(__dirname, 'ad_scheduler.exe')
  : path.join(__dirname, 'ad_scheduler');

const MIME = {
  '.html': 'text/html',
  '.css':  'text/css',
  '.js':   'application/javascript',
  '.json': 'application/json',
  '.ico':  'image/x-icon',
};

const server = http.createServer((req, res) => {
  /* CORS */
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  if (req.method === 'OPTIONS') {
    res.writeHead(204);
    return res.end();
  }

  /* API */
  if (req.method === 'POST' && req.url === '/api/allocate') {
    let body = '';
    req.on('data', chunk => body += chunk);
    req.on('end', () => {
      // Ensure binary is executable on Linux
      if (process.platform !== 'win32') {
        try { fs.chmodSync(BINARY, 0o755); } catch(e) { console.error("Chmod failed:", e.message); }
      }

      const child = execFile(BINARY, [], (err, stdout, stderr) => {
        if (err) {
          res.writeHead(500, { 'Content-Type': 'application/json' });
          return res.end(JSON.stringify({ error: err.message, stderr }));
        }
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(stdout);
      });
      child.stdin.write(body);
      child.stdin.end();
    });
    return;
  }

  /* Static files */
  let filePath = path.join(__dirname, 'public',
    req.url === '/' ? 'index.html' : req.url);

  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.writeHead(404);
      return res.end('Not found');
    }
    const ext = path.extname(filePath);
    res.writeHead(200, { 'Content-Type': MIME[ext] || 'text/plain' });
    res.end(data);
  });
});

server.listen(PORT, () => {
  console.log(`\n╔══════════════════════════════════════╗`);
  console.log(`║  Ad Slot Allocator - DAA Project     ║`);
  console.log(`║  Server running on port ${PORT}         ║`);
  console.log(`║  Open: http://localhost:${PORT}          ║`);
  console.log(`╚══════════════════════════════════════╝\n`);
});
