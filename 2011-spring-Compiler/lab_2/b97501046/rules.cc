/*
    NTU CSIE Compiler 2011 Lab 2
    B97501046 Qing-Cheng Li
    
    rules.cc
    implementation of class Rules.
*/
#include "rules.h"
#include <iostream>
#include <algorithm>

using namespace std;

/*
    nonTerminal
    Nonterminal table <nonterminal symbol,pointer of rules>
*/
map<string,Rules*> Rules::nonTerminal;

/*
    lexeme
    Lexeme table
*/
vector<string> Rules::lexeme;

/*
    startSymbol
    start symbol of this grammar    
*/
Rules* Rules::startSymbol;

/*
    Rules
    Constructor
*/
Rules::Rules(Token* token){
    toNull = false;
    LHS = token->getAttribute();    //set LHS
    nonTerminal.insert(pair<string,Rules*>(token->getAttribute(),this));    //add it to nonterminal table.
}

/*
    ~Rules
    Destructor
*/
Rules::~Rules(){

}

/*
    setStartSymbol
    set this rule's LHS as start symbol
*/
void Rules::setStartSymbol(){
    startSymbol = this;
}

/*
    getLHS
    return LHS string
*/
string Rules::getLHS(){
    return LHS;
}


/*
    newRHS
    build and new RHS
*/
void Rules::newRHS(){
    nowRHS.clear();
}

/*
    addRHS
    add token (symbol) to RHS
*/
void Rules::addRHS(Token* token){
    if(token!=NULL){
        nowRHS.push_back(token->getAttribute());
        if(token->getTokenCode()==4){
            lexeme.push_back(token->getAttribute());    //if it is a lexeme, add it to lexeme table
        }
    }else{  // (null)
        nowRHS.push_back("(null)");
        toNull = true;
    }
}

/*
    endRHS
    end a RHS
*/
void Rules::endRHS(){
    RHS.push_back(nowRHS);
}

/*
    getRHS
    return RHSs
*/
vector< vector<string> > Rules::getRHS(){
    return RHS;
}

