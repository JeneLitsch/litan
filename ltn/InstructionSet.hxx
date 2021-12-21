#pragma once
#include <cstdint>
namespace ltn {
	enum class Inst : std::uint8_t {
		EXIT = 0x00, ERROR, STATE,
		ADD = 0x10, SUB, MLT, DIV, MOD,
		EQL, UEQL, SML, BGR, SMLEQL, BGREQL,
		SHIFT_L, SHIFT_R,

		NEG = 0x20, NOT, INC, DEC,
		
		NEWI = 0x30, NEWF, NEWU, TRUE, FALSE, NVLL, CHAR, ELEM,
		CHAR_4 = 0x38, CHAR_8,

		JUMP = 0x40, CALL, RETURN, IF, INVOKE, EXTERNAL, CAPTURE,

		NEWARR = 0x50, NEWSTR, NEWOUT, NEWIN, NEWFX, NEWCLOCK,

		READ = 0x60, WRITE, SCRAP, MAKEVAR, AT, AT_WRITE, READ_X, WRITE_X,
		READ_0 = 0x68, READ_1, READ_2, READ_3, 
		WRITE_0, WRITE_1, WRITE_2, WRITE_3,

		OUT = 0x70, IN_STR, IN_LINE, IN_INT, IN_FLOAT, IS_EOF,

		MIN = 0x80, MAX, ROUND, FLOOR, CEIL, ABS, HYPOT, SQRT,
		SIN = 0x90, COS, TAN,

		SIZE = 0xa0, FRONT, BACK,
		INSERT_BACK, INSERT_FRONT, INSERT,
		REMOVE_BACK, REMOVE_FRONT, REMOVE,

		TO_SECONDS = 0xb0,

		TYPEID = 0xc0,

		
	};
}