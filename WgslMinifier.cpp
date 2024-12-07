#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>
#include <print>
#include <cassert>
#include <fstream>
#include <algorithm>
#include "tint_headers.h"
#include "wgsl_test.h"
#ifdef WGSLPACK_USE_COMPRESSION
#include "zstd.c"
#endif
#undef _CHAR
#undef IN
#undef DELETE
#undef INLINE
#undef FALSE
#undef TRUE
#undef CONST
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#define EXPORT_METHOD extern "C" EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT_METHOD
#endif
enum WGSLTokenTypes : uint16_t {
    _CHAR,
    IDENTIFIER,
    SHORTSWIZZLE,
    SWIZZLE,
    NEWLINE,
    NUMBER4,
    NUMBER8,
    UNSIGNED_NUMBER4,
    UNSIGNED_NUMBER8,
    FLOAT4,
    FLOAT8,
    U, I,
    IN,             // 'in' keyword
    // high frequency types
    LET, VAR, IF, DOT, FMA, VEC2, VEC3, VEC4, VEC2I, VEC3I, VEC4I,
    VEC2U, VEC3U, VEC4U, VEC2F, VEC3F, VEC4F, VEC2H, VEC3H, VEC4H,
    BITCAST, CLAMP, SELECT, INVERSE_SQRT,
    // Types
    _BOOL, F32, F16, I32, _U32, ARRAY, ATOMIC,
    // Matrix Types
    MAT2X2F, MAT2X2H, MAT2X3F, MAT2X3H, MAT2X4F, MAT2X4H, MAT3X2F, MAT3X2H, MAT3X3F,
    MAT3X3H, MAT3X4F, MAT3X4H, MAT4X2F, MAT4X2H, MAT4X3F, MAT4X3H, MAT4X4F, MAT4X4H,
    // Operators
    LEFT_PARAM, RIGHT_PARAM, LEFT_BRACKET, RIGHT_BRACKET, LEFT_ANGLE, RIGHT_ANGLE,
    SEMICOLON, COMMA, PERIOD, ASSIGN, MINUS, PLUS, MULTIPLY, DIVIDE, NOT, OR, AND,
    XOR, TILDE, MODULO, QUESTION, COLON, AT, HASH, DOLLAR, BANG, PIPE, AMPERSAND,
    CARET, PERCENT, LEFT_CURLY, RIGHT_CURLY,

    
    // Additional
    NULLUC, SELF, ABSTRACT, ACTIVE, ALIGNAS, ALIGNOF, AS, ASM, ASM_FRAGMENT,
    ASYNC, ATTRIBUTE, AUTO, AWAIT, BECOME, BINDING_ARRAY, CAST, CATCH, CLASS,
    CO_AWAIT, CO_RETURN, CO_YIELD, COHERENT, COLUMN_MAJOR, COMMON, COMPILE,
    COMPILE_FRAGMENT, CONCEPT, CONST_CAST, CONSTEVAL, CONSTEXPR, CONSTINIT,
    CRATE, DEBUGGER, DECLTYPE, DELETE, DEMOTE, DEMOTE_TO_HELPER, DO,
    DYNAMIC_CAST, ENUM, EXPLICIT, EXPORT, EXTENDS, EXTERNAL, 
    FILTER, FINAL, FINALLY, FRIEND, FROM, FXGROUP, GET, GOTO, GROUPSHARED,
    HIGH, IMPLEMENTS, IMPORT, INLINE, INSTANCEOF, INTERFACE, LAYOUT, LOW,
    MACRO, MATCH, MEDIUM, MODULE, MUTABLE, NAMESPACE, NODISCARD, NOEXCEPT,
    NOINLINE, NOINTERPOLATION, NON_COHERENT, NONCOHERENT, NOPERSPECTIVE, NULLLC,
    NULLPTR, OF, _OPERATOR, PACKAGE, PACKOFFSET, PARTITION, PASS, PATCH,
    PIXELFRAGMENT, PRECISE, PRECISION, PREMERGE, PRIV, PROTECTED, PUB, PUBLIC,
    READONLY, REF, REGARDLESS, REGISTER, REINTERPRET_CAST, REQUIRE, RESOURCE,
    RESTRICT, SET, SHARED, SIZEOF, SMOOTH, SNORM, STATIC, 
    STATIC_CAST, STD, SUBROUTINE, SUPER, TARGET, TEMPLATE, THIS, THREAD_LOCAL,
    THROW, TRAIT, TRY, TYPEDEF, TYPEID, TYPENAME, TYPEOF, UNION, UNLESS,
    UNORM, UNSAFE, UNSIZED, USE, USING, VARYING, VIRTUAL, VOLATILE, WGSL, WHERE,
    WITH, WRITEONLY, YIELD, LINEAR, FLAT, PERSPECTIVE,