/*
    getFirstSet
    get first set and return it.
    when first time call this function, it will find out first set for each RHS and save it in ``firstSet''
    after that, when call this function, it will return ``firstSet'' of all RHS
*/
vector<string> Rules::getFirstSet(){
    
    vector<string> tmp;
    vector<string> tmp_f;
    vector<string>::iterator it;
    map<string,Rules*>::iterator itn;
    
    if(firstSet.size()==0){ //first time call this function
        for(int i=0;i<RHS.size();i++){  //for each RHS
            if(RHS[i][0]=="(null)"){    //It's a null
                tmp.push_back("(null)");
                firstSet.push_back(tmp);
                tmp.clear();
            }else{
                for(int j=0;j<RHS[i].size();j++){   //look for symbol in this RHS
                    itn = nonTerminal.find(RHS[i][j]);  //is this symbol a nonterminal?
                    if(itn==nonTerminal.end()){     //it's a terminal
                        if(find(tmp.begin(),tmp.end(),RHS[i][j])==tmp.end()){
                            tmp.push_back(RHS[i][j]);   //add it into first set.
                        }
                        firstSet.push_back(tmp);
                        tmp.clear();
                        break;
                    }else{  //It's a nonTerminal
                        if(LHS==RHS[i][j]){     //if this nonterminal is its LHS
                            tmp.push_back("");  //add a special symbol `""' and skip it.
                            if(!toNull){
                                /*  NOTICE
                                    i think if A->CB, C is toNull and B is toNull, Then i have to set A is toNull,
                                    I do it at the next NOTICE comment
                                    (in fact, when we deal with the special symbol `""', if A=*=>(null),
                                     then (null) must in first(A)...)
                                */
                                firstSet.push_back(tmp);
                                tmp.clear();
                                break;
                            }
                            //if nonterminal goto (null), continue
                        }else{  //a nonterminal that != its LHS
                            //Rules* another = itn->second;
                            tmp_f = itn->second->getFirstSet(); //find the nonterminal's first set
                            /* add first(nonterminal) to first set, except (null) */
                            bool null = false;
                            for(int k=0;k<tmp_f.size();k++){
                                if(tmp_f[k]!="(null)"){
                                    if(find(tmp.begin(),tmp.end(),tmp_f[k])==tmp.end()){
                                        tmp.push_back(tmp_f[k]);
                                    }
                                }else{  //(null) in first(nonterminal)
                                    null = true;
                                }    
                            }
                            if(!null){
                                firstSet.push_back(tmp);
                                tmp.clear();
                                break;
                            }else{  //find (null), continue!
                                if(j+1!=RHS[i].size()){
                                    continue;
                                }else{  //last symbol=>(null), add (null) into first set.
                                    if(find(tmp.begin(),tmp.end(),"(null)")==tmp.end()){
                                        tmp.push_back("(null)");
                                    }
                                    firstSet.push_back(tmp);
                                    tmp.clear();
                                    if(!toNull){
                                        /*  NOTICE
                                            if all this rule may =*=> (null), this nonterminal symbol =*=> (null)
                                        */
                                        toNull = true;
                                    }
                                }
                            }
                        }//end if (check nonterminal<->LHS)
                    }//end if(check nonterminal)
                }//end for (j, for RHS[i])
            }//end if
        }//end for (i,for each RHS)
    }// end if
    
    tmp.clear();
    
    /* A->rhs1|rhs2; first(A)=first(rhs1)+first(rhs2) */
    for(int i=0;i<firstSet.size();i++){
        for(int j=0;j<firstSet[i].size();j++){
            if(find(tmp.begin(),tmp.end(),firstSet[i][j])==tmp.end()){
                if(firstSet[i][j]!=""){
                    tmp.push_back(firstSet[i][j]);
                }
            }
        }
    }
    
    /* deal with special symbol `""' */
    for(int i=0;i<firstSet.size();i++){
        it = find(firstSet[i].begin(),firstSet[i].end(),"");    //try to find `""'
        if(it!=firstSet[i].end()){  //find `""'
            do{ //erase all `""' in first(rhsi)
                firstSet[i].erase(it);
                it = find(firstSet[i].begin(),firstSet[i].end(),"");
            }while(it!=firstSet[i].end());
            for(int j=0;j<tmp.size();j++){  //add first(A) to first(rhsi)
                if(find(firstSet[i].begin(),firstSet[i].end(),tmp[j])==firstSet[i].end()){
                    if(tmp[j]!="(null)"){
                        firstSet[i].push_back(tmp[j]);
                    }
                }
            }
        }
    }
    
    return tmp;
}

