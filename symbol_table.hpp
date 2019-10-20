#ifndef LAKE_SYMBOL_TABLE_HPP
#define LAKE_SYMBOL_TABLE_HPP
#include <string>
#include <iostream>
#include <unordered_map>
#include <list>

//Use an alias template so that we can use
// "HashMap" and it means "std::unordered_map"
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace lake{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols
// exist for the lifetime of a scope in the
// symbol table.
class SemSymbol {
	//TODO add the fields that
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
	private:

		//std::string m_Id;
	protected:
		bool m_isDeclared;
		size_t m_line;
        size_t m_col;
    public:
		//may end up discarding constructor
        SemSymbol(){ }

        virtual size_t GetLine(){ return m_line; }
        virtual size_t GetCol(){ return m_col; }
		virtual void SetLineNum(size_t newLine)=0;
		virtual void SetColumnNum(size_t newColumn)=0;

        virtual bool SetType(std::string type)=0;
        virtual void SetId(std::string Id)=0;
        virtual void SetDeclared(bool isDeclared)=0;
		virtual std::string GetFullType()=0;
		virtual std::string GetType()=0;
		virtual std::string GetId()=0;
		virtual bool GetDeclared()=0;
		virtual char GetRepChar()=0;
};
//added by Z/E
class FuncSemSym : public SemSymbol{
    private:
        std::string m_returnType;
		std::string m_argsList;
		std::string m_name;
		// size_t m_line;
        // size_t m_col;

    public:
        FuncSemSym() : SemSymbol(){ }

		void SetLineNum(size_t newLine){ m_line = newLine; }
		void SetColumnNum(size_t newColumn){ m_col = newColumn; }

		//sets the return type only
		bool SetType(std::string returnType){ m_returnType = returnType; return true; }
		void SetId(std::string Id){ m_name = Id; }
		void SetDeclared(bool isDeclared){ m_isDeclared = isDeclared; }

		//returns (int,int) -> bool format of type as single string
		std::string GetFullType();
		//gets the return type only
		std::string GetType(){ return m_returnType; }
		std::string GetId(){  return m_name; }
		bool GetDeclared(){ return true; }
		char GetRepChar(){ return 'f'; }
		void SetArgsList(std::string argsList){ m_argsList = argsList; }
};
//added by Z/E
class VarSemSym : public SemSymbol{
	private:
		std::string m_type;
		std::string m_name;
		bool m_isDeclared;
		int m_ptrDepth;
	public:
		VarSemSym() : SemSymbol(){ }

		void SetLineNum(size_t newLine){ m_line = newLine; }
		void SetColumnNum(size_t newColumn){ m_col = newColumn; }

		bool SetType(std::string type);
		void SetId(std::string Id){ m_name = Id; }
		void SetDeclared(bool isDeclared){ m_isDeclared = isDeclared; }

		std::string GetFullType(){ return m_type; }
		//returns int -> int -> bool format of type as single string
		std::string GetType(){ return m_type; }
		std::string GetId(){ cout << m_name << endl; return m_name; }
		bool GetDeclared(){ return m_isDeclared; }
		char GetRepChar(){ return 'v'; }
		int GetPtrDepth(){ return m_ptrDepth; }
		void SetPtrDepth(int ptrDepth){ m_ptrDepth = ptrDepth; }
};

//A single scope. The symbol table is broken down into a
// chain of scope tables, and each scope table holds
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
		/*added by Z/E*/
		/*Error#1: only check current scope at top of Symbol Table*/
		bool Insert(SemSymbol* sym);
		bool CheckDeclared(std::string check);
		SemSymbol* GetSem(std::string getThis);
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc.
		void addFront(ScopeTable* newScope);
		//pops front of SymbolTable returning a ScopeTable
		void pop();
		/*Error#2: iterate through list to check all scopes for
				   given string in form of semSym*/
		bool CheckDeclared(std::string check);
		//adds to the front scope in the scope table
		bool addSym(SemSymbol* newSym);
		//gets a semantic symbol from the top scope
		SemSymbol* GetSem(std::string getThis);
	private:
		std::list<ScopeTable *> * scopeTableChain;
};


}

#endif
