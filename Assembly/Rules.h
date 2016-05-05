//
//  Rules.h
//  Syntax_Analysis
//
//  Created by SYJ on 4/3/16.
//  Copyright © 2016 SunYijie. All rights reserved.
//

#ifndef Rules_h
#define Rules_h

#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
using namespace std;


void OptFunctionDefinitions();
void Dollars();
void OptDeclarationList();
void StatementList();
void FunctionDefinitions();
void FDPrime();
void OptParameterList();
void Function();
void ParameterList();
void PLPrime();
void Parameter();
void Qualifier();
void Body();
void OptDeclarationList();
void DeclarationList();
void DLPrime();
void Declaration();
void IDs();
void IDsPrime();
void StatementList();
void SLPrime();
void Statement();
void Compound();
void Assign();
void If();
void IfPrime();
void Identifier();
void Return();
void ReturnPrime();
void Write();
void Read();
void While();
void Condition();
void Relop();
void Expression();
void Real();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();
void PrimaryPrime();
void Empty();
void Rat16S();
void ERROR();
void Integer();




class Parser{
private:
    fstream inFile, outFile;
    struct token TOKEN;
    ostream& OS = outFile;
    bool SyntaxOut = false;
    
public:
    void printTOKEN(){
        int WIDTH = 15;
        int BEGINING = 8;
        
        if(SyntaxOut)
            outFile << left << "Token: " << setw(WIDTH) << TOKEN.type << "       Lexeme: " << TOKEN.content << endl;
    }
    void putback(){
        for (int i = (TOKEN.content.length()-1); i!=-1; i--) {
            inFile.putback(TOKEN.content[i]);
        }
    }
    
    void TakeToken(){
        TOKEN = lexer(inFile, outFile);
        if(TOKEN.content =="" && !inFile.eof()){
            TakeToken();
        }
        //cout << TOKEN.type << "::::" << TOKEN.content << endl;
        
    }
    
    
    
    Parser(string infile, string outfile){
        inFile.open(infile);
        outFile.open(outfile, std::ios::out);
    }
    
