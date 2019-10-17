#ifndef LAKE_SYMBOL_TABLE_HPP
#define LAKE_SYMBOL_TABLE_HPP
#include <string>
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
        size_t m_line;
        size_t m_col;
        bool m_isDeclared;
		std::string m_Id;

    public:
		//may end up discarding constructor
        SemSymbol(/*may need params*/);
        virtual ~SemSymbol(){}

        virtual size_t GetLine(){ return m_line; }
        virtual size_t GetCol(){ return m_col; }
        virtual bool SetType(std::string type) = 0;
        virtual void SetId(std::string Id);
        virtual void SetDeclared(bool isDeclared);
		virtual std::string GetType();
		virtual std::string GetId();
		virtual bool GetDeclared();
};
//added by Z/E
class FuncSemSym : SemSymbol{
    private:
        std::string m_returnType;
		std::list<std::string>* m_argsList;
		std::string m_name;

    public:
        FuncSemSym() : SemSymbol(){ }
		void SetArgsList(std::list<std::string>* argsList);
		//sets the RETURN TYPE ONLY
		bool SetType(std::string returnType);
		void SetId(std::string Id);
		void SetLineNum(size_t newLine) { m_line = newLine; }
		void SetColumnNum(size_t newColumn) { m_col = newColumn; }
		//returns int -> int -> bool format of type as single string
		std::string GetType() { return m_returnType; }
		std::string GetId() { return m_name; }
		bool GetDeclared() { return true; }
};
//added by Z/E
class VarSemSym : SemSymbol{
	private:
		std::string m_type;
		std::string m_name;
		bool m_isDeclared;
	public:
		VarSemSym() : SemSymbol(){ }
		bool SetType(std::string type);
		void SetId(std::string Id);
		void SetDeclared(bool isDeclared);
		void SetLineNum(size_t newLine) { m_line = newLine; }
		void SetColumnNum(size_t newColumn) { m_col = newColumn; }
		//returns int -> int -> bool format of type as single string
		std::string GetType(){ return m_type; }
		std::string GetId(){ return m_name; }
		bool GetDeclared(){ return m_isDeclared; }
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
		void Insert(SemSymbol* sym);
		bool CheckDeclared(std::string check);
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
		ScopeTable* pop();
		/*Error#2: iterate through list to check all scopes for
				   given string in form of semSym*/
		bool CheckDeclared(std::string check);
	private:
		std::list<ScopeTable *> * scopeTableChain;
		bool nameAnalyisFail;
};


}

#endif
