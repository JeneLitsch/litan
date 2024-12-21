#include "analyze.hxx"
#include <iostream>
#include "stdxx/iife.hxx"

#include "ltnc/ast/expr/Map.hxx"
#include "ltnc/ast/expr/Struct.hxx"

#include "ltnc/sst/expr/Map.hxx"
#include "ltnc/sst/expr/Struct.hxx"

namespace ltn::c {
	namespace {
		sst::expr::Struct::Member analyze_member(const Scope & scope, sst::expr_ptr expr, const std::string & name) {
			return sst::expr::Struct::Member {
				.address = scope.resolve_member_id(name),
				.expr = std::move(expr),
			};
		}
	}


	sst::expr_ptr analyze_expr(const ast::expr::Struct & init, Scope & scope) {
		
		auto sst_init = sst::expr::strukt();

		for(const auto & [member, expr] : init.members) {
			auto sst_expr = analyze_expression(*expr, scope);
			sst_init->members.push_back(analyze_member(scope, std::move(sst_expr), member));
		}

		return sst_init;
	}


	
	sst::expr_ptr analyze_expr(const ast::expr::Map & init, Scope & scope) {
		auto map = sst::expr::map();
		for(auto & [key, val] : init.pairs) {
			map->pairs.push_back(sst::expr::Map::Pair{
				.key = analyze_expression(*key, scope),
				.val = analyze_expression(*val, scope),
			});
		}
		return map;
	}
}