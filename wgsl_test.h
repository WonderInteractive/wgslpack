const static std::string wgsl_test = R"(
diagnostic(off, derivative_uniformity);

struct type_View {
  @size(2368)
  padding_0 : u32,
  /* @offset(2368) */
  View_ViewSizeAndInvSize : vec4f,
  @size(32)
  padding_1 : u32,
  /* @offset(2416) */
  View_BufferSizeAndInvSize : vec4f,
  /* @offset(2432) */
  View_BufferBilinearUVMinMax : vec4f,
}

struct type__Globals {
  /* @offset(0) */
  TileBufferExtent : vec2u,
  /* @offset(8) */
  SamplePixelToHZBUV : vec2f,
}

alias Arr = array<f32, 8u>;

struct FSSRTTileInfos {
  /* @offset(0) */
  Directionality : Arr,
  /* @offset(32) */
  ClosestDeviceZ : f32,
  /* @offset(36) */
  Coord : vec2u,
}

alias RTArr = array<FSSRTTileInfos>;

struct type_RWStructuredBuffer_FSSRTTileInfos {
  /* @offset(0) */
  field0 : RTArr,
}

@group(0) @binding(1) var<uniform> View : type_View;

@group(0) @binding(5) var View_SharedPointClampedSampler : sampler;

@group(0) @binding(0) var<uniform> x_Globals : type__Globals;

@group(0) @binding(4) var ClosestHZBTexture : texture_2d<f32>;

@group(0) @binding(2) var<storage, read_write> TileClassificationBufferOutput : type_RWStructuredBuffer_FSSRTTileInfos;

@group(0) @binding(3) var DebugOutput : texture_storage_2d<rgba32float, write>;

var<private> x_3 : vec3u;

var<private> x_4 : vec3u;

const x_64 = vec2f(-0.5f);

const x_65 = vec2f(0.5f, -0.5f);

const x_66 = vec2f(-0.5f, 0.5f);

const x_39 = vec2f(0.5f);

