#include "MemberTable.hxx"
#include <optional>
#include <map>

namespace ltn::c {
	namespace {
		std::optional<MemberCode> parse_reserved_member_name(const std::string & str) {
			const static std::map<std::string, MemberCode> table {
				{"__add__", MemberCode::OPERATOR_ADD},
				{"__sub__", MemberCode::OPERATOR_SUB},
				{"__mlt__", MemberCode::OPERATOR_MLT},
				{"__div__", MemberCode::OPERATOR_DIV},
				{"__mod__", MemberCode::OPERATOR_MOD},
				{"__pow__", MemberCode::OPERATOR_POW},
				{"__cmp__", MemberCode::OPERATOR_CMP},
				{"__str__", MemberCode::OPERATOR_STR},
				{"__bool__", MemberCode::OPERATOR_BOOL},

				{"size",        MemberCode::SIZE},
				{"is_empty",    MemberCode::IS_EMTPY},
				{"push",        MemberCode::PUSH},
				{"pop",         MemberCode::POP},
				{"peek",        MemberCode::PEEK},
				{"back",        MemberCode::BACK},
				{"front",       MemberCode::FRONT},
				{"at",          MemberCode::AT},
				{"insert",      MemberCode::INSERT},
				{"erase",       MemberCode::ERASE},
				{"has",         MemberCode::HAS},
				{"keys",        MemberCode::KEYS},
				{"values",      MemberCode::VALUES},
				{"filter",      MemberCode::FILTER},
				{"transform",   MemberCode::TRANSFORM},
				{"reduce",      MemberCode::REDUCE},
				{"any",         MemberCode::ANY},
				{"all",         MemberCode::ALL},
				{"none",        MemberCode::NONE},
				{"slice",       MemberCode::SLICE},
				{"prefix",      MemberCode::PREFIX},
				{"suffix",      MemberCode::SUFFIX},
				{"reversed",    MemberCode::REVERSED},
				{"merged",      MemberCode::MERGED},
				{"zipped",      MemberCode::ZIPPED},
			};
			if(table.contains(str)) {
				return table.at(str);
			}
			else {
				return std::nullopt;
			}

			// for(const auto & [name, code] : table) {
			// 	if(match(name, tokens)) {
			// 		return code;
			// 	}
			// }
			// return std::nullopt;
		}

		std::uint64_t resolve_member_id(auto & table, const std::string & str) {
			if(auto code = parse_reserved_member_name(str)) {
				return static_cast<std::uint64_t>(*code);
			}
			if(table.contains(str)) {
				return table.at(str);
			}
			const auto id = table.size() + 256;
			table.insert({str, id});
			return id;
		}
	}

	std::uint64_t MemberTable::get_id(const std::string & name) {
		return resolve_member_id(this->table, name);
	}
}