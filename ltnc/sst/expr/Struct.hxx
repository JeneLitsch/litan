#pragma once
#include <vector>
#include "Expression.hxx"

namespace ltn::c::sst {
	struct Struct final : public ExpressionCRTP<Struct> {
		struct Member {
			std::uint64_t address;
			expr_ptr expr;
		};

		Struct();

		virtual std::uint64_t alloc() const override;

		std::vector<Member> members;
	};
}