    // Other Types
    PTR, SAMPLER,
    SAMPLER_COMPARISON, TEXTURE_1D, TEXTURE_2D, TEXTURE_2D_ARRAY, TEXTURE_3D,
    TEXTURE_CUBE, TEXTURE_CUBE_ARRAY, TEXTURE_MULTISAMPLED_2D, TEXTURE_STORAGE_1D,
    TEXTURE_STORAGE_2D, TEXTURE_STORAGE_2D_ARRAY, TEXTURE_STORAGE_3D, TEXTURE_DEPTH_2D,
    TEXTURE_DEPTH_2D_ARRAY, TEXTURE_DEPTH_CUBE, TEXTURE_DEPTH_CUBE_ARRAY,
    TEXTURE_DEPTH_MULTISAMPLED_2D,
    // Keywords
    BREAK, CASE, CONTINUE, CONTINUING, DEFAULT, DISCARD, ELSE, ENABLE, FALLTHROUGH,
    FALSE, FN, FOR, FUNCTION, LOOP, OVERRIDE, RETURN,
    STRUCT, SWITCH, TRUE, TYPE, WHILE,
    STATIC_ASSERT, DIAGNOSTIC_CONTROL, REQUIRES,
    // Built-in functions
    ABS, ACOS, ACOSH, ALL, ANY, ARRAY_LENGTH, ASIN, ASINH, ATAN, ATAN2,
    ATANH, ATOMIC_ADD, ATOMIC_AND, ATOMIC_COMPARE_EXCHANGE_WEAK, ATOMIC_EXCHANGE,
    ATOMIC_LOAD, ATOMIC_MAX, ATOMIC_MIN, ATOMIC_OR, ATOMIC_STORE, ATOMIC_SUB,
    ATOMIC_XOR, CEIL, COS, COSH, COUNT_LEADING_ZEROS, COUNT_ONE_BITS,
    COUNT_TRAILING_ZEROS, CROSS, DEGREES, DETERMINANT, DISTANCE, EXP,
    EXP2, EXTRACT_BITS, FACE_FORWARD, FIRST_LEADING_BIT, FIRST_TRAILING_BIT, FLOOR,
    FRAC, FREXP, INSERT_BITS, LDEXP, LENGTH, LOG,
    LOG2, MAX, MIN, MIX, MODF, NORMALIZE, POW, QUANTIZE_TO_F16, RADIANS, REFLECT,
    REFRACT, REVERSE_BITS, ROUND, SATURATE, SIGN, SIN, SINH, SMOOTHSTEP, SQRT,
    STEP, TAN, TANH, TRANSPOSE, TRUNC, UNPACK2X16_FLOAT, UNPACK2X16_SNORM,
    UNPACK2X16_UNORM, UNPACK4X8_SNORM, UNPACK4X8_UNORM, UNPACK4X_I8, UNPACK4X_U8,
    PACK2X16_FLOAT, PACK2X16_SNORM,
    PACK2X16_UNORM, PACK4X8_SNORM, PACK4X8_UNORM, PACK4X_I8, PACK4X_U8,
    WORKGROUP_BARRIER,
    // Access modes
    READ, WRITE, READ_WRITE,
    // Address spaces
    PRIVATE, WORKGROUP, UNIFORM, STORAGE,
    // Builtin values
    VERTEX_INDEX, INSTANCE_INDEX, POSITION, FRONT_FACING, FRAG_DEPTH,
    LOCAL_INVOCATION_ID, LOCAL_INVOCATION_INDEX, GLOBAL_INVOCATION_ID,
    WORKGROUP_ID, NUM_WORKGROUPS, SAMPLE_INDEX, SAMPLE_MASK,
    // Attributes
    ALIAS, ALIGN, BINDING, BUILTIN, COMPUTE, CONST, DIAGNOSTIC, FRAGMENT,
    GROUP, ID, INTERPOLATE, INVARIANT, LOCATION, MUST_USE, _SIZE,
    VERTEX, WORKGROUP_SIZE,
    // Texture formats
    R32UINT, R32SINT, R32FLOAT,
    RGBA8UNORM, RGBA8SNORM, RGBA8UINT, BGRA8UNORM,
    RGBA8SINT, RG32UINT, RG32SINT, RG32FLOAT, RGBA16UINT,
    RGBA16SINT, RGBA16FLOAT, RGBA32UINT, RGBA32SINT, RGBA32FLOAT,
    // Texture methods
    TEXTURE_STORE, TEXTURE_LOAD, TEXTURE_SIZE, TEXTURE_DIMENSIONS, TEXTURE_NUM_LAYERS,
    TEXTURE_NUM_LEVELS, TEXTURE_NUM_SAMPLES, TEXTURE_SAMPLE, TEXTURE_SAMPLE_BIAS,
    TEXTURE_SAMPLE_COMPARE, TEXTURE_SAMPLE_COMPARE_LEVEL, TEXTURE_SAMPLE_GRAD, TEXTURE_SAMPLE_LEVEL,
    TEXTURE_GATHER, TEXTURE_GATHER_COMPARE, TEXTURE_GATHER_BIAS, TEXTURE_GATHER_BIAS_COMPARE,
    TEXTURE_GATHER_COMPARE_LEVEL, TEXTURE_GATHER_LEVEL,
    // Special identifiers that shouldn't be renamed
    MAIN, DERIVATIVE_UNIFORMITY, OFF
};
uint8_t encode_swizzle(const std::string_view text, bool is_four_component) {
    static constexpr uint8_t lookup[26] = {
        0xFF, 0x2, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2, 0x3, 0x3, 0x0, 0x1,
        0xFF, 0xFF, 0x3, 0x0, 0x1, 0x2
    };

    uint8_t result = 0;
    size_t length = text.length();

    if (is_four_component) {
        if (length != 4) return 0;  // Error: must be exactly 4 components
        for (size_t i = 0; i < 4; i++) {
            uint8_t val = lookup[text[i] - 'a'];
            if (val == 0xFF) return 0;
            result |= val << (i * 2);
        }
    }
    else {
        if (length == 0 || length > 3) return 0;  // Error: must be 1-3 components
        // Store length in top 2 bits (1-3)
        result = (uint8_t)(length << 6);
        // Store components in remaining bits
        for (size_t i = 0; i < length; i++) {
            uint8_t val = lookup[text[i] - 'a'];
            if (val == 0xFF) return 0;
            result |= val << (i * 2);
        }
    }

    return result;
}

std::string decode_swizzle(uint8_t encoded, bool is_four_component) {
    static constexpr char components[] = "xyzw";
    std::string result;

    if (is_four_component) {
        for (int i = 0; i < 4; i++) {
            result += components[(encoded >> (i * 2)) & 0x3];
        }
    }
    else {
        // Length is stored in top 2 bits
        size_t length = (encoded >> 6) & 0x3;
        if (length == 0 || length > 3) return "";  // Invalid length

        for (size_t i = 0; i < length; i++) {
            result += components[(encoded >> (i * 2)) & 0x3];
        }
    }

    return result;
}

uint32_t data_size = 0;
uint32_t data_size2 = 0;
void AppendVar(uint8_t*& data, uint16_t value) {
    // If value fits in 7 bits, use single byte
    if (value < 0x80) {
        data[0] = value;
        data += 1;
        data_size += 1;
        return;
    }

    // Otherwise use two bytes, with high bit set on first byte
    data[0] = 0x80 | (value & 0x7F);         // Low 7 bits, with marker
    data[1] = (value >> 7) & 0xFF;           // High bits
    data += 2;
    data_size += 2;
}

uint16_t GetVar(const uint8_t* data, size_t& offset) {
    uint8_t first = data[offset];

    // Single byte format
    if ((first & 0x80) == 0) {
        offset += 1;
        return first;
    }

    // Two byte format
    uint16_t value = (first & 0x7F) | ((uint16_t)data[offset + 1] << 7);
    offset += 2;
    return value;
}
void AppendToken(uint8_t*& data, WGSLTokenTypes type) {
    AppendVar(data, type);
}
WGSLTokenTypes GetToken(const uint8_t* data, size_t& offset) {
    return (WGSLTokenTypes)GetVar(data, offset);
}
void AppendReservedToken(uint8_t*& data, WGSLTokenTypes type) {
    AppendToken(data, type);
}
void AppendIdentifierToken(uint8_t*& data, uint16_t var) {
    AppendToken(data, WGSLTokenTypes::IDENTIFIER);
    AppendVar(data, var);
}
void AppendSwizzleToken(uint8_t*& data, uint8_t swizzle) {
    AppendToken(data, WGSLTokenTypes::SWIZZLE);
    data[0] = swizzle;
    data += 1;
    data_size += 1;
}
void AppendShortSwizzleToken(uint8_t*& data, uint8_t swizzle) {
    AppendToken(data, WGSLTokenTypes::SHORTSWIZZLE);
    data[0] = swizzle;
    data += 1;
    data_size += 1;
}
template<typename TYPE>
void AppendNumber(uint8_t*& data, TYPE f) {
    AppendToken(data, (sizeof(TYPE) == 4) ? NUMBER4 : NUMBER8);
    *(TYPE*)data = f;
    data += sizeof(TYPE);
    data_size += sizeof(TYPE);
}
template<typename TYPE>
void AppendUnsignedNumber(uint8_t*& data, TYPE f) {
    AppendToken(data, (sizeof(TYPE) == 4) ? UNSIGNED_NUMBER4 : UNSIGNED_NUMBER8);
    *(TYPE*)data = f;
    data += sizeof(TYPE);
    data_size += sizeof(TYPE);
}
template<typename TYPE>
void AppendFloat(uint8_t*& data, TYPE f) {
    AppendToken(data, (sizeof(TYPE) == 4) ? FLOAT4 : FLOAT8);
    *(TYPE*)data = f;
    data += sizeof(TYPE);
    data_size += sizeof(TYPE);
}
void AppendChar(uint8_t*& data, const char c) {
    AppendToken(data, WGSLTokenTypes::_CHAR);
    *data = c;
    data++;
    data_size++;
}
void AppendText(WGSLTokenTypes type, uint8_t*& data, const std::string& str) {
    AppendToken(data, type);
    *data = (uint8_t)str.size();
    data += 1;
    memcpy(data, str.c_str(), str.size());
    data += str.size();
    data_size += 1 + (uint32_t)str.size();
}
// Reserved words, a multimap
static std::unordered_map<std::string, WGSLTokenTypes> g_reservedWords;
static std::array<std::string, OFF+1> g_reservedWordsReverse;

