/*
    NTU CSIE Compiler 2011 Lab 3
    B97501046 Qing-Cheng Li
    
    parser.cc
    a parser (LL1) for c-------, using Lab2 to find predict set. 
*/
#include "parser.h"

Parser::Parser(char * inputFileName){

    scanner = new Scanner(inputFileName);
    pass = false;
    trace = false;  //default false

    /* Build StringToTokenCode */
    StringToTokenCode["+"] = TC_ADD;
    StringToTokenCode["-"] = TC_SUB;
    StringToTokenCode["*"] = TC_MUT;
    StringToTokenCode["/"] = TC_DIV;
    StringToTokenCode["&&"] = TC_AND;
    StringToTokenCode["||"] = TC_OR;
    StringToTokenCode["!"] = TC_NOT;
    StringToTokenCode["<"] = TC_LT;
    StringToTokenCode[">"] = TC_GT;
    StringToTokenCode["<="] = TC_LE;
    StringToTokenCode[">="] = TC_GE;
    StringToTokenCode["=="] = TC_EQU;
    StringToTokenCode["!="] = TC_NEQ;
    StringToTokenCode["("] = TC_LP;
    StringToTokenCode[")"] = TC_RP;
    StringToTokenCode["["] = TC_LS;
    StringToTokenCode["]"] = TC_RS;
    StringToTokenCode["{"] = TC_LC;
    StringToTokenCode["}"] = TC_RC;
    StringToTokenCode[","] = TC_COM;
    StringToTokenCode[";"] = TC_SEM;
    StringToTokenCode["="] = TC_EQ;
    StringToTokenCode["int"] = TC_INT;
    StringToTokenCode["if"] = TC_IF;
    StringToTokenCode["else"] = TC_ELSE;
    StringToTokenCode["while"] = TC_WHILE;
    StringToTokenCode["break"] = TC_BRK;
    StringToTokenCode["continue"] = TC_CNT;
    StringToTokenCode["scan"] = TC_SCAN;
    StringToTokenCode["print"] = TC_PRINT;
    StringToTokenCode["id"] = TC_ID;
    StringToTokenCode["num"] = TC_NUM;
    StringToTokenCode["string"] = TC_STR;
}

Parser::~Parser(){

}

void Parser::parse(){   //return AST?Lab4?
    if(!scanner->scan()){
        /* scanner has error, default `pass' is false */
        return;
    }else{
        if(Program())
            pass = true;
        return;
    }
}

bool Parser::isPass(){
    return pass;
}

