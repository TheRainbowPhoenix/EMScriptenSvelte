// watcher.js

import chokidar from 'chokidar';
import { join } from 'path';
import { spawn } from 'child_process';

const fileToWatch = 'wasm/hello.cpp';
const bashCommand = 'rebuild.bat';

const filePath = join(process.cwd(), fileToWatch);

const watcher = chokidar.watch(filePath);

watcher.on('change', async (path) => {
  console.log(`File ${path} has changed. Running bash command...`);

  // Run the bash command using execa
  const childProcess = spawn(bashCommand, [], { stdio: 'inherit' });


  childProcess.on('error', (err) => {
    console.error('Error running bash command:', err.message);
  });

  childProcess.on('close', (code) => {
    if (code !== 0) {
      console.error(`Bash command exited with code ${code}`);
    } else {
      console.log('Bash command completed successfully');
    }
  });

    
});

console.log(`Watching file: ${fileToWatch}`);
