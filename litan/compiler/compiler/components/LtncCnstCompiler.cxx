#include "LtncCnstCompiler.hxx"
#include "Unused.hxx"

ltnc::CnstCompiler::CnstCompiler() {}

ltnc::ExprInfo ltnc::CnstCompiler::compile(
	CompilerPack & compPkg,
	const std::shared_ptr<ExprNew> & exprNew) const {
	return this->defaultConstructor(compPkg, exprNew);
}

ltnc::ExprInfo ltnc::CnstCompiler::defaultConstructor(
	CompilerPack & compPkg,
	const std::shared_ptr<ExprNew> & exprNew) const {
	const Type & type = compPkg.getSymbolTable().match(exprNew->typeId);

	
	CodeBuffer code = compPkg.codeBuffer();
	code << Comment(exprNew->typeId.name);
	code << AssemblyCode("array::new");
	for(const auto & member : type.members) {
		UNUSED(member);
		code << AssemblyCode("copy");
		code << Inst::newl(0); // init with nullptr;
		code << AssemblyCode("array::add");
	}
	return ExprInfo(TypeId(exprNew->typeId), code);
}	