bool Parser::match(string symbol,string rule){

    Token* token = scanner->getNextToken();

    if(token==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next input token (`"<< symbol << "'?) ( Rule: " << rule << " )" << endl;
        return false;
    }

    if(token->getTokenCode()!=StringToTokenCode[symbol]){
        cerr << "Line " << token->getPosition() << " <Parser> Not `" << symbol << "' ( Rule: " << rule << " )"<< token->getTokenCode() << endl;
        return false;
    }

    if(trace)
        cout << "match `" << symbol << "' ( line " << token->getPosition() << " )" << endl; 

    return true;
}

/* Recursive Desent Parsing */

/*
    Program -> BlockStmt
*/
bool Parser::Program(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "<Parser> No input token (maybe the inputfile is NULL)" << endl;
        return false;
    }
    
    switch(peek->getTokenCode()){
        /* 
            Program -> BlockStmt
            Predict:{ 
        */
        case TC_LC:
        
            if(trace)
                cout << "Program -> BlockStmt" << endl;
        
            if(!BlockStmt()){
                return false;
            }
        
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ,no `{' ( Rule: Program -> BlockStmt )" << endl;
            return false;
    }
}

/*
    BlockStmt -> { VarDecl Stmt }
*/
bool Parser::BlockStmt(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next input token ( Rule: BlockStmt -> { VarDecl Stmt } )" << endl;
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            BlockStmt -> { VarDecl Stmt }
            Predict:{
        */
        case TC_LC:
            
            if(trace)
                cout << "BlockStmt -> { VarDecl Stmt }" << endl;
            
            if(!match("{","BlockStmt -> { VarDecl Stmt }")){
                return false;
            }
            
            if(!VarDecl()){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            if(!match("}","BlockStmt -> { VarDecl Stmt }")){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "<Parser> Syntax error ,no `{' ( Rule: BlockStmt -> { VarDecl Stmt } )" << endl;
            return false;
    }
}

/* 
    VarDecl -> int VarDeclNum id VarDeclId ; VarDecl
            -> (null)
*/
bool Parser::VarDecl(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDecl -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            VarDecl -> int VarDeclNum id VarDeclId ; VarDecl
            Predict:int
        */
        case TC_INT:   //`int'
            
            if(trace)
                cout << "VarDecl -> int VarDeclNum id VarDeclId ; VarDecl" << endl;
            
            if(!match("int","VarDecl -> int VarDeclNum id VarDeclId ; VarDecl")){
                return false;
            }
            
            if(!VarDeclNum()){
                return false;
            }
            
            if(!match("id","VarDecl -> int VarDeclNum id VarDeclId ; VarDecl")){
                return false;
            }
            
            if(!VarDeclId()){
                return false;
            }
            
            if(!match(";","VarDecl -> int VarDeclNum id VarDeclId ; VarDecl")){
                return false;
            }
            
            if(!VarDecl()){
                return false;
            }
            
            return true;
        
        /*
            VarDecl -> (null)
            Predict:id break continue if print scan while { }
        */
        case TC_LC:
        case TC_RC:
        case TC_IF:
        case TC_WHILE:
        case TC_BRK:
        case TC_CNT:
        case TC_SCAN:
        case TC_PRINT:
        case TC_ID:
            if(trace)
                cout << "VarDecl -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDecl -> ??? )" << endl;
            return false;
    }
}

/*
    VarDeclNum -> [ num ] VarDeclNum
               -> (null)
*/
bool Parser::VarDeclNum(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDeclNum -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            VarDeclNum -> [ num ] VarDeclNum
            Predict:[  
        */
        case TC_LS:
        
            if(trace)
                cout << "VarDeclNum -> [ num ] VarDeclNum" << endl;
            
            if(!match("[","VarDeclNum -> [ num ] VarDeclNum")){
                return false;
            }
            
            if(!match("num","VarDeclNum -> [ num ] VarDeclNum")){
                return false;
            }
            
            if(!match("]","VarDeclNum -> [ num ] VarDeclNum")){
                return false;
            }
            
            if(!VarDeclNum()){
                return false;
            }
            
            return true;
        /*
            VarDeclNum -> (null)
            Predict:id
        */
        case TC_ID:
            if(trace)
                cout << "VarDeclNum -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDeclNum -> ??? )" << endl;
            return false;
    }
}

/*
    VarDeclId -> , id VarDeclId
              -> (null)
*/
bool Parser::VarDeclId(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDeclId -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            VarDeclId -> , id VarDeclId
            Predict:,
        */
        case TC_COM:
            
            if(trace)
                cout << "VarDeclId -> , id VarDeclId" << endl;
            
            if(!match(",","VarDeclId -> , id VarDeclId")){
                return false;
            }
            
            if(!match("id","VarDeclId -> , id VarDeclId")){
                return false;
            }
            
            if(!VarDeclId()){
                return false;
            }
            
            return true;
        /*
            VarDeclId -> (null)
            Predict:;
        */
        case TC_SEM:
            if(trace)
                cout << "VarDeclId -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDeclId -> ??? )" << endl;
            return false;
    }
}

