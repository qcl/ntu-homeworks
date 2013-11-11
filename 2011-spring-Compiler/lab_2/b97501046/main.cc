/*
    NTU CSIE Compiler 2011 Lab 2
    B97501046 Qing-Cheng Li
    
    main.cc
    The main program of lab 2.
*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include "token.h"
#include "scanner.h"
#include "rules.h"

using namespace std;

int main(int argc,char * argv[]){
    
    char * inputFileName;
    char * outputFileName;
    ofstream fout;
    
    if(argc!=3){
        cout << "usage: main [input file name] [output file name]" << endl;
        return 0;
    }
    
    inputFileName = argv[1];
    outputFileName = argv[2];
    
    Scanner* scanner = new Scanner(inputFileName);
    Token* token;
    Token* preToken;
    Token* nonToken;
    int tokenCode;
    string LHS;
    Rules* rule;
    bool isStartSymbol;
    vector<Rules*> allRules;
    vector<string> firstSet;
    vector<string> followSet;
    vector< vector<string> > RHS;
    vector< vector<string> > predictSet;
    
    fout.open(outputFileName);
    
    /* parse input file and build Rules */
    preToken = NULL;
    nonToken = NULL;
    isStartSymbol = true;
    while(!scanner->eof()){
        token = scanner->getNextToken();
        if(token!=NULL&&token->getTokenCode()==5){  //id
            preToken = token;
            token = scanner->getNextToken();
            if(token!=NULL&&token->getTokenCode()==1){  //:
                nonToken = preToken;    //assign nonterminal token
                preToken = token;
                token = scanner->getNextToken();
                tokenCode = token->getTokenCode();
                rule = new Rules(nonToken); //build new rule
                if(isStartSymbol){
                    rule->setStartSymbol();
                    isStartSymbol = false;
                }
                rule->newRHS(); //start a new RHS
                if(token!=NULL&&tokenCode!=1){  //!=`:'
                    while(true){
                        if(tokenCode==2){   //`|'
                            if(preToken->getTokenCode()==1||preToken->getTokenCode()==2 ){   // `:' or `|'
                                rule->addRHS(NULL);
                            }
                            rule->endRHS();
                            rule->newRHS();
                        }else if(tokenCode==3){ //`;'
                            if(preToken->getTokenCode()==1||preToken->getTokenCode()==2 ){   // `:' or `|'
                                rule->addRHS(NULL);
                            }
                            rule->endRHS();
                            allRules.push_back(rule);
                            break;
                        }else{  //id and str
                            rule->addRHS(token);
                        }
                        preToken = token;
                        token = scanner->getNextToken();
                        tokenCode = token->getTokenCode();
                    }//end while
                }//end if (!=:)   
            }//end if (==:)
        }//end if(id)
    }//end while

    /* Output result*/
    fout << " LHS , RHS , FIRST , FOLLOW , PREDICT " << endl;
    
    Rules::findFollowSet(); //build follow set for each Rules
    
    for(int i=0;i<allRules.size();i++){ //for each Rules
        
        LHS = allRules[i]->getLHS();
        RHS = allRules[i]->getRHS();
        firstSet = allRules[i]->getFirstSet();
        followSet = allRules[i]->getFollowSet();
        predictSet = allRules[i]->getPredictSet();
        
        /* Output LHS */
        fout << "\" " << LHS << " \",\" ";
        
        /* Output RHS[0] */
        for(int j=0;j<RHS[0].size();j++){
            fout << RHS[0][j] << " ";
        }
        fout << "\",\" ";
        
        /* Output first(LHS) */
        sort(firstSet.begin(),firstSet.end(),Rules::compare);
        for(int j=0;j<firstSet.size();j++){
            fout << firstSet[j] << " ";
        }
        fout << "\",\" ";
        
        /* Output follow(LHS) */
        sort(followSet.begin(),followSet.end(),Rules::compare);
        for(int j=0;j<followSet.size();j++){
            fout << followSet[j] << " ";
        }
        fout << "\",\" ";
        
        /* Output predict(RHS[0]) */
        sort(predictSet[0].begin(),predictSet[0].end(),Rules::compare);
        for(int j=0;j<predictSet[0].size();j++){
            fout << predictSet[0][j] << " ";
        }
        fout << "\"" << endl;
        
        /* If there are more than one RHS */
        for(int j=1;j<RHS.size();j++){
            
            /* Output RHS[j] */
            fout << ",\" ";
            for(int k=0;k<RHS[j].size();k++){
                fout << RHS[j][k] << " ";
            }
            fout << "\",,,\" ";
            
            /* Output predict(RHS[j]) */
            sort(predictSet[j].begin(),predictSet[j].end(),Rules::compare);
            for(int k=0;k<predictSet[j].size();k++){
                fout << predictSet[j][k] << " ";
            }
            fout << "\"" << endl;
            
        }
    }
    
    fout.close();
    
    return 0;
}