void AddReservedWord(const std::string& s, WGSLTokenTypes type) {
    g_reservedWords[s] = type;
    g_reservedWordsReverse[(uint16_t)type] = s;
}

void InitReservedWords() {
    for (auto token : std::initializer_list<std::pair<std::string, WGSLTokenTypes>>{
        // Operators
        {"(", LEFT_PARAM}, {")", RIGHT_PARAM}, {"[", LEFT_BRACKET}, {"]", RIGHT_BRACKET},
        {"<", LEFT_ANGLE}, {">", RIGHT_ANGLE}, {";", SEMICOLON}, {",", COMMA}, {".", PERIOD},
        {"=", ASSIGN}, {"-", MINUS}, {"+", PLUS}, {"*", MULTIPLY}, {"/", DIVIDE}, {"!", NOT},
        {"||", OR}, {"&&", AND}, {"^", XOR}, {"~", TILDE}, {"%", MODULO}, {"?", QUESTION},
        {":", COLON}, {"@", AT}, {"#", HASH}, {"$", DOLLAR}, {"!", BANG}, {"|", PIPE},
        {"&", AMPERSAND}, {"^", CARET}, {"%", PERCENT}, {"{", LEFT_CURLY}, {"}", RIGHT_CURLY},
        // Additional
        {"NULL", NULLUC},
        {"self", SELF},
        {"abstract", ABSTRACT},
        {"active", ACTIVE},
        {"alignas", ALIGNAS},
        {"alignof", ALIGNOF},
        {"as", AS},
        {"asm", ASM},
        {"asm_fragment", ASM_FRAGMENT},
        {"async", ASYNC},
        {"attribute", ATTRIBUTE},
        {"auto", AUTO},
        {"await", AWAIT},
        {"become", BECOME},
        {"binding_array", BINDING_ARRAY},
        {"cast", CAST},
        {"catch", CATCH},
        {"class", CLASS},
        {"co_await", CO_AWAIT},
        {"co_return", CO_RETURN},
        {"co_yield", CO_YIELD},
        {"coherent", COHERENT},
        {"column_major", COLUMN_MAJOR},
        {"common", COMMON},
        {"compile", COMPILE},
        {"compile_fragment", COMPILE_FRAGMENT},
        {"concept", CONCEPT},
        {"const_cast", CONST_CAST},
        {"consteval", CONSTEVAL},
        {"constexpr", CONSTEXPR},
        {"constinit", CONSTINIT},
        {"crate", CRATE},
        {"debugger", DEBUGGER},
        {"decltype", DECLTYPE},
        {"delete", DELETE},
        {"demote", DEMOTE},
        {"demote_to_helper", DEMOTE_TO_HELPER},
        {"do", DO},
        {"dynamic_cast", DYNAMIC_CAST},
        {"enum", ENUM},
        {"explicit", EXPLICIT},
        {"export", EXPORT},
        {"extends", EXTENDS},
        {"external", EXTERNAL},
        {"filter", FILTER},
        {"final", FINAL},
        {"finally", FINALLY},
        {"friend", FRIEND},
        {"from", FROM},
        {"fxgroup", FXGROUP},
        {"get", GET},
        {"goto", GOTO},
        {"groupshared", GROUPSHARED},
        {"high", HIGH},
        {"implements", IMPLEMENTS},
        {"import", IMPORT},
        {"inline", INLINE},
        {"instanceof", INSTANCEOF},
        {"interface", INTERFACE},
        {"layout", LAYOUT},
        {"low", LOW},
        {"macro", MACRO},
        {"match", MATCH},
        {"medium", MEDIUM},
        {"module", MODULE},
        {"mutable", MUTABLE},
        {"namespace", NAMESPACE},
        {"nodiscard", NODISCARD},
        {"noexcept", NOEXCEPT},
        {"noinline", NOINLINE},
        {"nointerpolation", NOINTERPOLATION},
        {"non_coherent", NON_COHERENT},
        {"noncoherent", NONCOHERENT},
        {"noperspective", NOPERSPECTIVE},    
        {"nulllc", NULLLC},
        {"nullptr", NULLPTR},
        {"of", OF},
        {"operator", _OPERATOR},
        {"package", PACKAGE},
        {"packoffset", PACKOFFSET},
        {"partition", PARTITION},
        {"pass", PASS},
        {"patch", PATCH},
        {"pixelfragment", PIXELFRAGMENT},
        {"precise", PRECISE},
        {"precision", PRECISION},
        {"premerge", PREMERGE},
        {"priv", PRIV},
        {"protected", PROTECTED},
        {"pub", PUB},
        {"public", PUBLIC},
        {"readonly", READONLY},
        {"ref", REF},
        {"regardless", REGARDLESS},
        {"register", REGISTER},
        {"reinterpret_cast", REINTERPRET_CAST},
        {"require", REQUIRE},
        {"resource", RESOURCE},
        {"restrict", RESTRICT},
        {"set", SET},
        {"shared", SHARED},
        {"sizeof", SIZEOF},
        {"smooth", SMOOTH},
        {"snorm", SNORM},
        {"static", STATIC},
        {"static_cast", STATIC_CAST},
        {"std", STD},
        {"subroutine", SUBROUTINE},
        {"super", SUPER},
        {"target", TARGET},
        {"template", TEMPLATE},
        {"this", THIS},
        {"thread_local", THREAD_LOCAL},
        {"throw", THROW},
        {"trait", TRAIT},
        {"try", TRY},
        {"typedef", TYPEDEF},
        {"typeid", TYPEID},
        {"typename", TYPENAME},
        {"typeof", TYPEOF},
        {"union", UNION},
        {"unless", UNLESS},
        {"unorm", UNORM},
        {"unsafe", UNSAFE},
        {"unsized", UNSIZED},
        {"use", USE},
        {"using", USING},
        {"varying", VARYING},
        {"virtual", VIRTUAL},
        {"volatile", VOLATILE},
        {"wgsl", WGSL},
        {"where", WHERE},
        {"with", WITH},
        {"writeonly", WRITEONLY},
        {"yield", YIELD},
        {"linear", LINEAR},
        {"flat", FLAT},
        {"perspective", PERSPECTIVE},
        // Types
        {"bool", _BOOL},
        {"f32", F32},
        {"f16", F16},
        {"i32", I32},
        {"u32", _U32},
        {"array", ARRAY},
        {"atomic", ATOMIC},
        // Matrix Types
        {"mat2x2f", MAT2X2F},
        {"mat2x3f", MAT2X3F},
        {"mat2x4f", MAT2X4F},
        {"mat3x2f", MAT3X2F},
        {"mat3x3f", MAT3X3F},
        {"mat3x4f", MAT3X4F},
        {"mat4x2f", MAT4X2F},
        {"mat4x3f", MAT4X3F},
        {"mat4x4f", MAT4X4F},
        {"mat2x2h", MAT2X2H},
        {"mat2x3h", MAT2X3H},
        {"mat2x4h", MAT2X4H},
        {"mat3x2h", MAT3X2H},
        {"mat3x3h", MAT3X3H},
        {"mat3x4h", MAT3X4H},
        {"mat4x2h", MAT4X2H},
        {"mat4x3h", MAT4X3H},
        {"mat4x4h", MAT4X4H},
        // Other Types
        {"ptr", PTR},
        {"sampler", SAMPLER},
        {"sampler_comparison", SAMPLER_COMPARISON},
        {"texture_1d", TEXTURE_1D},
        {"texture_2d", TEXTURE_2D},
        {"texture_2d_array", TEXTURE_2D_ARRAY},
        {"texture_3d", TEXTURE_3D},
        {"texture_cube", TEXTURE_CUBE},
        {"texture_cube_array", TEXTURE_CUBE_ARRAY},
        {"texture_multisampled_2d", TEXTURE_MULTISAMPLED_2D},
        {"texture_storage_1d", TEXTURE_STORAGE_1D},
        {"texture_storage_2d", TEXTURE_STORAGE_2D},
        {"texture_storage_2d_array", TEXTURE_STORAGE_2D_ARRAY},
        {"texture_storage_3d", TEXTURE_STORAGE_3D},
        {"texture_depth_2d", TEXTURE_DEPTH_2D},
        {"texture_depth_2d_array", TEXTURE_DEPTH_2D_ARRAY},
        {"texture_depth_cube", TEXTURE_DEPTH_CUBE},
        {"texture_depth_cube_array", TEXTURE_DEPTH_CUBE_ARRAY},
        {"texture_depth_multisampled_2d", TEXTURE_DEPTH_MULTISAMPLED_2D},
        {"vec2", VEC2},
        {"vec3", VEC3},
        {"vec4", VEC4},
        {"vec2i", VEC2I},
        {"vec3i", VEC3I},
        {"vec4i", VEC4I},
        {"vec2u", VEC2U},
        {"vec3u", VEC3U},
        {"vec4u", VEC4U},
        {"vec2f", VEC2F},
        {"vec3f", VEC3F},
        {"vec4f", VEC4F},
        {"vec2h", VEC2H},
        {"vec3h", VEC3H},
        {"vec4h", VEC4H},
        // Keywords
        {"break", BREAK},
        {"case", CASE},
        {"continue", CONTINUE},
        {"continuing", CONTINUING},
        {"default", DEFAULT},
        {"discard", DISCARD},
        {"else", ELSE},
        {"enable", ENABLE},
        {"fallthrough", FALLTHROUGH},
        {"false", FALSE},
        {"fn", FN},
        {"for", FOR},
        {"function", FUNCTION},
        {"if", IF},
        {"let", LET},
        {"loop", LOOP},
        {"override", OVERRIDE},
        {"return", RETURN},
        {"struct", STRUCT},
        {"switch", SWITCH},
        {"true", TRUE},
        {"type", TYPE},
        {"var", VAR},
        {"while", WHILE},
        {"bitcast", BITCAST},
        {"and", AND},
        {"or", OR},
        {"xor", XOR},
        {"not", NOT},
        {"static_assert", STATIC_ASSERT},
        {"diagnostic_control", DIAGNOSTIC_CONTROL},
        {"requires", REQUIRES},
        // Built-in functions
        {"abs", ABS},
        {"acos", ACOS},
        {"acosh", ACOSH},
        {"all", ALL},
        {"any", ANY},
        {"array_length", ARRAY_LENGTH},
        {"asin", ASIN},
        {"asinh", ASINH},
        {"atan", ATAN},
        {"atan2", ATAN2},
        {"atanh", ATANH},
        {"atomicAdd", ATOMIC_ADD},
        {"atomicAnd", ATOMIC_AND},
        {"atomicCompare_exchange_weak", ATOMIC_COMPARE_EXCHANGE_WEAK},
        {"atomicExchange", ATOMIC_EXCHANGE},
        {"atomicLoad", ATOMIC_LOAD},
        {"atomicMax", ATOMIC_MAX},
        {"atomicMin", ATOMIC_MIN},
        {"atomicOr", ATOMIC_OR},
        {"atomicStore", ATOMIC_STORE},
        {"atomicSub", ATOMIC_SUB},
        {"atomicXor", ATOMIC_XOR},
        {"ceil", CEIL},
        {"clamp", CLAMP},
        {"cos", COS},
        {"cosh", COSH},
        {"countLeadingZeros", COUNT_LEADING_ZEROS},
        {"countOneBits", COUNT_ONE_BITS},
        {"countTrailingZeros", COUNT_TRAILING_ZEROS},
        {"cross", CROSS},
        {"degrees", DEGREES},
        {"determinant", DETERMINANT},
        {"distance", DISTANCE},
        {"dot", DOT},
        {"exp", EXP},
        {"exp2", EXP2},
        {"extractBits", EXTRACT_BITS},
        {"faceForward", FACE_FORWARD},
        {"firstLeadingBit", FIRST_LEADING_BIT},
        {"firstTrailingBit", FIRST_TRAILING_BIT},
        {"floor", FLOOR},
        {"fma", FMA},
        {"fract", FRAC},
        {"frexp", FREXP},
        {"insertBits", INSERT_BITS},
        {"inverseSqrt", INVERSE_SQRT},
        {"ldexp", LDEXP},
        {"length", LENGTH},
        {"log", LOG},
        {"log2", LOG2},
        {"max", MAX},
        {"min", MIN},
        {"mix", MIX},
        {"modf", MODF},
        {"normalize", NORMALIZE},
        {"pow", POW},
        {"quantizeToF16", QUANTIZE_TO_F16},
        {"radians", RADIANS},
        {"reflect", REFLECT},
        {"refract", REFRACT},
        {"reverseBits", REVERSE_BITS},
        {"round", ROUND},
        {"saturate", SATURATE},
        {"select", SELECT},
        {"sign", SIGN},
        {"sin", SIN},
        {"sinh", SINH},
        {"smoothstep", SMOOTHSTEP},
        {"sqrt", SQRT},
        {"step", STEP},
        {"tan", TAN},
        {"tanh", TANH},
        {"transpose", TRANSPOSE},
        {"trunc", TRUNC},
        {"pack2x16float", PACK2X16_FLOAT},
		{"pack2x16snorm", PACK2X16_SNORM},
		{"pack2x16unorm", PACK2X16_UNORM},
		{"pack4x8snorm", PACK4X8_SNORM},
		{"pack4x8unorm", PACK4X8_UNORM},
		{"pack4xI8", PACK4X_I8},
		{"pack4xU8", PACK4X_U8},
        {"unpack2x16float", UNPACK2X16_FLOAT},
        {"unpack2x16snorm", UNPACK2X16_SNORM},
        {"unpack2x16unorm", UNPACK2X16_UNORM},
        {"unpack4x8snorm", UNPACK4X8_SNORM},
        {"unpack4x8unorm", UNPACK4X8_UNORM},
        {"unpack4xI8", UNPACK4X_I8},
        {"unpack4xU8", UNPACK4X_U8},
        {"workgroupBarrier", WORKGROUP_BARRIER},
        // Access modes
        {"read", READ},
        {"write", WRITE},
        {"read_write", READ_WRITE},
        // Address spaces
        {"private", PRIVATE},
        {"workgroup", WORKGROUP},
        {"uniform", UNIFORM},
        {"storage", STORAGE},
        // Builtin values
        {"vertex_index", VERTEX_INDEX},
        {"instance_index", INSTANCE_INDEX},
        {"position", POSITION},
        {"front_facing", FRONT_FACING},
        {"frag_depth", FRAG_DEPTH},
        {"local_invocation_id", LOCAL_INVOCATION_ID},
        {"local_invocation_index", LOCAL_INVOCATION_INDEX},
        {"global_invocation_id", GLOBAL_INVOCATION_ID},
        {"workgroup_id", WORKGROUP_ID},
        {"num_workgroups", NUM_WORKGROUPS},
        {"sample_index", SAMPLE_INDEX},
        {"sample_mask", SAMPLE_MASK},
        // Attributes
        {"alias", ALIAS},
        {"align", ALIGN},
        {"binding", BINDING},
        {"builtin", BUILTIN},
        {"compute", COMPUTE},
        {"const", CONST},
        {"diagnostic", DIAGNOSTIC},
        {"fragment", FRAGMENT},
        {"group", GROUP},
        {"id", ID},
        {"interpolate", INTERPOLATE},
        {"invariant", INVARIANT},
        {"location", LOCATION},
        {"must_use", MUST_USE},
        {"size", _SIZE},
        {"vertex", VERTEX},
        {"workgroup_size", WORKGROUP_SIZE},
        // Texture formats
        {"r32uint", R32UINT},
        {"r32sint", R32SINT},
        {"r32float", R32FLOAT},
        {"rgba8unorm", RGBA8UNORM},
        {"rgba8snorm", RGBA8SNORM},
        {"rgba8uint", RGBA8UINT},
        {"bgra8unorm", BGRA8UNORM},
        {"rgba8sint", RGBA8SINT},
        {"rg32uint", RG32UINT},
        {"rg32sint", RG32SINT},
        {"rg32float", RG32FLOAT},
        {"rgba16uint", RGBA16UINT},
        {"rgba16sint", RGBA16SINT},
        {"rgba16float", RGBA16FLOAT},
        {"rgba32uint", RGBA32UINT},
        {"rgba32sint", RGBA32SINT},
        {"rgba32float", RGBA32FLOAT},
        // Texture methods
        {"textureStore", TEXTURE_STORE},
        {"textureLoad", TEXTURE_LOAD},
        {"textureSize", TEXTURE_SIZE},
        {"textureDimensions", TEXTURE_DIMENSIONS},
        {"textureNumLayers", TEXTURE_NUM_LAYERS},
        {"textureNumLevels", TEXTURE_NUM_LEVELS},
        {"textureNumSamples", TEXTURE_NUM_SAMPLES},
        {"textureSample", TEXTURE_SAMPLE},
        {"textureSampleBias", TEXTURE_SAMPLE_BIAS},
        {"textureSampleCompare", TEXTURE_SAMPLE_COMPARE},
        {"textureSampleCompareLevel", TEXTURE_SAMPLE_COMPARE_LEVEL},
        {"textureSampleGrad", TEXTURE_SAMPLE_GRAD},
        {"textureSampleLevel", TEXTURE_SAMPLE_LEVEL},
        {"textureGather", TEXTURE_GATHER},
        {"textureGatherCompare", TEXTURE_GATHER_COMPARE},
        {"textureGatherBias", TEXTURE_GATHER_BIAS},
        {"textureGatherBiasCompare", TEXTURE_GATHER_BIAS_COMPARE},
        {"textureGatherCompareLevel", TEXTURE_GATHER_COMPARE_LEVEL},
        {"textureGatherLevel", TEXTURE_GATHER_LEVEL},
        // Special identifiers that shouldn't be renamed
        {"main", MAIN},
        {"derivative_uniformity", DERIVATIVE_UNIFORMITY},
        {"off", OFF},
    }) {
        AddReservedWord(token.first, token.second);
    }
}