/*
    Var -> id VarArithExpr
*/
bool Parser::Var(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: Var -> id VarArithExpr )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            Var -> id VarArithExpr
            Predict:id
        */
        case TC_ID:
            
            if(trace)
                cout << "Var -> id VarArithExpr" << endl;
            
            if(!match("id","Var -> id VarArithExpr")){
                return false;
            }
            
            if(!VarArithExpr()){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: Var -> id VarArithExpr )" << endl;
            return false;
    }
}

/*
    VarArithExpr -> [ ArithExpr ]   VarArithExpr
                 -> (null)
*/
bool Parser::VarArithExpr(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: VarArithExpr -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            VarArithExpr -> [ ArithExpr ]   VarArithExpr
            Predict:[
        */
        case TC_LS:
            
            if(trace)
                cout << "VarArithExpr -> [ ArithExpr ]   VarArithExpr" << endl;
            
            if(!match("[","VarArithExpr -> [ ArithExpr ]   VarArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            if(!match("]","VarArithExpr -> [ ArithExpr ]   VarArithExpr")){
                return false;
            }
            
            if(!VarArithExpr()){
                return false;
            }
            
            return true;
        /*
            VarArithExpr -> (null)
            Predict:!= && ) * + , - / ; < <= = == > >= ] ||
        */
        case TC_NEQ:
        case TC_AND:
        case TC_RP:
        case TC_MUT:
        case TC_ADD:
        case TC_COM:
        case TC_SUB:
        case TC_DIV:
        case TC_SEM:
        case TC_LT:
        case TC_LE:
        case TC_EQ:
        case TC_EQU:
        case TC_GT:
        case TC_GE:
        case TC_RS:
        case TC_OR:
            if(trace)
                cout << "VarArithExpr -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarArithExpr -> ??? )" << endl;
            return false;
    }
}

