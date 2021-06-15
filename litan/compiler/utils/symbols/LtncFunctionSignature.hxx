#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "LtncType.hxx"
#include "LtncVar.hxx"
namespace ltnc {
	class FunctionSignature {
	public:
		FunctionSignature(const TypeId & returnType, std::string name, const std::vector<Param> & params)
			: returnType(returnType), name(name), params(params) {}
		
		TypeId returnType;
		std::string name;
		std::vector<Param> params;
	};


	inline bool operator==(const FunctionSignature & signatureL, const FunctionSignature & signatureR) {
		if(signatureL.params.size() != signatureR.params.size()) return false;

		for(std::size_t idx = 0; idx < signatureL.params.size(); idx++){
			const Param & paramL = signatureL.params[idx];
			const Param & paramR = signatureR.params[idx];
			if(paramL.typeId != paramR.typeId) {
				return false;
			}
		}
		return signatureL.name == signatureR.name;
	}
};



	// inline bool operator<(const FunctionSignature & signatureL, const FunctionSignature & signatureR) {
	// 	if(signatureL.params.size() < signatureR.params.size()) {
	// 		return true;
	// 	}
	// 	for(std::size_t idx = 0; idx < signatureR.params.size(); idx++){
	// 		const Param & paramL = signatureL.params[idx];
	// 		const Param & paramR = signatureR.params[idx];
	// 		if(paramL.type < paramR.type) {
	// 			return true;
	// 		}
	// 	}
	// 	return signatureL.name < signatureR.name;
	// }



	// inline bool operator>(const FunctionSignature & signatureL, const FunctionSignature & signatureR) {
	// 	if(signatureL.params.size() > signatureR.params.size()) {
	// 		return true;
	// 	}
	// 	for(std::size_t idx = 0; idx < signatureL.params.size(); idx++){
	// 		const Param & paramL = signatureL.params[idx];
	// 		const Param & paramR = signatureR.params[idx];
	// 		if(paramL.type > paramR.type) {
	// 			return true;
	// 		}
	// 	}
	// 	return signatureL.name > signatureR.name;
	// }