WGSLTokenTypes isReservedWord(const std::string_view s) {
    auto it = g_reservedWords.find(s.data());
    if (it != g_reservedWords.end()) {
        return it->second; // return the mapped token type
    }
    return (WGSLTokenTypes)-1;
}
std::string getReservedWord(WGSLTokenTypes type) {
    return g_reservedWordsReverse[type];
}

// A number literal could be something like: 123, 456u, -78, 3.14f, etc.
bool isNumberLiteral(const std::string_view s) {
    // A simple heuristic:
    // - Optional leading '-'
    // - Then digits
    // - Possibly '.' or 'f' or 'u'
    // This is a simplified check. wndr-todo: make sure this follows all wgsl rules.
    size_t start = 0;
    if (!s.empty() && s[0] == '-') {
        start = 1;
    }
    bool hasDigits = false;
    for (size_t i = start; i < s.size(); i++) {
        char c = s[i];
        if (std::isdigit((unsigned char)c)) {
            hasDigits = true;
            continue;
        }
        // Allowed chars in a number literal: '.', 'f', 'u'
        // If something else appears, break
        if (c != '.' && c != 'f' && c != 'u') {
            return false;
        }
    }
    return hasDigits;
}

bool isFloatLiteral(const std::string& s) {
    // A float literal if it contains '.' or ends with 'f'
    // Example: "3.14", "0.5f"
    return (s.find('.') != std::string::npos) || (!s.empty() && s.back() == 'f');
}