/*
    Stmt -> Var = ArithExpr ; Stmt
         -> if ( LogicExpr ) BlockStmt StmtElse Stmt
         -> while ( LogicExpr ) BlockStmt Stmt
         -> break ; Stmt
         -> continue ; Stmt
         -> scan ( Var StmtVar ) ; Stmt
         -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt
         -> BlockStmt Stmt
         -> (null)
*/
bool Parser::Stmt(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: Stmt -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            Stmt -> Var = ArithExpr ; Stmt
            Predict:id
        */
        case TC_ID:
            
            if(trace)
                cout << "Stmt -> Var = ArithExpr ; Stmt" << endl;
            
            if(!Var()){
                return false;
            }
            
            if(!match("=","Stmt -> Var = ArithExpr ; Stmt")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            if(!match(";","Stmt -> Var = ArithExpr ; Stmt")){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt
            Predict:if
        */
        case TC_IF:
            
            if(trace)
                cout << "Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt" << endl;
            
            if(!match("if","Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt")){
                return false;
            }
            
            if(!match("(","Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt")){
                return false;
            }
            
            if(!LogicExpr()){
                return false;
            }
            
            if(!match(")","Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt")){
                return false;
            }
            
            if(!BlockStmt()){
                return false;
            }
            
            if(!StmtElse()){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> while ( LogicExpr ) BlockStmt Stmt
            Predict:while
        */
        case TC_WHILE:
            
            if(trace)
                cout << "Stmt -> while ( LogicExpr ) BlockStmt Stmt" << endl;
            
            if(!match("while","while ( LogicExpr ) BlockStmt Stmt")){
                return false;
            }
            
            if(!match("(","while ( LogicExpr ) BlockStmt Stmt")){
                return false;
            }
            
            if(!LogicExpr()){
                return false;
            }
            
            if(!match(")","while ( LogicExpr ) BlockStmt Stmt")){
                return false;
            }
            
            if(!BlockStmt()){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> break ; Stmt
            Predict:break
        */
        case TC_BRK:
            
            if(trace)
                cout << "Stmt -> break ; Stmt" << endl;
            
            if(!match("break","Stmt -> break ; Stmt")){
                return false;
            }
            
            if(!match(";","Stmt -> break ; Stmt")){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> continue ; Stmt
            Predict:continue
        */
        case TC_CNT:
            
            if(trace)
                cout << "Stmt -> continue ; Stmt" << endl;
            
            if(!match("continue","Stmt -> continue ; Stmt")){
                return false;
            }
            
            if(!match(";","Stmt -> continue ; Stmt")){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> scan ( Var StmtVar ) ; Stmt
            Predict:scan
        */
        case TC_SCAN:
            
            if(trace)
                cout << "Stmt -> scan ( Var StmtVar ) ; Stmt" << endl;
            
            if(!match("scan","Stmt -> scan ( Var StmtVar ) ; Stmt")){
                return false;
            }
            
            if(!match("(","Stmt -> scan ( Var StmtVar ) ; Stmt")){
                return false;
            }
            
            if(!Var()){
                return false;
            }
            
            if(!StmtVar()){
                return false;
            }
            
            if(!match(")","Stmt -> scan ( Var StmtVar ) ; Stmt")){
                return false;
            }
            
            if(!match(";","Stmt -> scan ( Var StmtVar ) ; Stmt")){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt
            Predict:print
        */
        case TC_PRINT:
        
            if(trace)
                cout << "Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt" << endl;
                
            if(!match("print","Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt")){
                return false;
            }
            
            if(!match("(","Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt")){
                return false;
            }
            
            if(!PrintableExpt()){
                return false;
            }
            
            if(!StmtPrintableExpr()){
                return false;
            }
            
            if(!match(")","Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt")){
                return false;
            }
            
            if(!match(";","Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt")){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> BlockStmt Stmt
            Predict:{
        */
        case TC_LC:
        
            if(trace)
                cout << "Stmt -> BlockStmt Stmt" << endl;
                
            if(!BlockStmt()){
                return false;
            }
            
            if(!Stmt()){
                return false;
            }
            
            return true;
        /*
            Stmt -> (null)
            Predict:}
        */
        case TC_RC:
            if(trace)
                cout << "Stmt -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: Stmt -> ??? )" << endl;
            return false;
    }
}

/*
    StmtElse -> else BlockStmt
             -> (null)
*/
bool Parser::StmtElse(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: StmtElse -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            StmtElse -> else BlockStmt
            Predict:else
        */
        case TC_ELSE:
        
            if(trace)
                cout << "StmtElse -> else BlockStmt" << endl;
                
            if(!match("else","StmtElse -> else BlockStmt")){
                return false;
            }
            
            if(!BlockStmt()){
                return false;
            }
            
            return true;
        /*
            StmtElse -> (null)
            Predict:id break continue if print scan while { }
        */
        case TC_ID:
        case TC_BRK:
        case TC_CNT:
        case TC_IF:
        case TC_PRINT:
        case TC_SCAN:
        case TC_WHILE:
        case TC_LC:
        case TC_RC:
            if(trace)
                cout << "StmtElse -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtElse -> ??? )" << endl;
            return false;
    }
}

/*
    StmtVar -> , Var StmtVar
            -> (null)
*/
bool Parser::StmtVar(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: StmtVar -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            StmtVar -> , Var StmtVar
            Predict:,
        */
        case TC_COM:
            
            if(trace)
                cout << "StmtVar -> , Var StmtVar" << endl;
            
            if(!match(",","StmtVar -> , Var StmtVar")){
                return false;
            }
            
            if(!Var()){
                return false;
            }
            
            if(!StmtVar()){
                return false;
            }
            
            return true;
        /*
            StmtVar -> (null)
            Predict:)
        */
        case TC_RP:
            if(trace)
                cout << "StmtVar -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtVar -> ??? )" << endl;
            return false;
    }
}

/*
    StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr
                      -> (null)
*/
bool Parser::StmtPrintableExpr(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: StmtPrintableExpr -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr
            Predict:,
        */
        case TC_COM:
            
            if(trace)
                cout << "StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr" << endl;
            
            if(!match(",","StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr")){
                return false;
            }
            
            if(!PrintableExpt()){
                return false;
            }
            
            if(!StmtPrintableExpr()){
                return false;
            }
            
            return true;
        /*
            StmtPrintableExpr -> (null)
            Predict:)
        */
        case TC_RP:
            if(trace)
                cout << "StmtPrintableExpr -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtPrintableExpr -> ??? )" << endl;
            return false;
    }
}

/*
    PrintableExpt -> string
                  -> ArithExpr
*/
bool Parser::PrintableExpt(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: PrintableExpt -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            PrintableExpt -> string
            Predict:string
        */
        case TC_STR:
        
            if(trace)
                cout << "PrintableExpt -> string" << endl;
                
            if(!match("string","PrintableExpt -> string")){
                return false;
            }
            
            return true;
        /*
            PrintableExpt -> ArithExpr
            Predict:id num ( + -
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        
            if(trace)
                cout << "PrintableExpt -> ArithExpr" << endl;
                
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: PrintableExpt -> ??? )" << endl;
            return false;
    }
}

/*
    ArithExpr -> ArithExprMD ArithExprASMore
*/
bool Parser::ArithExpr(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExpr -> ArithExprMD ArithExprASMore )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExpr -> ArithExprMD ArithExprASMore
            Predict:id num ( + -
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        
            if(trace)
                cout << "ArithExpr -> ArithExprMD ArithExprASMore" << endl;
        
            if(!ArithExprMD()){
                return false;
            }
            
            if(!ArithExprASMore()){
                return false;
            }
        
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExpr -> ArithExprMD ArithExprASMore )" << endl;
            return false;
    }
}

/*
    ArithExprMD -> ArithExprPN ArithExprMDMore
*/
bool Parser::ArithExprMD(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprMD -> ArithExprPN ArithExprMDMore )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExprMD -> ArithExprPN ArithExprMDMore
            Predict:id num ( + -
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        
            if(trace)
                cout << "ArithExprMD -> ArithExprPN ArithExprMDMore" << endl;
        
            if(!ArithExprPN()){
                return false;
            }
            
            if(!ArithExprMDMore()){
                return false;
            }
        
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprMD -> ArithExprPN ArithExprMDMore )" << endl;
            return false;
    }
}

/*
    ArithExprASMore -> + ArithExprMD ArithExprASMore
                    -> - ArithExprMD ArithExprASMore
                    -> (null)
*/
bool Parser::ArithExprASMore(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprASMore -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExprASMore -> + ArithExprMD ArithExprASMore
            Predict:+
        */
        case TC_ADD:
        
            if(trace)
                cout << "ArithExprASMore -> + ArithExprMD ArithExprASMore" << endl;
        
            if(!match("+","ArithExprASMore -> + ArithExprMD ArithExprASMore")){
                return false;
            }
            
            if(!ArithExprMD()){
                return false;
            }
            
            if(!ArithExprASMore()){
                return false;
            }
        
            return true;
        /*
            ArithExprASMore -> - ArithExprMD ArithExprASMore
            Predict:-
        */
        case TC_SUB:
            
            if(trace)
                cout << "ArithExprASMore -> - ArithExprMD ArithExprASMore" << endl;
            
            if(!match("-","ArithExprASMore -> - ArithExprMD ArithExprASMore")){
                return false;
            }
            
            if(!ArithExprMD()){
                return false;
            }
            
            if(!ArithExprASMore()){
                return false;
            }
            
            return true;
        /*
            ArithExprASMore -> (null)
            Predict:!= && ) , ; < <= == > >= ] ||
        */
        case TC_NEQ:
        case TC_AND:
        case TC_RP:
        case TC_COM:
        case TC_SEM:
        case TC_LT:
        case TC_LE:
        case TC_EQU:
        case TC_GT:
        case TC_GE:
        case TC_RS:
        case TC_OR:
            if(trace)
                cout << "ArithExprASMore -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprASMore -> ??? )" << endl;
            return false;
    }
}

/*
    ArithExprPN -> + ArithExprBase
                -> - ArithExprBase
                -> ArithExprBase
*/
bool Parser::ArithExprPN(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprPN -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExprPN -> + ArithExprBase
            Predict: +
        */
        case TC_ADD:
        
            if(trace)
                cout << "ArithExprPN -> + ArithExprBase" << endl;
        
            if(!match("+","ArithExprPN -> + ArithExprBase")){
                return false;
            }
            
            if(!ArithExprBase()){
                return false;
            }
            
            return true;        
        /*
            ArithExprPN -> - ArithExprBase
            Predict: -
        */
        case TC_SUB:
        
            if(trace)
                cout << "ArithExprPN -> - ArithExprBase" << endl;
        
            if(!match("-","ArithExprPN -> - ArithExprBase")){
                return false;
            }
            
            if(!ArithExprBase()){
                return false;
            }
            
            return true;   
        /*
            ArithExprPN -> ArithExprBase
            Predict: id num (
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
            
            if(trace)
                cout << "ArithExprPN -> ArithExprBase" << endl;

            if(!ArithExprBase()){
                return false;
            }
            
            return true; 
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprPN -> ??? )" << endl;
            return false;
    }
}

/*
    ArithExprBase -> num
                  -> Var
                  -> ( ArithExpr )
*/
bool Parser::ArithExprBase(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprBase -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExprBase -> num
            Predict:num
        */
        case TC_NUM:
            
            if(trace)
                cout << "ArithExprBase -> num" << endl;
            
            if(!match("num","ArithExprBase -> num")){
                return false;
            }
            
            return true;
        /*
            ArithExprBase -> Var
            Predict:id
        */
        case TC_ID:
            
            if(trace)
                cout << "ArithExprBase -> Var" << endl;
            
            if(!Var()){
                return false;
            }
            
            return true;
        /*
            ArithExprBase -> ( ArithExpr )
            Predict:(
        */
        case TC_LP:
            
            if(trace)
                cout << "ArithExprBase -> ( ArithExpr )" << endl;
            
            if(!match("(","ArithExprBase -> ( ArithExpr )")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            if(!match(")","ArithExprBase -> ( ArithExpr )")){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprBase -> ??? )" << endl;
            return false;
    }
}

/*
    ArithExprMDMore -> * ArithExprPN ArithExprMDMore
                    -> / ArithExprPN ArithExprMDMore
                    -> (null)
*/
bool Parser::ArithExprMDMore(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprMDMore -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            ArithExprMDMore -> * ArithExprPN ArithExprMDMore
            Predict:*
        */
        case TC_MUT:
            
            if(trace)
                cout << "ArithExprMDMore -> * ArithExprPN ArithExprMDMore" << endl;
            
            if(!match("*","ArithExprMDMore -> * ArithExprPN ArithExprMDMore")){
                return false;
            }
            
            if(!ArithExprPN()){
                return false;
            }
            
            if(!ArithExprMDMore()){
                return false;
            }
            
            return true;
        /*
            ArithExprMDMore -> / ArithExprPN ArithExprMDMore
            Predict:/
        */
        case TC_DIV:
            
            if(trace)
                cout << "ArithExprMDMore -> / ArithExprPN ArithExprMDMore" << endl;
                
            if(!match("/","ArithExprMDMore -> / ArithExprPN ArithExprMDMore")){
                return false;
            }
            
            if(!ArithExprPN()){
                return false;
            }
            
            if(!ArithExprMDMore()){
                return false;
            }
            
            return true;
        /*
            ArithExprMDMore -> (null)
            Predict:!= && ) + , - ; < <= == > >= ] ||
        */
        case TC_NEQ:
        case TC_AND:
        case TC_RP:
        case TC_ADD:
        case TC_COM:
        case TC_SUB:
        case TC_SEM:
        case TC_LT:
        case TC_LE:
        case TC_EQU:
        case TC_GT:
        case TC_GE:
        case TC_RS:
        case TC_OR:
            if(trace)
                cout << "ArithExprMDMore -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprMDMore -> ??? )" << endl;
            return false;
    }
}

/*
    LogicExpr -> LogicExprAnd LogicExprOrMore
*/
bool Parser::LogicExpr(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExpr -> LogicExprAnd LogicExprOrMore )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExpr -> LogicExprAnd LogicExprOrMore
            Predict:id num ! ( + - [
        */
        case TC_ID:
        case TC_NUM:
        case TC_NOT:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        case TC_LS:
        
            if(trace)
                cout << "LogicExpr -> LogicExprAnd LogicExprOrMore" << endl;
                
            if(!LogicExprAnd()){
                return false;
            }
            
            if(!LogicExprOrMore()){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExpr -> LogicExprAnd LogicExprOrMore )" << endl;
            return false;
    }
}

/*
    LogicExprAnd -> LogicExprTF LogicExprAndMore
*/
bool Parser::LogicExprAnd(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprAnd -> LogicExprTF LogicExprAndMore )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprAnd -> LogicExprBase LogicExprAndMore
            Predict:id num ! ( + - [
        */
        case TC_ID:
        case TC_NUM:
        case TC_NOT:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        case TC_LS:
        
            if(trace)
                cout << "LogicExprAnd -> LogicExprTF LogicExprAndMore" << endl;
                
            if(!LogicExprTF()){
                return false;
            }
            
            if(!LogicExprAndMore()){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprAnd -> LogicExprTF LogicExprAndMore )" << endl;
            return false;
    }
}

/*
    LogicExprOrMore -> || LogicExprAnd LogicExprOrMore
                    -> (null)
*/
bool Parser::LogicExprOrMore(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprOrMore -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprOrMore -> || LogicExprAnd LogicExprOrMore
            Predict:||
        */
        case TC_OR:
            
            if(trace)
                cout << "LogicExprOrMore -> || LogicExprAnd LogicExprOrMore" << endl;
            
            if(!match("||","LogicExprOrMore -> || LogicExprAnd LogicExprOrMore")){
                return false;
            }
            
            if(!LogicExprAnd()){
                return false;
            }
            
            if(!LogicExprOrMore()){
                return false;
            }
            
            return true;
        /*
            LogicExprOrMore -> (null)
            Predict:) ]
        */
        case TC_RP:
        case TC_RS:
            if(trace)
                cout << "LogicExprOrMore -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprOrMore -> ??? )" << endl;
            return false;
    }
}

