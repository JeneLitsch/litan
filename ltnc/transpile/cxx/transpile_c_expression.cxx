#include "transpile_c.hxx"

namespace ltn::c::trans::cxx {
	void transpile_c_expr(const sst::Choose &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::Ternary & ternary, std::ostream & out, Indent indent) {
		out << "[&] () {\n";
		out << indent.in() << "const ltn::Tmp condition = ";
		transpile_c_expression(*ternary.condition, out, indent.in());
		out << ";\n";

		out << indent.in() << "if(ltn::is_truthy(condition)) {\n";
		out << indent.in().in() << "return ";
		transpile_c_expression(*ternary.if_branch, out, indent.in());
		out << ";\n";
		out << indent.in() << "}\n";
		out << indent.in() << "else {\n";
		out << indent.in().in() << "return ";
		transpile_c_expression(*ternary.else_branch, out, indent.in());
		out << ";\n";
		out << indent.in() << "}\n";
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Binary & expr, std::ostream & out, Indent indent) {
		const auto inner = indent.in();
		out << "[&] () {\n";
		out << inner << "const ltn::Tmp l = "; transpile_c_expression(*expr.l, out, indent.in()); out << ";\n";
		out << inner << "const ltn::Tmp r = "; transpile_c_expression(*expr.r, out, indent.in()); out << ";\n";
		out << inner << "return ";
		using OP = ltn::c::sst::Binary::Op;
		switch (expr.op)
		{
			case OP::ADD:          out << "ltn::add(l, r)";      break;
			case OP::SUB:          out << "ltn::sub(l, r)";      break;
			case OP::MLT:          out << "ltn::mlt(l, r)";      break;
			case OP::DIV:          out << "ltn::div(l, r)";      break;
			case OP::MOD:          out << "ltn::mod(l, r)";          break;
			case OP::POW:          out << "ltn::pow(l, r)";          break;
			case OP::SMALLER:      out << "ltn::smaller(l, r)";      break;
			case OP::BIGGER:       out << "ltn::bigger(l, r)";       break;
			case OP::SMALLEREQUAL: out << "ltn::smallerequal(l, r)"; break;
			case OP::BIGGEREQUAL:  out << "ltn::biggerequal(l, r)";  break;
			case OP::EQUAL:        out << "ltn::equal(l, r)";        break;
			case OP::UNEQUEL:      out << "ltn::unequel(l, r)";      break;
			case OP::SPACE_SHIP:   out << "ltn::space_ship(l, r)";   break;
			case OP::SHIFT_L:      out << "ltn::shift_l(l, r)";      break;
			case OP::SHIFT_R:      out << "ltn::shift_r(l, r)";      break;
			case OP::BIT_AND:      out << "ltn::bit_and(l, r)";      break;
			case OP::BIT_OR:       out << "ltn::bit_or(l, r)";       break;
			case OP::BIT_XOR:      out << "ltn::bit_xor(l, r)";      break;
			case OP::AND:          out << "ltn::and(l, r)";          break;
			case OP::OR:           out << "ltn::or(l, r)";           break;
			case OP::ELVIS:        out << "ltn::elvis(l, r)";        break;
			case OP::NULLCO:       out << "ltn::nullco(l, r)";       break;
		}
		out << ";\n";
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Unary & expr, std::ostream & out, Indent indent) {
		out << "[&] () {\n";
		out << indent.in() << "const ltn::Tmp x = "; transpile_c_expression(*expr.expr, out, indent.in()); out << ";\n";
		out << indent.in() << "return ";
		using OP = ltn::c::sst::Unary::Op;
		switch (expr.op) {
			case OP::NEG:    out << "ltn::neg(x)";    break; 
			case OP::NOT:    out << "ltn::not(x)";    break; 
			case OP::NUL:    out << "ltn::nul(x)";    break; 
			case OP::BITNOT: out << "ltn::bitnot(x)"; break; 
			case OP::DEREF:  out << "ltn::deref(x)";  break; 
		}
		out << ";\n";
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Integer & literal, std::ostream & out, Indent indent) {
		out << "ltn::value_int(" << literal.value << ")"; 
	}