    void PrintParseInfo(ostream& OS, string lhs, string rhs){
        int WIDTH =8;
        if(SyntaxOut)
            OS <<setw(WIDTH)<< "" << lhs << " -> " << rhs << endl;
        /*cout <<setw(WIDTH)<< "" << lhs << " -> " << rhs << endl;*/
    }
    void Rat16S(){
        OptFunctionDefinitions();
        TakeToken();
        if(TOKEN.content == "$$"){
            printTOKEN();
            OptDeclarationList();
            TakeToken();
            if(TOKEN.content == "$$"){
                printTOKEN();
                StatementList();
                TakeToken();
                if(TOKEN.content == "$$"){
                    printTOKEN();
                    //OS<<endl<<endl<<"###################END###################"<<endl;
                }else{
                    ERROR();
                }
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    
    
    
    void OptFunctionDefinitions(){
        TakeToken();
        if(TOKEN.content == "function"){
            putback();
            PrintParseInfo(OS, "<Opt Function Definitions>", "<Function Defination>");
            FunctionDefinitions();
            
        }else{
            putback();
            PrintParseInfo(OS, "<Opt Function Definitions>", "<Empty>");
        }
        
    }
    
    
    void FunctionDefinitions(){
        PrintParseInfo(OS, "<Function Definitions>", "<Function> <FDPrime>");
        Function();
        FDPrime();
    }
    
    void Function(){
        PrintParseInfo(OS, "<Function>", "function <Identifier> ( Opt Parameter List> ) <Opt Declaration List> <Body>");
        TakeToken();
        if(TOKEN.content == "function"){
            printTOKEN();
            TakeToken();
            if(TOKEN.type == "identifier"){
                putback();
                Identifier();
            }else{
                ERROR();
            }
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                OptParameterList();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    OptDeclarationList();
                    Body();
                }else{
                    ERROR();
                }
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    void FDPrime(){
        TakeToken();
        if(TOKEN.content == "function"){
            printTOKEN();
            putback();
            PrintParseInfo(OS, "<FDPrime>", "<Function Definitions>");
            FunctionDefinitions();
        }else{
            putback();
            PrintParseInfo(OS, "<FDPrime>", "<Empty>");
        }
        
    }
    
    
    
    
    
    void OptParameterList(){
        TakeToken();
        if(TOKEN.type == "identifier"){
            putback();
            PrintParseInfo(OS, "<Opt Parameter List>", "<Parameter List>");
            ParameterList();
        }else{
            putback();
            PrintParseInfo(OS, "<Opt Parameter List>", "<Empty>");
        }
    }
    
    void ParameterList(){
        PrintParseInfo(OS, "<Parameter List>", "<Parameter> <PLPrime>");
        Parameter();
        PLPrime();
    }
    
    void PLPrime(){
        TakeToken();
        if(TOKEN.content == ","){
            printTOKEN();
            PrintParseInfo(OS, "<PL Prime>", "<ParameterList>");
            ParameterList();
        }else{
            putback();
            PrintParseInfo(OS, "<PL Prime>", "<Empty>");
        }
    }
    
    void Parameter(){
        PrintParseInfo(OS, "<Parameter>", "<IDs> : <Qualifier>");
        IDs();
        TakeToken();
        if(TOKEN.content == ":"){
            printTOKEN();
            Qualifier();
        }else{
            ERROR();
        }
        
    }
    
    void Qualifier(){
        TakeToken();
        printTOKEN();
        if(TOKEN.content == "integer"){
            PrintParseInfo(OS, "<Qualifier>", "integer");
        }else if(TOKEN.content == "boolean"){
            PrintParseInfo(OS, "<Qualifier>", "boolean");
        }else if(TOKEN.content == "real"){
            PrintParseInfo(OS, "<Qualifier>", "real");
        }else{
            ERROR();
        }
    }
    
    void Body(){
        PrintParseInfo(OS, "<Body>", "{ <Statement List> }");
        TakeToken();
        if(TOKEN.content == "{"){
            printTOKEN();
            StatementList();
            TakeToken();
            printTOKEN();
            if(TOKEN.content != "}"){
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    void OptDeclarationList(){
        TakeToken();
        if(TOKEN.content =="integer" || TOKEN.content == "boolean" || TOKEN.content == "real"){
            printTOKEN();
            putback();
            PrintParseInfo(OS, "<Opt Declaration List>", "<Declaration List>");
            DeclarationList();
        }else{
            putback();
            PrintParseInfo(OS, "<Opt Declaration List>", "<Empty>");
        }
        
    }
    void DeclarationList(){
        PrintParseInfo(OS, "<DeclarationList>", "<Declaration>;<DLPrime>");
        Declaration();
        TakeToken();
        if(TOKEN.content == ";"){
            printTOKEN();
            DLPrime();
        }else{
            ERROR();
        }
    }
    
    void DLPrime(){
        TakeToken();
        if(TOKEN.content == "integer" || TOKEN.content == "boolean" || TOKEN.content == "real"){
            putback();
            PrintParseInfo(OS, "<DLPrime>", "<DeclartionList>");
            DeclarationList();
        }else{
            putback();
            PrintParseInfo(OS, "<DLPrime>", "<Empty>");
        }
        
    }
    
    
    void Declaration(){
        PrintParseInfo(OS, "<Declaration>", "<Qualifier> <IDs>");
        Qualifier();
        IDs();
    }
    
    void IDs(){
        PrintParseInfo(OS, "<IDs>", "<Identifier> <IDsPrime>");
        Identifier();
        IDsPrime();
        
    }
    void IDsPrime(){
        TakeToken();
        if(TOKEN.content == ","){
            printTOKEN();
            PrintParseInfo(OS, "<IDsPrime>", "<IDs>");
            IDs();
        }else{
            putback();
            PrintParseInfo(OS, "<IDsPrime>", "<Empty>");
        }
        
    }
    
    
    
    void StatementList(){
        PrintParseInfo(OS, "<StatementList>", "<Statement><SLPrime>");
        Statement();
        SLPrime();
    }
    
    void SLPrime(){
        TakeToken();
        
        if(TOKEN.content == "{" || TOKEN.type == "identifier" || TOKEN.content == "if" || TOKEN.content == "return" || TOKEN.content == "printf" || TOKEN.content == "scanf" || TOKEN.content == "while"){
            putback();
            PrintParseInfo(OS, "<SLPrime>", "<Statement List>");
            StatementList();
            
        }else{
            putback();
            PrintParseInfo(OS, "<SLPrime>", "<Empty>");
        }
        
    }
    
    
    void Statement(){
        TakeToken();
        if(TOKEN.content == "{"){  printTOKEN();
            putback();
            PrintParseInfo(OS,  "<Statement>", "<Compound>");
            Compound();
        }
        else if(TOKEN.type == "identifier"){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<Assign>");
            Assign();
        }
        else if(TOKEN.content == "if" ){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<If>");
            If();
        }
        else if(TOKEN.content == "return"){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<Return>");
            Return();
        }
        else if(TOKEN.content == "printf"){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<Write>");
            Write();
        }
        else if(TOKEN.content == "scanf"){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<Read>");
            Read();
        }
        else if(TOKEN.content == "while"){
            putback();
            PrintParseInfo(OS,  "<Statement>", "<While>");
            While();
        }
    }
    
    void Compound(){
        PrintParseInfo(OS, "<Compound>", "{<StatementList>}");
        TakeToken();
        if(TOKEN.content == "{"){  printTOKEN();
            printTOKEN();
            StatementList();
            TakeToken();
            printTOKEN();
            if(TOKEN.content != "}"){
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    void  Assign(){
        PrintParseInfo(OS, "<Assign>", "<Identifier> := <Expression>;");
        TakeToken();
        if(TOKEN.type == "identifier"){
            putback();
            Identifier();
            TakeToken();
            if(TOKEN.content ==":="){
                printTOKEN();
                Expression();
                TakeToken();
                printTOKEN();
                if(TOKEN.content != ";"){
                    ERROR();
                }
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
    }
    
    void  If(){
        PrintParseInfo(OS, "<If>", "if (<Condition>) <Statement> <IfPrime");
        TakeToken();
        if(TOKEN.content == "if"){
            printTOKEN();
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                Condition();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    Statement();
                    IfPrime();
                }else{
                    ERROR();
                }
            }else{
                ERROR();
            }
            
        }else{
            ERROR();
        }
        
    }
    
    
    void  IfPrime(){
        TakeToken();
        if(TOKEN.content =="endif"){
            printTOKEN();
            PrintParseInfo(OS, "<IFPrime>", TOKEN.content);
        }
        else if(TOKEN.content == "else"){
            printTOKEN();
            PrintParseInfo(OS, "<IFPrime>", TOKEN.content + "<Statement> endif");
            Statement();
            TakeToken();
            printTOKEN();
            if(TOKEN.content !="endif"){
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    void  Return(){
        TakeToken();
        if(TOKEN.content == "return"){
            printTOKEN();
            PrintParseInfo(OS, "<Return", "return <Return Prime>");
            ReturnPrime();
        }else{
            ERROR();
        }
    }
    
    
    void  ReturnPrime(){
        TakeToken();
        if(TOKEN.content == ";"){
            printTOKEN();
            PrintParseInfo(OS, "<Return Prime>", ";");
        }else if(TOKEN.content == "-" || TOKEN.type == "identifier" || TOKEN.type == "integer" || TOKEN.content == "(" || TOKEN.type == "real" || TOKEN.content == "true" || TOKEN.content =="false"){
            putback();
            PrintParseInfo(OS, "<Return Prime>", "<Expression>;");
            Expression();
            TakeToken();
            if(TOKEN.content != ";"){
                ERROR();
            }
        }
    }
    
    void  Write(){
        PrintParseInfo(OS, "<Write>", "printf ( <Expression> );");
        TakeToken();
        if(TOKEN.content == "printf"){
            printTOKEN();
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                Expression();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    TakeToken();
                    if(TOKEN.content == ";"){
                        printTOKEN();
                    }else{
                        ERROR();
                    }
                }else{
                    ERROR();
                }
                
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    
    
    void  Read(){
        PrintParseInfo(OS, "<Read>", "scanf <IDs>");
        TakeToken();
        if(TOKEN.content == "scanf"){
            printTOKEN();
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                IDs();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    TakeToken();
                    printTOKEN();
                    if(TOKEN.content != ";"){
                        ERROR();
                    }
                }else{
                    ERROR();
                }
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
        
    }
    
    
    void  While(){
        PrintParseInfo(OS, "<While>", "while( <Condition> ) <Statement>");
        TakeToken();
        if(TOKEN.content == "while"){
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                Condition();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    Statement();
                }else{
                    ERROR();
                }
            }else{
                ERROR();
            }
        }else{
            ERROR();
        }
    }
    
    
    
    
    void  Condition(){
        PrintParseInfo(OS, "<Condition>", "<Expression> <Relop> <Expression>");
        Expression();
        Relop();
        Expression();
    }
    
    void  Relop(){
        TakeToken();
        PrintParseInfo(OS, "<Relop>", TOKEN.type);
        printTOKEN();
        /*
         if(TOKEN.content == "="){
         return;
         }else if(TOKEN.content == "/="){
         return;
         }else if(TOKEN.content == ">"){
         return;
         }else if(TOKEN.content == "<"){
         return;
         }else if(TOKEN.content == "=>"){
         return;
         }else if(TOKEN.content == "<="){
         return;
         }
         */
        
    }
    
    void  Expression(){
        PrintParseInfo(outFile,"<Expression>","<Term> <Expression Prime>");
        Term();
        ExpressionPrime();
    }
    
    
    void  ExpressionPrime(){
        TakeToken();
        if(TOKEN.content == "+" ||TOKEN.content == "-" ){
            printTOKEN();
            PrintParseInfo(OS, "<Expression Prime>", TOKEN.content +" <Term> <Expression Prime>");
            Term();
            ExpressionPrime();
        }else{
            putback();
            PrintParseInfo(OS, "<Expression Prime>", "<Empty>");
        }
    }
    
    void  Term(){
        PrintParseInfo(OS, "<Term>","<Factor> <Term Prime>");
        Factor();
        TermPrime();
    }
    
    void  TermPrime (){
        TakeToken();
        if(TOKEN.content == "*" ||TOKEN.content == "/" ){
            printTOKEN();
            PrintParseInfo(OS, "<Term Prime>", TOKEN.content+" <Factor> <Term Prime>");
            Factor();
            TermPrime();
        }else{
            putback();
            PrintParseInfo(OS, "<Term Prime>", "<Empty>");
        }
    }
    
    
    void  Factor(){
        TakeToken();
        if(TOKEN.content == "-"){
            printTOKEN();
            PrintParseInfo(OS, "<Factor>", "-<Primary>");
            Primary();
        }
        putback();
        PrintParseInfo(OS, "<Factor>", "<Primary>");
        Primary();
    }
    
    void  Primary(){
        TakeToken();
        
        if(TOKEN.type == "identifier"){
            putback();
            PrintParseInfo(OS, "<Primary>", "<Identifier>");
            Identifier();
            PrimaryPrime();
        }else if(TOKEN.type == "Integer"){
            putback();
            PrintParseInfo(OS, "<Primary>", "<Integer>");
            Integer();
        }else if(TOKEN.content =="("){
            PrintParseInfo(OS, "<Primary>", "(<Expression>)");
            Expression();
            TakeToken();
            if(TOKEN.content == ")"){
                printTOKEN();
                return;
            }else{
                ERROR();
            }
        }else if(TOKEN.type == "Real"){
            putback();
            PrintParseInfo(OS, "<Primary>", "<Real>");
            Real();
        }else if(TOKEN.content == "true" || TOKEN.content == "false"){
            PrintParseInfo(OS, "<Primary>", TOKEN.content);
            return;
        }
        
    }
    void PrimaryPrime(){
        TakeToken();
        if(TOKEN.content == "("){
            printTOKEN();
            PrintParseInfo(OS, "<Primary Prime>", "( <IDs> )");
            IDs();
            TakeToken();
            printTOKEN();
            if(TOKEN.content != ")"){
                ERROR();
            }
        }else{
            putback();
            PrintParseInfo(OS, "<Primary Prime>", "<Empty>");
        }
        
    }
    void Identifier(){
        TakeToken();
        PrintParseInfo(OS, "<Identifier>", TOKEN.content);
        printTOKEN();
        return;
    }
    
    void Real(){
        TakeToken();
        PrintParseInfo(OS, "<Real>", TOKEN.content);
        printTOKEN();
        return;
    }
    
    void Integer(){
        TakeToken();
        PrintParseInfo(OS, "<Integer>", TOKEN.content);
        printTOKEN();
        return;
    }
    
    void Empty(){
        return;
    }
    
    void ERROR(){
        PrintParseInfo(OS, "<ERROR>", "TOKEN TYPE: "+ TOKEN.type + "   TOKEN Content: " + TOKEN.content);
        exit(-1);
    }
    
    
};





#endif /*Rules_h*/