/*
    LogicExprTF -> ! LogicExprBase
                -> LogicExprBase
*/
bool Parser::LogicExprTF(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule:LogicExprTF -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprTF -> ! LogicExprBase
            Predict:!
        */
        case TC_NOT:
            
            if(trace)
                cout << "LogicExprTF -> ! LogicExprBase" << endl;
            
            if(!match("!","LogicExprTF -> ! LogicExprBase")){
                return false;
            }
            
            if(!LogicExprBase()){
                return false;
            }
                        
            return true;
        /*
            LogicExprTF -> LogicExprBase
            Predict: id num ( + - [
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
        case TC_LS:
        
            if(trace)
                cout << "LogicExprTF -> LogicExprBase" << endl;
            
            if(!LogicExprBase()){
                return false;
            }
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprTF -> ??? )" << endl;
            return false;
    }
}

/*
    LogicExprBase -> ArithExpr LogicExprComp
                  -> [ LogicExpr ]
*/
bool Parser::LogicExprBase(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprBase -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprBase -> ArithExpr LogicExprComp
            Predict:id num ( + -
        */
        case TC_ID:
        case TC_NUM:
        case TC_LP:
        case TC_ADD:
        case TC_SUB:
            
            if(trace)
                cout << "LogicExprBase -> ArithExpr LogicExprComp" << endl;
            
            if(!ArithExpr()){
                return false;
            }
            
            if(!LogicExprComp()){
                return false;
            }
            
            return true;
        /*
            LogicExprBase -> [ LogicExpr ]
            Predict:[
        */
        case TC_LS:
            
            if(trace)
                cout << "LogicExprBase -> [ LogicExpr ]" << endl;
            
            if(!match("[","LogicExprBase -> [ LogicExpr ]")){
                return false;
            }
            
            if(!LogicExpr()){
                return false;
            }
            
            if(!match("]","LogicExprBase -> [ LogicExpr ]")){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprBase -> ??? )" << endl;
            return false;
    }
}

/*
    LogicExprAndMore -> && LogicExprTF LogicExprAndMore
                     -> (null)
*/
bool Parser::LogicExprAndMore(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: LogicExprAndMore -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprAndMore -> && LogicExprTF LogicExprAndMore
            Predict:&&
        */
        case TC_AND:
            
            if(trace)
                cout << "LogicExprAndMore -> && LogicExprTF LogicExprAndMore" << endl;
            
            if(!match("&&","LogicExprAndMore -> && LogicExprTF LogicExprAndMore")){
                return false;
            }
            
            if(!LogicExprTF()){
                return false;
            }
            
            if(!LogicExprAndMore()){
                return false;
            }
            
            return true;
        /*
            LogicExprAndMore -> (null)
            Predict:) ] ||
        */
        case TC_RP:
        case TC_RS:
        case TC_OR:
            if(trace)
                cout << "LogicExprAndMore -> (null)" << endl;
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprAndMore -> ??? )" << endl;
            return false;
    }
}

