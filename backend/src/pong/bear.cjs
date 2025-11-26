const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const projectRoot = path.resolve(__dirname, '../..');
const srcDir = path.join(projectRoot, 'src/pong/srcs');

const nodeVersion = process.version.replace(/^v/, '');

const nodeGypCache = path.join(process.env.HOME, '.cache', 'node-gyp', nodeVersion, 'include', 'node');

const includeDirs = [
  path.join(projectRoot, 'src/pong/includes'),
  path.join(projectRoot, 'node_modules/node-addon-api'),
  nodeGypCache
];

const cppFiles = fs.readdirSync(srcDir).filter(f => f.endsWith('.cpp'));

const compileCommands = cppFiles.map(file => {
  const command = [
    'g++',
    '-std=c++17',
    ...includeDirs.map(dir => `-I${dir}`),
    '-DNODE_GYP_MODULE_NAME=addon',
    '-DUSING_UV',
    '-DUSING_V8',
    '-fPIC',
    '-Wall',
    '-Wextra',
    '-c',
    path.join(srcDir, file)
  ].join(' ');

  return {
    directory: srcDir,
    command,
    file: path.join(srcDir, file)
  };
});

fs.writeFileSync(
  path.join(projectRoot, 'src/pong/compile_commands.json'),
  JSON.stringify(compileCommands, null, 2)
);
