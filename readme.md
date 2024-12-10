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
import Module from './wgslpack.js';
const WGSLPack = await Module();

WGSLPack.init();
const packed = WGSLPack.pack(shader_code);
const unpacked = WGSLPack.unpack(packed);

console.log('Original length:', str.length, 'Packed length:', packed.length, 'Unpacked length:', unpacked.length, 'Unpacked:', unpacked);
```

## Build Requirements

- CMake 3.20+
- C++23 compatible compiler
- Emscripten (for WebAssembly build)

## Future Optimizations

### Binary Size Optimizations
- Function call compaction:
  - `textureLoad(VolumetricTexture, x_414, x_415);` → `textureLoad VolumetricTexture x_414 x_415` (5 bytes savings)
  - `myFunc(x_414, x_415);` → `myFunc x_414 x_415E` (3 bytes savings)
- Common pattern compaction:
  - `@group(1) @binding(12) var VolumetricDepthTexture : texture_2d<f32>;` → `binding 1 12 var VolumetricDepthTexture texture_2d f32` (11 bytes savings)
  - `diagnostic(off, derivative_uniformity);` → `diagnostic_off_derivative_uniformity` (6 bytes savings)
- Variable size optimization:
  - Use single-byte indices for variables up to 255 before switching to variable size encoding
  - Signal encoding scheme at file start

### Code Structure Optimizations (Do this at the tint level if possible)
- Dead Store Elimination (DSE):
  - Remove unnecessary temporaries and intermediate assignments
  - Example: `var x_435 : bool; x_435 = (x_429 < x_395.y); x_436 = x_435;` → `x_436 = (x_429 < x_395.y);`
- Code Block Consolidation:
  - Flatten nested control structures where possible

### Pattern Optimizations
- Constant Folding:

## Planned Features
- [ ] Optimization statistics reporting
- [ ] Configurable optimization levels
- [ ] Source maps for debugging

## License

Apache License 2.0

## Contributing

Pull requests welcome! Please read CONTRIBUTING.md first.