/*
    findFollowSet
    find all Rule's follow set
    it's a static function, find follow set when first time call it.
    it will save follow set to each Rules object's followSet, so after call this function, 
    followSet->size() will > 0.
*/
void Rules::findFollowSet(){

    vector<string> tmp;
    vector< vector<string> > preFS;
    map<string,Rules*>::iterator itn;
    map<string,Rules*>::iterator itFolSet;
    map<string,Rules*>::iterator itAftFol;
    vector<string>::iterator it;
    Rules* tmpRule;
    
    if(nonTerminal.begin()->second->followSet.size()!=0)
        return; //if it has found follow set, do nothing.
    
    /* Step1. add $ to Start Symbol */
    startSymbol->followSet.push_back("(eof)");
    
    /* Step2. if there is A->aBb, then everything in first(b) except (null) is in follow(B) */
    while(true){
        /* record state of last time */
        preFS.clear();
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){
            tmpRule = itn->second;
            preFS.push_back(tmpRule->followSet);
        }
        
        /* Find B in A->aBb */
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){ //for all nonterminal
            tmpRule = itn->second;
            for(int i=0;i<tmpRule->RHS.size();i++){ //for each RHS
                if(tmpRule->RHS[i][0]=="(null)")    //if A->(null), skip and continue
                    continue;
                for(int j=0;j<tmpRule->RHS[i].size();j++){  //for each symbol in RHS
                    itFolSet = nonTerminal.find(tmpRule->RHS[i][j]);
                    if(itFolSet!=nonTerminal.end()){    //it's a nonterminal
                    
                        /* follow(B) = first(b) except (null) */
                        int next = j;
                        while(++next<tmpRule->RHS[i].size()){
                            itAftFol = nonTerminal.find(tmpRule->RHS[i][next]);    ////is the next symbol a nonterminal?
                            if(itAftFol!=nonTerminal.end()){    //next is also a nonterminal
                                /* follow(B) = first(b) except (null) */
                                tmp = itAftFol->second->getFirstSet();
                                bool hasNull = false;
                                for(int k=0;k<tmp.size();k++){
                                    if(tmp[k]!="(null)"){
                                        if(find(itFolSet->second->followSet.begin(),itFolSet->second->followSet.end(),tmp[k])==itFolSet->second->followSet.end()){
                                            itFolSet->second->followSet.push_back(tmp[k]);
                                        }
                                    }else{
                                        hasNull = true;
                                    }
                                }
                                
                                if(!hasNull)
                                    break;
                                
                            }else{  //next is not a nonterminal.
                                /* follow(B) = first(b) = b */
                                if(find(itFolSet->second->followSet.begin(),itFolSet->second->followSet.end(),tmpRule->RHS[i][next])==itFolSet->second->followSet.end()){
                                    itFolSet->second->followSet.push_back(tmpRule->RHS[i][next]);
                                }
                                break;
                            }
                        }                  
                    }//end if (nonterminal)
                }//end for (j,each symbol in RHS[i])
            }//end for (i,each RHS)
        }//end for (itn,each nonterminal)
        
        /* Compare last time and this time */
        int tmp_i = 0;
        bool breakCond = true;
        
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){
            tmpRule = itn->second;
            /*
            cout << tmpRule->LHS << ":[ ";
            for(int i=0;i<preFS[tmp_i].size();i++){
                cout << preFS[tmp_i][i] << " ";
            }
            cout << "]=>[ ";
            for(int i=0;i<tmpRule->followSet.size();i++){
                cout << tmpRule->followSet[i] << " ";
            }
            cout << " ]" << endl;
            */
            if(preFS[tmp_i++]!=tmpRule->followSet){
                breakCond = false;
                break;
            }
        }        
        if(breakCond)break; //if breakCond = true, break!
    }
    
    /* Step3. if there is A->aB or A->aBb, where (null) in first(b), then everything in follow(A) is in follow(B) */
    while(true){
        /* record state of last time */
        preFS.clear();
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){
            tmpRule = itn->second;
            preFS.push_back(tmpRule->followSet);
        }
        
        /* Find B in A->aB or A->aB(null) */
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){ //for each nonterminal
            tmpRule = itn->second;
            for(int i=0;i<tmpRule->RHS.size();i++){ //for each RHS
                if(tmpRule->RHS[i][0]=="(null)")    //if A->(null), skip and continue
                    continue;
                for(int j=tmpRule->RHS[i].size()-1;j>=0;j--){   //search (null)/nonterminal from end of RHS[i]
                    itFolSet = nonTerminal.find(tmpRule->RHS[i][j]);
                    if(itFolSet!=nonTerminal.end()){    //it's a nonTerminal (in the end of RHS[i])
                        /* follow(B) += follow(A) */
                        for(int k=0;k<tmpRule->followSet.size();k++){
                            if(find(itFolSet->second->followSet.begin(),itFolSet->second->followSet.end(),tmpRule->followSet[k])==itFolSet->second->followSet.end()){
                                itFolSet->second->followSet.push_back(tmpRule->followSet[k]);
                            }
                        }
                        /* check first(B) */
                        tmp.clear();
                        tmp = itFolSet->second->getFirstSet();
                        if(find(tmp.begin(),tmp.end(),"(null)")!=tmp.end()){    //there is (null)
                            continue;   //if first(B) has (null), continue
                        }else{
                            break;
                        }   
                    }else{
                        break;
                    }
                }//end for (j, RHS[i])
            }//end for (i, RHS)
        }//end for (itn, nonterminal)
        
        /* Compare last time and this time */
        int tmp_i = 0;
        bool breakCond = true;
        
        for(itn=nonTerminal.begin() ; itn != nonTerminal.end(); itn++){
            tmpRule = itn->second;
            /*
            cout << tmpRule->LHS << ":[ ";
            for(int i=0;i<preFS[tmp_i].size();i++){
                cout << preFS[tmp_i][i] << " ";
            }
            cout << "]=>[ ";
            for(int i=0;i<tmpRule->followSet.size();i++){
                cout << tmpRule->followSet[i] << " ";
            }
            cout << " ]" << endl;
            */
            if(preFS[tmp_i++]!=tmpRule->followSet){
                breakCond = false;
                break;
            }
        }
        if(breakCond)break; //if breakCond = true, break!
    }   
    
}

