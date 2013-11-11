/*
    NTU CSIE Compiler 2011 Lab 2
    B97501046 Qing-Cheng Li
    
    rules.h
    The header file of object Rules, which used to record the grammar rules.
*/
#ifndef _RULES_H_
#define _RULES_H_
#include <vector>
#include <map>
#include <string>
#include "token.h"

class Rules{
    private:
        static map<string,Rules*> nonTerminal;  //Nonterminal table <nonterminal symbol,pointer of rules>
        static vector<string> lexeme;   //Lexeme table
        static Rules* startSymbol;
        
        string LHS;
        vector< vector<string> > RHS;
        vector< vector<string> > firstSet;  //for each RHS
        vector< vector<string> > predictSet;//for each RHS
        vector<string> followSet;
        
        vector<string> nowRHS;  //tmp RHS
        bool toNull;    //if there is a rule A =*=> (null), then toNull=ture
        
    public:
        Rules(Token* token);
        ~Rules();
        
        void setStartSymbol();
        
        string getLHS();
        
        void newRHS();  //if need to add a new RHS, must call this function first
        void addRHS(Token* token);
        void endRHS();  //call this to end a RHS
        vector< vector<string> > getRHS();
        
        vector<string> getFirstSet();
        
        static void findFollowSet();
        vector<string> getFollowSet();
        
        vector< vector<string> > getPredictSet();
        
        static bool compare(string a,string b); //for sorting the vector<string> to fit output format
};

#endif