fn main_00003168_74cf6f70_1() {
  var x_123 : f32;
  var x_124 : f32;
  var x_138 : f32;
  var x_139 : f32;
  var x_153 : f32;
  var x_154 : f32;
  var x_168 : f32;
  var x_169 : f32;
  var x_201 : f32;
  var x_202 : f32;
  var x_216 : f32;
  var x_217 : f32;
  var x_231 : f32;
  var x_232 : f32;
  var x_246 : f32;
  var x_247 : f32;
  var x_277 : f32;
  var x_278 : f32;
  var x_292 : f32;
  var x_293 : f32;
  var x_307 : f32;
  var x_308 : f32;
  var x_322 : f32;
  var x_323 : f32;
  var x_353 : f32;
  var x_354 : f32;
  var x_368 : f32;
  var x_369 : f32;
  var x_383 : f32;
  var x_384 : f32;
  var x_398 : f32;
  var x_399 : f32;
  var x_428 : f32;
  var x_429 : f32;
  var x_443 : f32;
  var x_444 : f32;
  var x_458 : f32;
  var x_459 : f32;
  var x_473 : f32;
  var x_474 : f32;
  var x_491 : bool;
  var x_492 : bool;
  let x_70 = x_3;
  let x_71 = x_3.xy;
  let x_73 = x_4.z;
  let x_78 = View.View_ViewSizeAndInvSize;
  let x_80 = (vec2f(((x_71 * vec2u(8u)) + vec2u(4u))) * x_78.zw);
  let x_82 = (f32(x_73) * 0.78539818525314331055f);
  let x_85 = vec2f(cos(x_82), sin(x_82));
  let x_86 = x_78.xy;
  let x_91 = x_Globals.SamplePixelToHZBUV;
  let x_95 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, ((x_80 * x_86) * x_91), 2.0f).x;
  let x_97 = max(ceil(1.614223480224609375f), 3.0f);
  let x_98 = pow(2.0f, x_97);
  let x_99 = fma(2.0f, x_98, 8.0f);
  let x_102 = fma(x_80, x_86, (x_85 * min(8.0f, 8.0f)));
  let x_104 = (x_102 + (x_64 * x_98));
  let x_107 = View.View_BufferSizeAndInvSize.zw;
  let x_108 = (x_104 * x_107);
  let x_110 = View.View_BufferBilinearUVMinMax;
  let x_111 = x_110.xy;
  let x_112 = x_110.zw;
  if (all((x_108 == clamp(x_108, x_111, x_112)))) {
    x_123 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_104 * x_91), (x_97 - 1.0f)).x;
    x_124 = x_123;
  } else {
    x_124 = 0.0f;
  }
  let x_126 = (x_102 + (x_65 * x_98));
  let x_127 = (x_126 * x_107);
  if (all((x_127 == clamp(x_127, x_111, x_112)))) {
    x_138 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_126 * x_91), (x_97 - 1.0f)).x;
    x_139 = x_138;
  } else {
    x_139 = 0.0f;
  }
  let x_141 = (x_102 + (x_66 * x_98));
  let x_142 = (x_141 * x_107);
  if (all((x_142 == clamp(x_142, x_111, x_112)))) {
    x_153 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_141 * x_91), (x_97 - 1.0f)).x;
    x_154 = x_153;
  } else {
    x_154 = 0.0f;
  }
  let x_156 = (x_102 + (x_39 * x_98));
  let x_157 = (x_156 * x_107);
  if (all((x_157 == clamp(x_157, x_111, x_112)))) {
    x_168 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_156 * x_91), (x_97 - 1.0f)).x;
    x_169 = x_168;
  } else {
    x_169 = 0.0f;
  }
  let x_174 = View.View_ViewSizeAndInvSize.z;
  let x_178 = max(0.0f, ((max(max(x_124, x_139), max(x_154, x_169)) - x_95) / (8.0f * x_174)));
  let x_182 = max(ceil(log2((x_99 * 0.3826834559440612793f))), 3.0f);
  let x_183 = pow(2.0f, x_182);
  let x_184 = fma(2.0f, x_183, x_99);
  let x_187 = fma(x_80, x_86, (x_85 * min(x_99, x_99)));
  let x_189 = (x_187 + (x_64 * x_183));
  let x_190 = (x_189 * x_107);
  if (all((x_190 == clamp(x_190, x_111, x_112)))) {
    x_201 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_189 * x_91), (x_182 - 1.0f)).x;
    x_202 = x_201;
  } else {
    x_202 = 0.0f;
  }
  let x_204 = (x_187 + (x_65 * x_183));
  let x_205 = (x_204 * x_107);
  if (all((x_205 == clamp(x_205, x_111, x_112)))) {
    x_216 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_204 * x_91), (x_182 - 1.0f)).x;
    x_217 = x_216;
  } else {
    x_217 = 0.0f;
  }
  let x_219 = (x_187 + (x_66 * x_183));
  let x_220 = (x_219 * x_107);
  if (all((x_220 == clamp(x_220, x_111, x_112)))) {
    x_231 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_219 * x_91), (x_182 - 1.0f)).x;
    x_232 = x_231;
  } else {
    x_232 = 0.0f;
  }
  let x_234 = (x_187 + (x_39 * x_183));
  let x_235 = (x_234 * x_107);
  if (all((x_235 == clamp(x_235, x_111, x_112)))) {
    x_246 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_234 * x_91), (x_182 - 1.0f)).x;
    x_247 = x_246;
  } else {
    x_247 = 0.0f;
  }
  let x_254 = max(x_178, ((max(max(x_202, x_217), max(x_232, x_247)) - x_95) / (x_99 * x_174)));
  let x_258 = max(ceil(log2((x_184 * 0.3826834559440612793f))), 3.0f);
  let x_259 = pow(2.0f, x_258);
  let x_260 = fma(2.0f, x_259, x_184);
  let x_263 = fma(x_80, x_86, (x_85 * min(x_184, x_184)));
  let x_265 = (x_263 + (x_64 * x_259));
  let x_266 = (x_265 * x_107);
  if (all((x_266 == clamp(x_266, x_111, x_112)))) {
    x_277 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_265 * x_91), (x_258 - 1.0f)).x;
    x_278 = x_277;
  } else {
    x_278 = 0.0f;
  }
  let x_280 = (x_263 + (x_65 * x_259));
  let x_281 = (x_280 * x_107);
  if (all((x_281 == clamp(x_281, x_111, x_112)))) {
    x_292 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_280 * x_91), (x_258 - 1.0f)).x;
    x_293 = x_292;
  } else {
    x_293 = 0.0f;
  }
  let x_295 = (x_263 + (x_66 * x_259));
  let x_296 = (x_295 * x_107);
  if (all((x_296 == clamp(x_296, x_111, x_112)))) {
    x_307 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_295 * x_91), (x_258 - 1.0f)).x;
    x_308 = x_307;
  } else {
    x_308 = 0.0f;
  }
  let x_310 = (x_263 + (x_39 * x_259));
  let x_311 = (x_310 * x_107);
  if (all((x_311 == clamp(x_311, x_111, x_112)))) {
    x_322 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_310 * x_91), (x_258 - 1.0f)).x;
    x_323 = x_322;
  } else {
    x_323 = 0.0f;
  }
  let x_330 = max(x_254, ((max(max(x_278, x_293), max(x_308, x_323)) - x_95) / (x_184 * x_174)));
  let x_334 = max(ceil(log2((x_260 * 0.3826834559440612793f))), 3.0f);
  let x_335 = pow(2.0f, x_334);
  let x_336 = fma(2.0f, x_335, x_260);
  let x_339 = fma(x_80, x_86, (x_85 * min(x_260, x_260)));
  let x_341 = (x_339 + (x_64 * x_335));
  let x_342 = (x_341 * x_107);
  if (all((x_342 == clamp(x_342, x_111, x_112)))) {
    x_353 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_341 * x_91), (x_334 - 1.0f)).x;
    x_354 = x_353;
  } else {
    x_354 = 0.0f;
  }
  let x_356 = (x_339 + (x_65 * x_335));
  let x_357 = (x_356 * x_107);
  if (all((x_357 == clamp(x_357, x_111, x_112)))) {
    x_368 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_356 * x_91), (x_334 - 1.0f)).x;
    x_369 = x_368;
  } else {
    x_369 = 0.0f;
  }
  let x_371 = (x_339 + (x_66 * x_335));
  let x_372 = (x_371 * x_107);
  if (all((x_372 == clamp(x_372, x_111, x_112)))) {
    x_383 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_371 * x_91), (x_334 - 1.0f)).x;
    x_384 = x_383;
  } else {
    x_384 = 0.0f;
  }
  let x_386 = (x_339 + (x_39 * x_335));
  let x_387 = (x_386 * x_107);
  if (all((x_387 == clamp(x_387, x_111, x_112)))) {
    x_398 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_386 * x_91), (x_334 - 1.0f)).x;
    x_399 = x_398;
  } else {
    x_399 = 0.0f;
  }
  let x_406 = max(x_330, ((max(max(x_354, x_369), max(x_384, x_399)) - x_95) / (x_260 * x_174)));
  let x_410 = max(ceil(log2((x_336 * 0.3826834559440612793f))), 3.0f);
  let x_411 = pow(2.0f, x_410);
  let x_414 = fma(x_80, x_86, (x_85 * min(x_336, x_336)));
  let x_416 = (x_414 + (x_64 * x_411));
  let x_417 = (x_416 * x_107);
  if (all((x_417 == clamp(x_417, x_111, x_112)))) {
    x_428 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_416 * x_91), (x_410 - 1.0f)).x;
    x_429 = x_428;
  } else {
    x_429 = 0.0f;
  }
  let x_431 = (x_414 + (x_65 * x_411));
  let x_432 = (x_431 * x_107);
  if (all((x_432 == clamp(x_432, x_111, x_112)))) {
    x_443 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_431 * x_91), (x_410 - 1.0f)).x;
    x_444 = x_443;
  } else {
    x_444 = 0.0f;
  }
  let x_446 = (x_414 + (x_66 * x_411));
  let x_447 = (x_446 * x_107);
  if (all((x_447 == clamp(x_447, x_111, x_112)))) {
    x_458 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_446 * x_91), (x_410 - 1.0f)).x;
    x_459 = x_458;
  } else {
    x_459 = 0.0f;
  }
  let x_461 = (x_414 + (x_39 * x_411));
  let x_462 = (x_461 * x_107);
  if (all((x_462 == clamp(x_462, x_111, x_112)))) {
    x_473 = textureSampleLevel(ClosestHZBTexture, View_SharedPointClampedSampler, (x_461 * x_91), (x_410 - 1.0f)).x;
    x_474 = x_473;
  } else {
    x_474 = 0.0f;
  }
  let x_481 = max(x_406, ((max(max(x_429, x_444), max(x_459, x_474)) - x_95) / (x_336 * x_174)));
  let x_482 = x_70.x;
  let x_484 = x_Globals.TileBufferExtent.x;
  x_492 = false;
  if ((x_482 < x_484)) {
    x_491 = (x_70.y < x_Globals.TileBufferExtent.y);
    x_492 = x_491;
  }
  if (x_492) {
    let x_497 = (x_482 + (x_70.y * x_484));
    TileClassificationBufferOutput.field0[x_497].Directionality[x_73] = x_481;
    if ((x_73 == 0u)) {
      TileClassificationBufferOutput.field0[x_497].ClosestDeviceZ = x_95;
    }
  }
  if ((x_73 == 0u)) {
    textureStore(DebugOutput, vec2i(x_71), vec4f((x_481 * 0.31830987334251403809f), x_95, 0.0f, 0.0f));
  }
  return;
}

@compute @workgroup_size(8i, 8i, 8i)
fn main(@builtin(global_invocation_id) x_3_param : vec3u, @builtin(local_invocation_id) x_4_param : vec3u) {
  x_3 = x_3_param;
  x_4 = x_4_param;
  main_00003168_74cf6f70_1();
})";