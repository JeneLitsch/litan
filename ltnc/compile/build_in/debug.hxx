#pragma once
#include <array>
#include <string_view>
using namespace std::string_view_literals;
namespace ltn::c::compile {
	constexpr auto debug_state = std::array{
		"state"sv,
		"return"sv,
	};
}