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

bool TypeNode::nameAnalysis(SymbolTable * symTab)
{
	return true;
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

bool ExpListNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	/*looping over list called myDecls
	that contains a variety of DeclNode(VarDeclNode||FnDeclNode)*/
	for (auto decl : *myExps){
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

bool ExpNode::nameAnalysis(SymbolTable * symTab)
{
	return true;
}

bool DerefNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	return(result && myTgt->nameAnalysis(symTab));
	//needs to be filled in
}

bool IdNode::nameAnalysis(SymbolTable * symTab)
{
	if(symTab->CheckDeclared(myStrVal) == false)
	{
		return false;
	}
	SetSymbol(symTab->GetSem(myStrVal));
	return true;
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
	if(varType->getPtrDepth()){
		newSym->SetPtrDepth(varType->getPtrDepth());
	}	
	newSym->SetLineNum(varType->getLine());
	newSym->SetColumnNum(varType->getCol());
	newSym->SetType(varType->GetType());
	
	return(nameAnalysisOk && symTab->addSym(newSym));
	// throw new ToDoError("[DELETE ME] I'm a varDecl"
	// 	" you should add the information from my"
	// 	" subtree to the symbolTable as a new"
	// 	" entry in the current scope table"
	// );
}

bool FormalDeclNode::nameAnalysis(SymbolTable * symTab)
{
	bool nameAnalysisOk = true;
	TypeNode* varType = this->getTypeNode();
	std::string varId = this->getDeclaredName();
	VarSemSym* newSym = new VarSemSym();
	newSym->SetPtrDepth(varType->getPtrDepth());
	newSym->SetLineNum(varType->getLine());
	newSym->SetColumnNum(varType->getCol());
	newSym->SetType(varType->GetType());
	return(nameAnalysisOk && symTab->addSym(newSym));
}
bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//create new FnSemSym
	FuncSemSym* newSym = new FuncSemSym();
	newSym->SetType(this->GetType());
	newSym->SetId(this->getDeclaredName());

	//create new scope and add it to the front of symTab
	ScopeTable* newScope = new ScopeTable();
	symTab->addFront(newScope);	
	//call NA on formalsList and fnBodyNode
	nameAnalysisOk = (nameAnalysisOk && myFormals->nameAnalysis(symTab));
	nameAnalysisOk = (nameAnalysisOk && myBody->nameAnalysis(symTab));
	//pop scope
	symTab->pop();
	newSym->SetArgsList(myFormals->GetFormalsType());
	return(nameAnalysisOk && symTab->addSym(newSym));
	/*
	needs to be filled in
	*/
	// throw new ToDoError("[DELETE ME] I'm an fnDecl."
	// 	" you should add and make current a new"
	// 	" scope table for my body"
	// );
}

bool FormalsListNode::nameAnalysis(SymbolTable* symTab){
	bool result = true;
	FormalDeclNode* FormalDeclNodeTemp;
	for (std::list<FormalDeclNode *>::iterator it=myFormals->begin(); it!=myFormals->end(); ++it){
		FormalDeclNodeTemp = *it;
		//call NA on current formalDeclNode
		result = FormalDeclNodeTemp->nameAnalysis(symTab) && result;
		//get type of the formalDeclNode that was just processed
		std::string currArgType = FormalDeclNodeTemp->GetType();
		//append the type onto formalsTypeString for later access in FnDeclNode as its argsType;
		formalsTypeString = formalsTypeString + "," + currArgType;
	}
	return result;
}

bool FnBodyNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myStmtList->nameAnalysis(symTab) && result;
	result = myVarDecls->nameAnalysis(symTab) && result;
	return result;
}

bool AssignNode::nameAnalysis(SymbolTable* symTab)
{
	bool result = true;
	result = result && myTgt->nameAnalysis(symTab);
	result = result && mySrc->nameAnalysis(symTab);
	return result;
}

bool CallExpNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myId->nameAnalysis(symTab) && result;
	result = myExpList->nameAnalysis(symTab) && result;
	return result;
}

bool PlusNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool MinusNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool TimesNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool DivideNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool AndNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool OrNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool EqualsNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool NotEqualsNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool LessNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool GreaterNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool LessEqNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool GreaterEqNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myExp1->nameAnalysis(symTab) && result;
	result = myExp2->nameAnalysis(symTab) && result;
	return result;
}

bool AssignStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = myAssign->nameAnalysis(symTab) && result;
	return result;
}

bool PostIncStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myExp->nameAnalysis(symTab);
	return result;
}

bool PostDecStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myExp->nameAnalysis(symTab);
	return result;
}

bool ReadStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myExp->nameAnalysis(symTab);
	return result;
}

bool WriteStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myExp->nameAnalysis(symTab);
	return result;
}

bool IfStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	ScopeTable * top = new ScopeTable();
	symTab->addFront(top);
	result = result && myDecls->nameAnalysis(symTab);
	result = result && myStmts->nameAnalysis(symTab);
	//result = result && myExp->nameAnalysis(symTab);
	symTab->pop();
	return result;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	ScopeTable * ifT = new ScopeTable();
	symTab->addFront(ifT);
	result = result && myDeclsT->nameAnalysis(symTab);
	result = result && myStmtsT->nameAnalysis(symTab);

	symTab->pop();

	ScopeTable * ifF = new ScopeTable();
	symTab->addFront(ifF);
	result = result && myDeclsF->nameAnalysis(symTab);
	result = result && myStmtsF->nameAnalysis(symTab);
	symTab->pop();
	return result;
}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	ScopeTable * top = new ScopeTable();
	symTab->addFront(top);
	result = result && myDecls->nameAnalysis(symTab);
	result = result && myStmts->nameAnalysis(symTab);
	symTab->pop();
	return result;
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myCallExp->nameAnalysis(symTab);
	return result;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab)
{
	bool result = true;
	result = result && myExp->nameAnalysis(symTab);
	return result;
}
}