/*
    getFollowSet
    return follow set.
*/
vector<string> Rules::getFollowSet(){
    return followSet;
}

/*
    getPredictSet
    return predict set (for each RHS)
    if first(RHS[i]) has no (null), predict(RHS[i]) = first(RHS[i])
    if has (null), predict(RHS[i]) = first(RHS[i]) - (null) + follow(LHS)
*/
vector< vector<string> > Rules::getPredictSet(){
    
    vector<string> tmp;
    
    predictSet.clear();
    
    for(int i=0;i<firstSet.size();i++){ //for each RHS (firstSet's order = RHS)
        if(find(firstSet[i].begin(),firstSet[i].end(),"(null)")==firstSet[i].end()){
            //if first(RHS[i]) has no (null), predict(RHS[i]) = first(RHS[i])
            predictSet.push_back(firstSet[i]);
        }else{
            //if has (null), predict(RHS[i]) = first(RHS[i]) - (null) + follow(LHS)
            tmp.clear();
            for(int j=0;j<firstSet[i].size();j++){  //predict(RHS[i]) = first(RHS[i]) - (null)
                if(firstSet[i][j]!="(null)"&&find(tmp.begin(),tmp.end(),firstSet[i][j])==tmp.end()){
                    tmp.push_back(firstSet[i][j]);
                }
            }
            for(int j=0;j<followSet.size();j++){    //predict(RHS[i]) += follow(LHS)
                if(find(tmp.begin(),tmp.end(),followSet[j])==tmp.end()){
                    tmp.push_back(followSet[j]);
                }
            }
            predictSet.push_back(tmp);
        }
    }
    return predictSet;
}

/*
    compare
    compare function for sort
    (null)<(eof)<token_class<lexeme
*/
bool Rules::compare(string a,string b){
        
    if(a=="(null)"){
        return true;
    }else if(a=="(eof)"){
        if(b=="(null)"){
            return false;
        }else{
            return true;
        }
    }else if(b=="(null)"||b=="(eof)"){
        return false;
    }
    
    if(find(lexeme.begin(),lexeme.end(),a)==lexeme.end()){
        if(find(lexeme.begin(),lexeme.end(),b)==lexeme.end()){
            return a<b; //a,b is not in lexeme
        }else{
            return true;    //b in lexeme
        }
    }else{
        if(find(lexeme.begin(),lexeme.end(),b)==lexeme.end()){
            return false;   //a in lexeme
        }else{
            return a<b; //a,b in lexeme
        }
    }
    return a<b; //unknow orz
}