bool isUnsignedLiteral(const std::string& s) {
    // Unsigned literal if it ends with 'u'
    // Example: "123u"
    return (!s.empty() && s.back() == 'u');
}

float parseFloat(const std::string& s) {
    return std::stof(s);
}

uint32_t parseUnsigned(const std::string& s) {
    // strip trailing 'u' if present
    std::string temp = s;
    if (!temp.empty() && temp.back() == 'u')
        temp.pop_back();
    return (uint32_t)std::stoul(temp);
}

int32_t parseInt(const std::string& s) {
    // could handle 'u' or 'f' but we assume isNumberLiteral logic prevents that for pure ints
    return std::stoi(s);
}
void Tokenize(uint8_t*& data, uint32_t& size, std::string_view wgsl) {
    std::string current;

    for (size_t i = 0; i < wgsl.length(); i++) {
        char c = wgsl[i];
        
        // Handle identifiers/keywords
        if (std::isalpha(c) || c == '_') {
            current = c;
            while (++i < wgsl.length() && (std::isalnum(wgsl[i]) || wgsl[i] == '_')) {
                current += wgsl[i];
            }
            i--;
            WGSLTokenTypes reservedType = isReservedWord(current);
            if (reservedType != (WGSLTokenTypes)-1) {
                // Reserved token
                AppendReservedToken(data, reservedType);
            }
            else if (isNumberLiteral(current)) {
                // Parse as number
                if (isFloatLiteral(current)) {
                    float f = parseFloat(current);
                    AppendFloat(data, f);
                }
                else if (isUnsignedLiteral(current)) {
                    uint32_t u = parseUnsigned(current);
                    AppendUnsignedNumber(data, u);
                }
                else {
                    int32_t val = parseInt(current);
                    AppendNumber(data, val);
                }
            }
            else {
                AppendText(IDENTIFIER, data, current);
            }
            continue;
        }

        // Handle numbers
        if (std::isdigit(c) || (c == '-' && i + 1 < wgsl.length() && std::isdigit(wgsl[i + 1]))) {
            current = c;
            while (++i < wgsl.length() && (std::isdigit(wgsl[i]) || wgsl[i] == '.' || 
                   wgsl[i] == 'f' || wgsl[i] == 'u')) {
                current += wgsl[i];
            }
            i--;
            if (isFloatLiteral(current)) {
                float f = parseFloat(current);
                AppendFloat(data, f);
            }
            else if (isUnsignedLiteral(current)) {
                uint32_t u = parseUnsigned(current);
                AppendUnsignedNumber(data, u);
            }
            else {
                int32_t val = parseInt(current);
                AppendNumber(data, val);
            }
            continue;
        }

        // Handle newlines
        if (c == '\n') {
            AppendReservedToken(data, NEWLINE);
            continue;
        }

        current = c;
        WGSLTokenTypes reservedType = isReservedWord(current);
        if (reservedType != (WGSLTokenTypes)-1) {
            // Reserved token
            AppendReservedToken(data, reservedType);
            continue;
        }
    }
}
static uint8_t ReadU8(const std::vector<uint8_t>& d, size_t& offset) {
    uint8_t len;
    memcpy(&len, &d[offset], 1);
    offset += 1;
    return len;
}
// Helper to read a 4-byte number as float or int
static uint32_t ReadU32(const std::vector<uint8_t>& d, size_t& offset) {
    uint32_t v;
    memcpy(&v, &d[offset], 4);
    offset += 4;
    return v;
}

