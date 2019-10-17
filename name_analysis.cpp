#include "ast.hpp"
#include "symbol_table.hpp"
#include "err.hpp"

namespace lake{

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	return this->myDeclList->nameAnalysis(symTab);
}

bool DeclListNode::nameAnalysis(SymbolTable * symTab){
	bool result = true;
	for (auto decl : *myDecls){
		result = decl->nameAnalysis(symTab) && result;
	}
	return result;
}

bool VarDeclListNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//calls name analysis on all the vardecls and gets if they were successful or not
	for (auto decl : *myDecls){
		nameAnalysisOk = decl->nameAnalysis(symTab) && nameAnalysisOk;
	}
	// throw new ToDoError("[DELETE ME] I'm a varDeclListNode"
	// 	" you should iterate over my subtree, adding"
	// 	" symbols as neccesary to the current scope"
	// );
	return nameAnalysisOk;
}

bool StmtListNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	throw new ToDoError("[DELETE ME] I'm a stmtListNode"
		" you should iterate over my subtree, using"
		" the symbols previously gathered to "
		" see if variables are declared"
	);
	return nameAnalysisOk;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol* myVar = new VarSemSym();
	myVar->SetType(myType->getType());
	myVar->SetId(myID->getString());
	nameAnalysisOk = symTab->addSym(myVar);
	// throw new ToDoError("[DELETE ME] I'm a varDecl"
	// 	" you should add the information from my"
	// 	" subtree to the symbolTable as a new"
	// 	" entry in the current scope table"
	// );
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;

	// throw new ToDoError("[DELETE ME] I'm an fnDecl."
	// 	" you should add and make current a new"
	// 	" scope table for my body"
	// );
	return nameAnalysisOk;
}

bool FormalsListNode::nameAnalysis(SymbolTable* symTab){
	FormalDeclNode* DeclNodeTemp;
	std::string formalsListString;
	for (std::list<FormalDeclNode *>::iterator it=myFormals->begin(); it!=myFormals->end(); ++it){
		DeclNodeTemp = *it;
		std::string currArgType = DeclNodeTemp->GetType();
		formalsListString = formalsListString + "," + currArgType;
	}

}
}
