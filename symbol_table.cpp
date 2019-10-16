#include "symbol_table.hpp"
namespace lake{
bool VarSemSym::SetType(std::string type){
	if(type == "VOID"){
		//throw error 
		return(false);
	}
	else{
		m_type = type;
	}
}
void VarSemSym::SetId(std::string Id){
	m_name = Id;
}
void VarSemSym::SetDeclared(bool isDeclared){
	m_isDeclared = isDeclared;
}

void FuncSemSym::SetArgsList(std::list<std::string>* argsList){
	m_argsList = argsList;
}
bool FuncSemSym::SetType(std::string returnType){
	m_returnType = returnType;
	return true;
}
void SetId(std::string Id){}
ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
}


}
