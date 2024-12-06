#if defined(WGSLPACK_ENABLE_VERIFICATION)
#include "src/tint/api/tint.h"
#include "src/tint/cmd/common/helper.h"
#include "src/tint/lang/wgsl/ast/module.h"
#include "src/tint/lang/wgsl/ast/transform/first_index_offset.h"
#include "src/tint/lang/wgsl/ast/transform/manager.h"
#include "src/tint/lang/wgsl/ast/transform/renamer.h"
#include "src/tint/lang/wgsl/ast/transform/single_entry_point.h"
#include "src/tint/lang/wgsl/ast/transform/substitute_override.h"
#include "src/tint/lang/wgsl/helpers/flatten_bindings.h"
#include "src/tint/utils/command/command.h"
#include "src/tint/utils/containers/transform.h"
#include "src/tint/utils/diagnostic/formatter.h"
#include "src/tint/utils/macros/defer.h"
#include "src/tint/utils/text/string.h"
#include "src/tint/utils/text/string_stream.h"
#include "src/tint/lang/spirv/reader/reader.h"
#include "src/tint/lang/wgsl/reader/reader.h"
#include "src/tint/lang/wgsl/writer/writer.h"
#include "src/tint/lang/core/builtin_value.h"
#include "src/tint/lang/core/fluent_types.h"
#include "src/tint/lang/core/type/depth_texture.h"
#include "src/tint/lang/core/type/sampled_texture.h"
#include "src/tint/lang/core/type/texture_dimension.h"
#include "src/tint/lang/wgsl/ast/assignment_statement.h"
#include "src/tint/utils/text/styled_text_printer.h"
#include "src/tint/lang/wgsl/ast/break_statement.h"
#include "src/tint/lang/wgsl/ast/if_statement.h"
#include "src/tint/lang/wgsl/ast/loop_statement.h"
#include "src/tint/lang/wgsl/ast/return_statement.h"
#include "src/tint/lang/wgsl/ast/stage_attribute.h"
#include "src/tint/lang/wgsl/ast/switch_statement.h"
#include "src/tint/lang/wgsl/ast/unary_op_expression.h"
#include "src/tint/lang/wgsl/ast/variable_decl_statement.h"
#include "src/tint/utils/containers/hashmap.h"
#include "src/tint/utils/containers/hashset.h"
#include "src/tint/lang/core/type/atomic.h"
#include "src/tint/lang/core/type/depth_multisampled_texture.h"
#include "src/tint/lang/core/type/depth_texture.h"
#include "src/tint/lang/core/type/multisampled_texture.h"
#include "src/tint/lang/core/type/sampled_texture.h"
#include "src/tint/lang/core/type/storage_texture.h"
#include "src/tint/lang/core/type/texture_dimension.h"
#include "src/tint/lang/glsl/writer/common/options.h"
#include "src/tint/lang/wgsl/ast/call_statement.h"
#include "src/tint/lang/wgsl/ast/id_attribute.h"
#include "src/tint/lang/wgsl/ast/internal_attribute.h"
#include "src/tint/lang/wgsl/ast/interpolate_attribute.h"
#include "src/tint/lang/wgsl/ast/transform/add_block_attribute.h"
#include "src/tint/lang/wgsl/ast/transform/add_empty_entry_point.h"
#include "src/tint/lang/wgsl/ast/transform/binding_remapper.h"
#include "src/tint/lang/wgsl/ast/transform/builtin_polyfill.h"
#include "src/tint/lang/wgsl/ast/transform/canonicalize_entry_point_io.h"
#include "src/tint/lang/wgsl/ast/transform/demote_to_helper.h"
#include "src/tint/lang/wgsl/ast/transform/direct_variable_access.h"
#include "src/tint/lang/wgsl/ast/transform/disable_uniformity_analysis.h"
#include "src/tint/lang/wgsl/ast/transform/expand_compound_assignment.h"
#include "src/tint/lang/wgsl/ast/transform/manager.h"
#include "src/tint/lang/wgsl/ast/transform/multiplanar_external_texture.h"
#include "src/tint/lang/wgsl/ast/transform/preserve_padding.h"
#include "src/tint/lang/wgsl/ast/transform/promote_initializers_to_let.h"
#include "src/tint/lang/wgsl/ast/transform/promote_side_effects_to_decl.h"
#include "src/tint/lang/wgsl/ast/transform/remove_phonies.h"
#include "src/tint/lang/wgsl/ast/transform/renamer.h"
#include "src/tint/lang/wgsl/ast/transform/robustness.h"
#include "src/tint/lang/wgsl/ast/transform/simplify_pointers.h"
#include "src/tint/lang/wgsl/ast/transform/single_entry_point.h"
#include "src/tint/lang/wgsl/ast/transform/std140.h"
#include "src/tint/lang/wgsl/ast/transform/unshadow.h"
#include "src/tint/lang/wgsl/ast/transform/zero_init_workgroup_memory.h"
#include "src/tint/lang/wgsl/ast/variable_decl_statement.h"
#include "src/tint/lang/wgsl/helpers/append_vector.h"
#include "src/tint/lang/wgsl/sem/block_statement.h"
#include "src/tint/lang/wgsl/sem/builtin_enum_expression.h"
#include "src/tint/lang/wgsl/sem/call.h"
#include "src/tint/lang/wgsl/sem/function.h"
#include "src/tint/lang/wgsl/sem/member_accessor_expression.h"
#include "src/tint/lang/wgsl/sem/module.h"
#include "src/tint/lang/wgsl/sem/statement.h"
#include "src/tint/lang/wgsl/sem/struct.h"
#include "src/tint/lang/wgsl/sem/switch_statement.h"
#include "src/tint/lang/wgsl/sem/value_constructor.h"
#include "src/tint/lang/wgsl/sem/value_conversion.h"
#include "src/tint/lang/wgsl/sem/variable.h"
#include "src/tint/utils/containers/map.h"
#include "src/tint/utils/ice/ice.h"
#include "src/tint/utils/macros/defer.h"
#include "src/tint/utils/macros/scoped_assignment.h"
#include "src/tint/utils/rtti/switch.h"
#include "src/tint/utils/strconv/float_to_string.h"
#include "src/tint/utils/text/string.h"
#include "src/tint/utils/text/string_stream.h"
#include "src/tint/lang/wgsl/ast/alias.h"
#include "src/tint/lang/wgsl/ast/struct.h"
#pragma comment(lib, "tint_lang_wgsl_writer_syntax_tree_printer.lib")
#pragma comment(lib, "tint_api.lib")
#pragma comment(lib, "tint_api_common.lib") 
#pragma comment(lib, "tint_cmd_common.lib")
#pragma comment(lib, "tint_lang_core.lib")
#pragma comment(lib, "tint_lang_core_constant.lib")
#pragma comment(lib, "tint_lang_core_intrinsic.lib")
#pragma comment(lib, "tint_lang_core_ir.lib")
#pragma comment(lib, "tint_lang_core_ir_transform.lib")
#pragma comment(lib, "tint_lang_core_type.lib")
#pragma comment(lib, "tint_lang_spirv.lib")
#pragma comment(lib, "tint_lang_spirv_ir.lib")
#pragma comment(lib, "tint_lang_spirv_type.lib")
#pragma comment(lib, "tint_lang_spirv_validate.lib")
#pragma comment(lib, "tint_lang_spirv_reader.lib")
#pragma comment(lib, "tint_lang_spirv_reader_parser.lib")
#pragma comment(lib, "tint_lang_spirv_reader_ast_parser.lib")
#pragma comment(lib, "tint_lang_spirv_reader_common.lib")
#pragma comment(lib, "tint_lang_spirv_reader_lower.lib")
#pragma comment(lib, "tint_lang_spirv_reader_ast_lower.lib")
#pragma comment(lib, "tint_lang_wgsl.lib")
#pragma comment(lib, "tint_lang_wgsl_ir.lib")
#pragma comment(lib, "tint_lang_wgsl_features.lib")
#pragma comment(lib, "tint_lang_wgsl_intrinsic.lib")
#pragma comment(lib, "tint_lang_wgsl_ast.lib")
#pragma comment(lib, "tint_lang_wgsl_ast_transform.lib")
#pragma comment(lib, "tint_lang_wgsl_helpers.lib")
#pragma comment(lib, "tint_lang_wgsl_inspector.lib")
#pragma comment(lib, "tint_lang_wgsl_program.lib")
#pragma comment(lib, "tint_lang_wgsl_reader.lib")
#pragma comment(lib, "tint_lang_wgsl_reader_lower.lib")
#pragma comment(lib, "tint_lang_wgsl_reader_parser.lib")
#pragma comment(lib, "tint_lang_wgsl_reader_program_to_ir.lib")
#pragma comment(lib, "tint_lang_wgsl_resolver.lib")
#pragma comment(lib, "tint_lang_wgsl_sem.lib")
#pragma comment(lib, "tint_lang_wgsl_writer.lib")
#pragma comment(lib, "tint_lang_wgsl_writer_raise.lib")
#pragma comment(lib, "tint_lang_wgsl_writer_ast_printer.lib")
#pragma comment(lib, "tint_lang_wgsl_writer_ir_to_program.lib")
#pragma comment(lib, "tint_utils_symbol.lib")
#pragma comment(lib, "tint_utils_text.lib")
#pragma comment(lib, "tint_utils_debug.lib")
#pragma comment(lib, "tint_utils_diagnostic.lib")
#pragma comment(lib, "tint_utils_generator.lib")
#pragma comment(lib, "tint_utils_ice.lib")
#pragma comment(lib, "tint_utils_id.lib")
#pragma comment(lib, "tint_utils_memory.lib")
#pragma comment(lib, "tint_utils_reflection.lib")
#pragma comment(lib, "tint_utils_result.lib")
#pragma comment(lib, "tint_utils_rtti.lib")
#pragma comment(lib, "tint_utils_strconv.lib")
#pragma comment(lib, "tint_utils_system.lib")
#pragma comment(lib, "absl_base.lib")
#pragma comment(lib, "absl_strings.lib")
#pragma comment(lib, "absl_strings_internal.lib")
#pragma comment(lib, "SPIRV-Tools.lib")
#pragma comment(lib, "SPIRV-Tools-opt.lib")
static void InitializeTint() {
	tint::Initialize();
	tint::SetInternalCompilerErrorReporter(&tint::cmd::TintInternalCompilerErrorReporter);
}
static std::string VerifyWithTint(const std::string_view wgsl) {
	struct TransformFactory {
		const char* name;
		std::function<bool(tint::inspector::Inspector&,
			tint::ast::transform::Manager& manager,
			tint::ast::transform::DataMap& inputs)>
			make;
	};
	std::vector<TransformFactory> transforms = {
			{"first_index_offset",
			 [](tint::inspector::Inspector&, tint::ast::transform::Manager& m,
				tint::ast::transform::DataMap& i) {
				 i.Add<tint::ast::transform::FirstIndexOffset::BindingPoint>(0, 0);
				 m.Add<tint::ast::transform::FirstIndexOffset>();
				 return true;
			 }},
			{"renamer",
			 [](tint::inspector::Inspector&, tint::ast::transform::Manager& m,
				tint::ast::transform::DataMap&) {
				 m.Add<tint::ast::transform::Renamer>();
				 return true;
			 }},
			{"robustness",
			 [&](tint::inspector::Inspector&, tint::ast::transform::Manager&,
				 tint::ast::transform::DataMap&) {  // enabled via writer option
					 //options.enable_robustness = true;
					 return true;
				 }},
				{"substitute_override",
				 [&](tint::inspector::Inspector& inspector, tint::ast::transform::Manager& m,
					 tint::ast::transform::DataMap& i) {
					 tint::ast::transform::SubstituteOverride::Config cfg;

					std::unordered_map<tint::OverrideId, double> values;
					cfg.map = std::move(values);

					i.Add<tint::ast::transform::SubstituteOverride::Config>(cfg);
					m.Add<tint::ast::transform::SubstituteOverride>();
				 return true;
			 }},
	};
	tint::Program program;
	enum class InputFormat {
		kUnknown,
		kWgsl,
		kSpirvBin,
		kSpirvAsm,
	};
	auto input_format = InputFormat::kSpirvBin;
	auto source_file = std::make_unique<tint::Source::File>("test", std::string(wgsl.begin(), wgsl.end()));
	tint::wgsl::reader::Options parser_options;
	parser_options.allowed_features = tint::wgsl::AllowedFeatures::Everything();
	program = tint::wgsl::reader::Parse(source_file.get(), parser_options);
	bool error = false;
	tint::Program::printer = [](const tint::Program& program) {
		auto result = tint::wgsl::writer::Generate(program, {});
		if (result != tint::Success) {
			return result.Failure().reason.Str();
		}
		return result->wgsl;
		};
	if (program.Diagnostics().NumErrors() > 0) {
		auto str = program.Diagnostics().Str();
        std::cerr << "Failed to parse: " << str << "\n";
		return str;
	}
    return "";
}
#else
static void InitializeTint() {
}
static std::string VerifyWithTint(const std::string_view wgsl) {
	return "";
}
#endif