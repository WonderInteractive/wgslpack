# WGSLPack

A fast, lightweight minifier and binary serializer for WGSL (WebGPU Shading Language) shaders.

## Features

- Minifies WGSL code by removing unnecessary whitespace and comments
- Renames variables to shorter forms while preserving functionality
- Serializes to a binary format for smaller file sizes
- Optional ZSTD compression
- Available as both a CLI tool and WebAssembly module

## Installation

### CLI Tool
```bash
# Build from source
git clone https://github.com/yourusername/wgslpack
cd wgslpack
mkdir build
cd build
cmake ..
cmake --build .
OR for Emscripten Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### WebAssembly
```bash
npm install wgslpack
```

## Usage

### CLI
```bash
# Basic minification
wgslpack ./source.wgsl -o ./dest.wgsl

# With compression
wgslpack ./source.wgsl -o ./dest.wgsl --compress
```

### JavaScript/WebAssembly
```javascript
// ES6 Module
import { WGSLPack } from 'wgslpack';

const pack = await WGSLPack.init();
const minified = pack.process(sourceCode);
```

## Build Requirements

- CMake 3.20+
- C++23 compatible compiler
- Emscripten (for WebAssembly build)

## License

Apache License 2.0

## Contributing

Pull requests welcome! Please read CONTRIBUTING.md first.