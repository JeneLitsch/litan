#pragma once
#include <sstream>
#include <span>
#include "litan_core/casts.hxx"
#include "litan_compiler/CompilerError.hxx"
#include "litan_compiler/ast/AST.hxx"
#include "litan_compiler/sst/SST.hxx"
#include "utils/Context.hxx"
#include "litan_compiler/make_jump_id.hxx"
#include "litan_compiler/labels.hxx"

#include "litan_compiler/scoping/Scope.hxx"
#include "litan_compiler/scoping/FunctionScope.hxx"
#include "litan_compiler/scoping/MajorScope.hxx"
#include "litan_compiler/scoping/MinorScope.hxx"

#include "error.hxx"

namespace ltn::c {
	struct AnalysisOptions {
		bool optimize = true;
	};

	sst::Program analyze(const ast::Program & program, const AnalysisOptions & options);

	sst::func_ptr analyze_function(
		const ast::decl::Function &,
		FunctionScope &,
		std::optional<Label> override_label,
		const std::vector<std::unique_ptr<ast::expr::Var>> & captures);
	
	sst::glob_ptr analyze_global(const ast::decl::Global &, Context &, std::uint64_t id);
	sst::defn_ptr analyze_definition(const ast::decl::Definition &, Context &, std::uint64_t id);
	std::vector<sst::defn_ptr> analyze_enumeration(const ast::decl::Enumeration &, std::uint64_t & id);

	sst::stmt_ptr analyze_statement(const ast::stmt::Statement &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Block &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Return &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Yield &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Assign &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Throw &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::IfElse &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::InfiniteLoop &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::While &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::ForEach &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::NewVar &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Switch &, Scope &);
	sst::stmt_ptr analyze_stmt(const ast::stmt::Conditional &, Scope &);

	sst::expr_ptr analyze_expression(const ast::expr::Expression &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Lambda &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Choose &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Ternary &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Binary &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Unary &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Integer &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Float &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Bool &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Char &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Null &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::String &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Array &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Tuple &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Call &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::InvokeMember &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Index &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::FxPointer &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Iife &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Var &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Member &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Reflect &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Struct &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Map &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::CustomLiteral &, Scope &);
	sst::expr_ptr analyze_expr(const ast::expr::Conditional &, Scope &);

	sst::bind_ptr analyze_binding(const ast::bind::Binding &, Scope &);
	sst::bind_ptr analyze_bind(const ast::bind::Group &, Scope &);
	sst::bind_ptr analyze_bind(const ast::bind::NewVar &, Scope &);
}	