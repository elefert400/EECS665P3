#include "ast.hpp"
#include "symbol_table.hpp"
#include "err.hpp"

namespace lake{

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	//create new empty scope
	ScopeTable* globalScope = new ScopeTable();
	//add new empty scope to symbol table
	symTab->addFront(globalScope);
	//call NA on list to continue
	return this->myDeclList->nameAnalysis(symTab);
}

bool DeclListNode::nameAnalysis(SymbolTable * symTab){
	bool result = true;
	/*looping over list called myDecls 
	that contains a variety of DeclNode(VarDeclNode||FnDeclNode)*/
	for (auto decl : *myDecls){
		//call NA on current iterations DeclNode
		result = decl->nameAnalysis(symTab) && result;
	}
	return result;
}

bool VarDeclListNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	/*looping over list called myDecls 
	that contains VarDeclNode's*/
	for (auto decl : *myDecls){
		//call NA on current iteration VarDeclNode
		nameAnalysisOk = decl->nameAnalysis(symTab) && nameAnalysisOk;
	}
	return nameAnalysisOk;
	// throw new ToDoError("[DELETE ME] I'm a varDeclListNode"
	// 	" you should iterate over my subtree, adding"	
	// 	" symbols as neccesary to the current scope"
	// );
}

bool StmtListNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	/*looping over list called myStmts 
	that contains a variety of StmtNode's*/
	for (auto stmt : *myStmts){
		//call NA on current iteration VarDeclNode
		nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
	}
	return nameAnalysisOk;
	// throw new ToDoError("[DELETE ME] I'm a stmtListNode"
	// 	" you should iterate over my subtree, using"	
	// 	" the symbols previously gathered to "	
	// 	" see if variables are declared"
	// );
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	TypeNode* varType = this->getTypeNode();
	std::string varId = this->getDeclaredName();
	VarSemSym* newSym = new VarSemSym();
	return nameAnalysisOk;
	// throw new ToDoError("[DELETE ME] I'm a varDecl"
	// 	" you should add the information from my"	
	// 	" subtree to the symbolTable as a new"	
	// 	" entry in the current scope table"
	// );
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