/*
    LogicExprComp -> > ArithExpr
                  -> >= ArithExpr
                  -> < ArithExpr
                  -> <= ArithExpr
                  -> == ArithExpr
                  -> != ArithExpr
*/
bool Parser::LogicExprComp(){
    
    Token* peek = scanner->peekNextToken();
    if(peek==NULL){
        cerr << "Near line " << scanner->getPreviousToken()->getPosition() <<" <Parser> No next token, cannot predict rule! ( Rule: LogicExprComp -> ??? )";
        return false;
    }
    
    switch(peek->getTokenCode()){
        /*
            LogicExprComp -> > ArithExpr
            Predict:>
        */
        case TC_GT:

            if(trace)
                cout << "LogicExprComp -> > ArithExpr" << endl;
        
            if(!match(">","LogicExprComp -> > ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /*
            LogicExprComp -> >= ArithExpr
            Predict:>=
        */
        case TC_GE:
        
            if(trace)
                cout << "LogicExprComp -> >= ArithExpr" << endl;
        
            if(!match(">=","LogicExprComp -> >= ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /*
            LogicExprComp -> < ArithExpr
            Predict:<
        */
        case TC_LT:
        
            if(trace)
                cout << "LogicExprComp -> < ArithExpr" << endl;
                
            if(!match("<","LogicExprComp -> < ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /*
            LogicExprComp -> <= ArithExpr
            Predict:<=
        */
        case TC_LE:
        
            if(trace)
                cout << "LogicExprComp -> <= ArithExpr" << endl;
        
            if(!match("<=","LogicExprComp -> <= ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /*
            LogicExprComp -> == ArithExpr
            Predict:==
        */
        case TC_EQU:
        
            if(trace)
                cout << "LogicExprComp -> == ArithExpr" << endl;
        
            if(!match("==","LogicExprComp -> == ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /*
            LogicExprComp -> != ArithExpr
            Predict:!=
        */
        case TC_NEQ:
        
            if(trace)
                cout << "LogicExprComp -> != ArithExpr" << endl;
        
            if(!match("!=","LogicExprComp -> != ArithExpr")){
                return false;
            }
            
            if(!ArithExpr()){
                return false;
            }
            
            return true;
        /* Error */
        default:
            cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprComp -> ??? )" << endl;
            return false;
    }
}

