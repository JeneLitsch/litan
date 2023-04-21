#include "instructions.hxx"
#include "ltn/fxcodes.hxx"
#include "ltnvm/build_in/algorithm.hxx"
#include "ltnvm/build_in/random.hxx"
#include "ltnvm/build_in/functional.hxx"
#include "ltnvm/build_in/io.hxx"
#include "ltnvm/build_in/math.hxx"
#include "ltnvm/build_in/string.hxx"

namespace ltn::vm::inst {
	static constexpr auto make_build_in_table() {
		struct TableWrapper{
			using Fx = Value(*)(VmCore&);
			constexpr TableWrapper() {
				std::fill(std::begin(array), std::end(array), [] (auto &) -> Value {
					std::stringstream ss;
					ss << "Illegal Build In Function";
					throw std::runtime_error{ss.str()};
				});
			}
			std::array<Fx, 256> array;

			constexpr Fx & operator[](FxCode op_code) {
				return array[static_cast<std::size_t>(op_code)];
			}
		} table;
		


		// algorithm
		table[FxCode::SORT_ASCN] = build_in::sort_ascn;
		table[FxCode::SORT_DESC] = build_in::sort_desc;
		table[FxCode::IS_SORTED_ASCN] = build_in::is_sorted_ascn;
		table[FxCode::IS_SORTED_DESC] = build_in::is_sorted_desc;
		table[FxCode::FIND] = build_in::find;
		table[FxCode::FILL] = build_in::fill;
		table[FxCode::REVERSE] = build_in::reverse;
		
		// random
		table[FxCode::SPLIT] = build_in::split;
		table[FxCode::RAND] = build_in::rand;
		table[FxCode::RAND_INT] = build_in::rand_int;
		table[FxCode::RAND_FLOAT] = build_in::rand_float;

		//functional
		table[FxCode::ARITY] = build_in::arity;

		//io
		table[FxCode::IO_RESET_COLOR] = build_in::reset_color;
		table[FxCode::IO_SET_FG_COLOR] = build_in::set_fg_color;
		table[FxCode::IO_SET_BG_COLOR] = build_in::set_bg_color;
		table[FxCode::IO_IS_EOF] = build_in::is_eof;
		table[FxCode::IO_CLOSE_STREAM] = build_in::close_stream;
		table[FxCode::IO_PRINT] = build_in::print;
		table[FxCode::IO_READ_STR] = build_in::read_str;
		table[FxCode::IO_READ_LINE] = build_in::read_line;
		table[FxCode::IO_READ_BOOL] = build_in::read_bool;
		table[FxCode::IO_READ_CHAR] = build_in::read_char;
		table[FxCode::IO_READ_INT] = build_in::read_int;
		table[FxCode::IO_READ_FLOAT] = build_in::read_float;
		table[FxCode::IO_READ_ALL] = build_in::read_all;

		table[FxCode::IO_COUT] = build_in::cout;
		table[FxCode::IO_FOUT] = build_in::fout;
		table[FxCode::IO_STROUT] = build_in::strout;
		table[FxCode::IO_CIN] = build_in::cin;
		table[FxCode::IO_FIN] = build_in::fin;
		table[FxCode::IO_STRIN] = build_in::strin;

		// string
		table[FxCode::TO_STRING] = build_in::to_string;
		table[FxCode::SPLIT_STRING] = build_in::split_string;
		table[FxCode::JOIN_STRING] = build_in::join_string;

		// Math
		table[FxCode::MIN] = build_in::min;
		table[FxCode::MAX] = build_in::max;
		table[FxCode::CLAMP] = build_in::clamp;
		table[FxCode::ROUND] = build_in::round;
		table[FxCode::FLOOR] = build_in::floor;
		table[FxCode::CEIL] = build_in::ceil;
		table[FxCode::ABS] = build_in::abs;
		table[FxCode::SIN] = build_in::sin;
		table[FxCode::COS] = build_in::cos;
		table[FxCode::TAN] = build_in::tan;
		table[FxCode::SQRT] = build_in::sqrt;
		table[FxCode::HYPOT] = build_in::hypot;
		table[FxCode::LOG] = build_in::log;
		table[FxCode::LN] = build_in::ln;
		table[FxCode::LD] = build_in::ld;
		table[FxCode::LG] = build_in::lg;
		
		return table.array;
	}

	static constexpr auto build_in_table = make_build_in_table();

	void build_in(VmCore & core) {
		const auto byte0 = static_cast<std::uint32_t>(core.fetch_byte());
		const auto byte1 = static_cast<std::uint32_t>(core.fetch_byte());
		const auto code = (byte0 << 8) + byte1;
		const Value result = build_in_table[code](core);
		core.stack.push(result);
	}
}