// Helper to read an 8-byte number
static uint64_t ReadU64(const std::vector<uint8_t>& d, size_t& offset) {
    uint64_t v;
    memcpy(&v, &d[offset], 8);
    offset += 8;
    return v;
}
std::string ToWGSL(const std::vector<uint8_t>& d) {
    std::string wgsl;
    size_t offset = 0;
    int indentLevel = 0;
    int prevToken = -1;

    auto addIndent = [&]() {
        for (int i = 0; i < indentLevel; i++) wgsl += "  ";
        };

    while (offset < d.size()) {
        WGSLTokenTypes token = (WGSLTokenTypes)GetToken(d.data(), offset);
        std::string text;

        // Decide how to read based on token type
        if (token == NEWLINE) {
            wgsl += "\n";
            if (offset < d.size()) addIndent();
            prevToken = token;
            continue;
        }
        else if (token == NUMBER4) {
            int32_t val = ReadU32(d, offset);
            text = std::to_string(val);
        }
        else if (token == NUMBER8) {
            uint64_t val = ReadU64(d, offset);
            text = std::to_string(val);
        }
        else if (token == UNSIGNED_NUMBER4) {
            uint32_t val = ReadU32(d, offset);
            text = std::to_string(val) + "u";
        }
        else if (token == UNSIGNED_NUMBER8) {
            uint64_t val = ReadU64(d, offset);
            text = std::to_string(val) + "u";
        }
        else if (token == FLOAT4) {
            uint32_t val = ReadU32(d, offset);
            float db;
            memcpy(&db, &val, 4);
            text = std::to_string(db);
            while (text.back() == '0') {
                text.pop_back();
            }
        }
        else if (token == FLOAT8) {
            uint64_t val = ReadU64(d, offset);
            double db;
            memcpy(&db, &val, 8);
            text = std::to_string(db);
            while (text.back() == '0') {
                text.pop_back();
            }
        }
        else if (token == IDENTIFIER) {
            uint16_t val = GetVar(d.data(), offset);
            std::string var = "v" + std::to_string(val);
            text += var;
        }
        else if (token == SWIZZLE) {
            uint8_t val = *(d.data() + offset++);
            std::string swizzle = decode_swizzle(val, true);
            text += swizzle;
        }
        else if (token == SHORTSWIZZLE) {
            uint8_t val = *(d.data() + offset++);
            std::string swizzle = decode_swizzle(val, false);
            text += swizzle;
        }
        else if (token == _CHAR) {
            uint8_t val = *(d.data() + offset++);
            text += val;
        }
        else {
            text = getReservedWord(token);
        }

        // Spacing logic
        WGSLTokenTypes currType = (WGSLTokenTypes)token;
        WGSLTokenTypes prevTypeEnum = (prevToken >= 0) ? (WGSLTokenTypes)prevToken : NEWLINE;

        wgsl += text;

        switch (token) {
        case RETURN:
        case STRUCT:
        case ALIAS:
        case CONST:
        case VAR:
        case LET:
        case FN:
            wgsl += " ";
            break;
        default:
            // Reserved or unknown token that had no extra data
            // Just print whatever text we got from getReservedWord
            break;
        }
        prevToken = token;
    }

    return wgsl;
}
struct WGSLPreprocessor {
    std::string result;
    std::unordered_map<std::string, uint16_t> renamedVars;
    std::unordered_map<uint16_t, std::string> renamedVarsReverse;
    uint16_t nextVarId = 0;
    uint16_t getNameId() {
        return nextVarId++;
    }
    std::string_view getOriginalName(const std::string_view name) {
        auto varId = (uint16_t)std::stoi(name.substr(1).data());
        return renamedVarsReverse[varId];
    }
    bool is_swizzle(const std::string_view name) {
        if (name.empty()) return false;
        switch(name[0]) {
            case 'x': case 'y': case 'z': case 'w': return name.find_first_not_of("xyzw") == name.npos;
            case 'r': case 'g': case 'b': case 'a': return name.find_first_not_of("rgba") == name.npos;
            case 's': case 't': case 'p': case 'q': return name.find_first_not_of("stpq") == name.npos;
            default: return false;
        }
    }
    bool canMinify(const std::string_view name) {
        return name.size() > 1;
    }

