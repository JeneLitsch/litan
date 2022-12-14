#pragma once
#include <vector>
#include "stdxx/memory.hxx"

namespace ltn::c::sst {
	struct Expression;

	template<typename Base, typename Case>
	struct Switch : Base {
		Switch(const type::Type & type) : Base(type) {}
		std::unique_ptr<Expression> condition;
		std::vector<std::pair<
			std::unique_ptr<Expression>,
			std::unique_ptr<Case>
		>> cases;
		std::unique_ptr<Case> d3fault;
	};
}