	void transpile_c_expr(const sst::Float & literal, std::ostream & out, Indent indent) {
		out << "ltn::value_float(" << literal.value << ")"; 
	}

	void transpile_c_expr(const sst::Bool & literal, std::ostream & out, Indent indent) {
		out << "ltn::value_bool(" << literal.value << ")"; 
	}

	void transpile_c_expr(const sst::Char & literal, std::ostream & out, Indent indent) {
		out << "ltn::value_char(" << literal.value << ")"; 
	}

	void transpile_c_expr(const sst::Null &, std::ostream & out, Indent indent) {
		out << "ltn::value_null());\n"; 
	}
	


	void transpile_c_expr(const sst::String & literal, std::ostream & out, Indent indent) {
		out << "ltn::value_string(\"";
		for(const char c : literal.value) {
			switch (c) {
			case '\n': out << "\\n"; break;
			case '\t': out << "\\t"; break;
			case '\"': out << "\\\""; break;
			case '\\': out << "\\\\"; break;
			default: out << c;
			}
		} 
		out << "\")"; 
	}



	void transpile_c_expr(const sst::Array & literal, std::ostream & out, Indent indent) {
		const auto inner = indent.in();
		out << "[&] () {\n";
		out << inner << "ltn::Tmp tmp = ltn::value_array();\n"; 
		out << inner << "auto & arr = tmp.get().val.arr->value;\n";
		for(const auto & elem : literal.elements) {
			out << inner << "arr.push_back(";
			transpile_c_expression(*elem, out, inner.in());
			out << ");\n";
		} 
		out << inner << "return tmp.get();\n"; 
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Tuple & literal, std::ostream & out, Indent indent) {
		const auto inner = indent.in();
		out << "[&] () {\n";
		out << inner << "ltn::Tmp tmp = ltn::value_tuple();\n"; 
		out << inner << "auto & tup = tmp.get().val.tup->value;\n";
		for(const auto & elem : literal.elements) {
			out << inner << "tup.push_back(";
			transpile_c_expression(*elem, out, inner.in());
			out << ");\n";
		} 
		out << inner << "return tmp.get();\n"; 
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Call & call, std::ostream & out, Indent indent) {
		const auto inner = indent.in();
		out << "[&] () {\n";
		for(std::size_t i = 0; i < std::size(call.arguments); ++i) {
			out << inner << "const ltn::Tmp arg_" << i << " = ";
			transpile_c_expression(*call.arguments[i], out, inner);
			out << ";\n";  
		}
		out << inner << "return ";
		out << "fx::";
		print_namespace(call.namespaze, out);
		if(!std::empty(call.namespaze)) {
			out << "::";
		}
		out << call.name << "_" << call.arity() << "(";
		for(std::size_t i = 0; i < std::size(call.arguments); ++i) {
			if(i) out << ",";
			out << "arg_" << i << ".get()";
		}
		out << ");\n";
		out << indent << "}()";
	}


	
	void transpile_c_expr(const sst::Invoke &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::InvokeMember &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::Index &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::FxPointer &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::Iife & iife, std::ostream & out, Indent indent) {
		out << "[&] () {\n";
		transpile_c_statement(*iife.stmt, out, indent.in());
		out << indent << "}()";
	}



	void transpile_c_expr(const sst::Var & var, std::ostream & out, Indent indent) {
		out << "var_" << var.address << ".get()";
	}



	void transpile_c_expr(const sst::Member &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::GlobalVar & global, std::ostream & out, Indent indent) {
		out << "global::var_" << global.address << ".get()";
	}
	void transpile_c_expr(const sst::Reflect &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::InitStruct &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::Map &, std::ostream & out, Indent indent) {}
	void transpile_c_expr(const sst::Type &, std::ostream & out, Indent indent) {}



	void transpile_c_expression(const sst::Expression & expr, std::ostream & out, Indent indent) {
		sst::visit_expression(expr, [&] (const auto & e) {
			transpile_c_expr(e, out, indent);
			return 0;
		});
	}
}