    std::vector<uint8_t> preprocess(const std::vector<uint8_t>& originalData) {
        std::vector<uint8_t> newData(originalData.size());
        uint8_t* data = newData.data();

        // First pass: collect names
        {
            size_t offset = 0;
            WGSLTokenTypes last_type = (WGSLTokenTypes)-1;
            while (offset < originalData.size()) {
                WGSLTokenTypes type = GetToken(originalData.data(), offset);

                if (type == NEWLINE) {
                    // No data after type
                }
                else if (type == NUMBER4 || type == FLOAT4) {
                    offset += 4; // skip number data
                }
                else if (type == NUMBER8 || type == FLOAT8) {
                    offset += 8; // skip number data
                }
                else if (type == IDENTIFIER) {
                    uint8_t len;
                    memcpy(&len, &originalData[offset], 1);
                    offset += 1;
                    std::string text((const char*)&originalData[offset], len);
                    offset += len;
                    if (last_type == WGSLTokenTypes::PERIOD && is_swizzle(text)) {

                    }
                    else if (canMinify(text)) {
                        if (renamedVars.find(text) == renamedVars.end()) {
                            renamedVars[text] = getNameId();
                            renamedVarsReverse[renamedVars[text]] = text;
                        }
                    }
                }
                else {
                    // Reserved token or something with no data
                    // no length to read
                }
                last_type = type;
            }
        }

        // Second pass: rewrite with replaced names
        {
            size_t offset = 0;
            WGSLTokenTypes last_type = (WGSLTokenTypes)-1;
            while (offset < originalData.size()) {
                WGSLTokenTypes type = GetToken(originalData.data(), offset);

                if (type == FLOAT4) {
                    uint32_t val;
                    memcpy(&val, &originalData[offset], 4);
                    offset += 4;
                    float f = *(float*)&val;
                    AppendFloat(data, f);
                }
                else if (type == FLOAT8) {
                    uint64_t val;
                    memcpy(&val, &originalData[offset], 8);
                    offset += 8;
                    double d = *(double*)&val;
                    AppendFloat(data, d);
                }
                else if (type == NUMBER4) {
                    int32_t val;
                    memcpy(&val, &originalData[offset], 4);
                    offset += 4;
                    AppendNumber(data, val);
                }
                else if (type == NUMBER8) {
                    uint64_t val;
                    memcpy(&val, &originalData[offset], 8);
                    offset += 8;
                    AppendNumber(data, val);
                }
                else if (type == UNSIGNED_NUMBER4) {
                    uint32_t val;
                    memcpy(&val, &originalData[offset], 4);
                    offset += 4;
                    AppendUnsignedNumber(data, val);
                }
                else if (type == UNSIGNED_NUMBER8) {
                    uint64_t val;
                    memcpy(&val, &originalData[offset], 8);
                    offset += 8;
                    AppendUnsignedNumber(data, val);
                }
                else if (type == IDENTIFIER) {

                    uint8_t len;
                    memcpy(&len, &originalData[offset], 1);
                    offset += 1;
                    std::string text((const char*)&originalData[offset], len);
                    offset += len;
                    if (last_type == WGSLTokenTypes::PERIOD && text.size() > 1 && is_swizzle(text)) {
                        if (text.size() > 3)
                            AppendSwizzleToken(data, encode_swizzle(text, true));
                        else 
                            AppendShortSwizzleToken(data, encode_swizzle(text, false));
                    }
                    else if (canMinify(text)) {
                        uint16_t newName = renamedVars[text];
                        AppendIdentifierToken(data, newName);
                    }
                    else {
                        AppendChar(data, text[0]);
                    }
                }
                else {
                    // Reserved token or something no extra data
                    // Append as-is
                    AppendReservedToken(data, type);
                }
                last_type = type;
            }
        }
        newData.resize(data_size);
        newData.shrink_to_fit();
        return newData;
    }
};
std::string RemoveWhitespace(const std::string_view wgsl) {
    std::string result;
    bool inLineComment = false;
    bool inBlockComment = false;
    bool skipNextNewline = false;
    bool keepNextNewline = false;  // For after decorators

    for (size_t i = 0; i < wgsl.length(); i++) {
        // Handle start of comments
        if (!inBlockComment && !inLineComment && wgsl[i] == '/' && i + 1 < wgsl.length()) {
            if (wgsl[i + 1] == '/') {
                inLineComment = true;
                i++;
                continue;
            }
            if (wgsl[i + 1] == '*') {
                inBlockComment = true;
                i++;
                continue;
            }
        }

        // Handle end of comments
        if (inLineComment && wgsl[i] == '\n') {
            inLineComment = false;
            if (keepNextNewline) {
                result += '\n';
                keepNextNewline = false;
            }
            else if (!skipNextNewline) {
                result += ' ';
            }
            continue;
        }
        if (inBlockComment && wgsl[i] == '*' && i + 1 < wgsl.length() && wgsl[i + 1] == '/') {
            inBlockComment = false;
            i++;
            continue;
        }

        // Skip comment contents
        if (inLineComment || inBlockComment) continue;

        // Check for decorator (@)
        if (wgsl[i] == '@') {
            keepNextNewline = true;
        }

        // Handle whitespace
        if (std::isspace(wgsl[i])) {
            if (wgsl[i] == '\n') {
                if (keepNextNewline) {
                    result += '\n';
                    keepNextNewline = false;
                }
                else if (!skipNextNewline) {
                    result += ' ';
                }
            }
            else if (!result.empty() && !std::isspace(result.back())) {
                result += ' ';
            }
            continue;
        }

        // Set flag to skip next newline after specific characters
        if (wgsl[i] == '{' || wgsl[i] == ';') {
            skipNextNewline = true;
        }
        else if (!std::isspace(wgsl[i])) {
            skipNextNewline = false;
        }

        result += wgsl[i];
    }

    // Remove trailing whitespace
    while (!result.empty() && std::isspace(result.back())) {
        result.pop_back();
    }

    return result;
}
#if defined(WGSLPACK_USE_COMPRESSION)
template<typename TYPE>
std::string compress(const TYPE text) {
    // Get max compressed size to allocate buffer
    size_t const max_dst_size = ZSTD_compressBound(text.size());
    
    // Allocate compression buffer
    std::string compressed;
    compressed.resize(max_dst_size);
    
    // Compress the data
    size_t const compressed_size = ZSTD_compress(
        compressed, max_dst_size,
        text.data(), text.size(),
        21
    );
    
    // Check for compression error
    if (ZSTD_isError(compressed_size)) {
        delete[] compressed;
        return "";  // Return empty string on error
    }
    
    // Convert compressed data to string
    compressed.resize(compressed_size);
    compressed.shrink_to_fit();
    
    return compressed;
}
#if defined(WGSLPACK_ENABLE_TESTS)
template<typename TYPE>
uint32_t compressed_size(const TYPE text) {
    return compress(text).size();
}
#endif
#endif
void PrintTokens(const std::vector<uint8_t>& d) {
    size_t offset = 0;
    int count = 0;
    while (offset < d.size()) {
        WGSLTokenTypes token = GetToken(d.data(), offset);
        std::cout << count++ << ": " << (uint16_t)token << " (";

        // Print additional data based on token type
        switch (token) {
        case NUMBER4:
        case UNSIGNED_NUMBER4:
        case FLOAT4: {
            uint32_t val = ReadU32(d, offset);
            std::cout << val;
            break;
        }
        case NUMBER8:
        case UNSIGNED_NUMBER8:
        case FLOAT8: {
            uint64_t val = ReadU64(d, offset);
            std::cout << val;
            break;
        }
        case IDENTIFIER: {
            uint16_t val = GetVar(d.data(), offset);
            std::cout << "v" << val;
            break;
        }
        case SWIZZLE:
        case SHORTSWIZZLE: {
            uint8_t val = *(d.data() + offset++);
            std::string swizzle = decode_swizzle(val, token == SWIZZLE);
            std::cout << swizzle;
            break;
        }
        case _CHAR: {
            uint8_t val = *(d.data() + offset++);
            std::cout << (char)val;
            break;
        }
        default: {
            std::string text = getReservedWord(token);
            if (!text.empty()) {
                std::cout << text;
            }
            break;
        }
        }
        std::cout << ")\n";
    }
}

struct TokenStats {
    std::unordered_map<WGSLTokenTypes, int> counts;
    std::unordered_map<WGSLTokenTypes, size_t> sizes;

    void analyze(const std::vector<uint8_t>& d) {
        size_t offset = 0;
        while (offset < d.size()) {
            size_t start_offset = offset;
            WGSLTokenTypes token = GetToken(d.data(), offset);

            switch (token) {
            case NUMBER4:
            case UNSIGNED_NUMBER4:
            case FLOAT4:
                offset += 4;
                break;
            case NUMBER8:
            case UNSIGNED_NUMBER8:
            case FLOAT8:
                offset += 8;
                break;
            case IDENTIFIER:
                GetVar(d.data(), offset);
                break;
            case SWIZZLE:
            case SHORTSWIZZLE:
            case _CHAR:
                offset += 1;
                break;
            default:
                // noop
                break;
            }

            counts[token]++;
            sizes[token] += (offset - start_offset);
        }
    }

    void print() const {
        std::print("\nToken Statistics:\n");
        std::print("Token Type | Count | Total Bytes\n");
        std::print("-----------+-------+------------\n");

        std::vector<std::pair<WGSLTokenTypes, size_t>> sorted;
        for (const auto& [token, size] : sizes) {
            sorted.push_back({ token, size });
        }
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        size_t total_size = 0;
        int total_count = 0;

        for (const auto& [token, size] : sorted) {
            std::string name = getReservedWord(token);
            if (name.empty()) name = std::to_string((int)token);
            else name += ":" + std::to_string((int)token);
            
            std::print("{:<10} | {:>5} | {:>10} bytes\n", 
                name, counts.at(token), size);

            total_size += size;
            total_count += counts.at(token);
        }

        std::print("-----------+-------+------------\n");
        std::print("Total      | {:>5} | {:>10} bytes\n", 
            total_count, total_size);
    }
};
std::vector<uint8_t> pack(const std::string_view wgsl) {
    WGSLPreprocessor preprocessor;
    VerifyWithTint(wgsl);
    auto wgsl_test_processed = RemoveWhitespace(wgsl);
    VerifyWithTint(wgsl_test_processed);
    std::vector<uint8_t> originalTokens;
    originalTokens.resize(wgsl_test_processed.size() * 2);
    uint8_t* data = (uint8_t*)originalTokens.data();
    uint32_t size = 0;
    Tokenize(data, size, wgsl_test_processed);
    originalTokens.resize(data_size);
    originalTokens.shrink_to_fit();
    data_size2 = data_size;
    data_size = 0;
    return preprocessor.preprocess(originalTokens);
}
std::string unpack(const std::vector<uint8_t>& binary) {
    auto reconstructed = ToWGSL(binary);
#if defined(WGSLPACK_ENABLE_VERIFICATION)
    auto str = VerifyWithTint(reconstructed);
    if (str.contains("unresolved call target")) {
        // Extract identifier between single quotes
        size_t start = str.find('\'') + 1;
        size_t end = str.find('\'', start);
        std::string target = str.substr(start, end - start);
        std::print("unresolved call target: {} Original name: {}\n", target, preprocessor.getOriginalName(target));
        return;
    }
#endif

#if defined(WGSLPACK_USE_COMPRESSION)
    auto compressed = compress(binary);
#endif
#if defined(WGSLPACK_ENABLE_TESTS)
    std::print("Original size: {}\n", wgsl.size());
#if defined(WGSLPACK_USE_COMPRESSION)
    std::print("Original compressed size: {}\n", compressed_size(wgsl));
#endif
    std::print("Binary size: {}\n", binary.size());
#if defined(WGSLPACK_USE_COMPRESSION)
    std::print("Binary compressed size: {}\n", compressed.size());
#endif
    std::print("Reconstructed size: {}\n", reconstructed.size());
#if defined(WGSLPACK_USE_COMPRESSION)
    std::print("Reconstructed compressed size: {}\n", compressed_size(reconstructed));
#endif
#endif

#if defined(WGSLPACK_ENABLE_VERIFICATION)
    TokenStats stats;
    stats.analyze(binary);
    stats.print();
#endif
    return reconstructed;
}
std::string output_file = "";
void process(const std::string_view wgsl) {
    auto binary = pack(wgsl);
    auto reconstructed = unpack(binary);
    // Write to file
    std::ofstream file(output_file);
    file << reconstructed;
}
void init() {
#if defined(WGSLPACK_ENABLE_VERIFICATION)
    InitializeTint();
#endif
    InitReservedWords();
}
// used like this
// wgslpack ./source.wgsl -o ./dest.wgsl --compress
#if !defined(__EMSCRIPTEN__)
int main(int argc, char** argv) {
    std::string source_file = "";
    bool compress = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-o") {
            output_file = argv[++i];
        }
        else if (arg == "--compress") {
            compress = true;
        }
        else {
            source_file = arg;
        }
    }
    init();
    std::ifstream file(source_file);
    std::string wgsl_from_file((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    process(wgsl_from_file);
    return 0;
}
#else
using namespace emscripten;
val pack_wasm(std::string source) {
    auto result = pack(source);
    return val(typed_memory_view(result.size(), result.data()));
}

std::string unpack_wasm(val packed) {
    std::vector<uint8_t> data(packed["length"].as<size_t>());
    val view = val(typed_memory_view(data.size(), data.data()));
    view.call<void>("set", packed);
    return unpack(data);
}
EMSCRIPTEN_BINDINGS(wgslpack) {
    function("init", &init);
    function("pack", &pack_wasm);
    function("unpack", &unpack_wasm);
}
#endif