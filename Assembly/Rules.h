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
#include <stack>
#include "lexer.h"
#include "Assembly.h"

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



vector<INSTR> INSTR_TABLE;
vector<SYMBOL> Symbol_Table;
stack<int> jumpstack;
bool OpSign = false;
bool Declarating = true;
string DataType;

int Instr_Addr = 1;
int Memory_Location = 5000;
void gen_instr(string OP, int oprnd){
    int size = INSTR_TABLE.size();
    if(oprnd == -999){
        OpSign = true;
    }
    

    if(OpSign && INSTR_TABLE[size-1].OP == "JUMPZ"){
        INSTR Jumpz = INSTR_TABLE[size-1];
        INSTR_TABLE.pop_back();
        INSTR Op = INSTR_TABLE[size-2];
        INSTR_TABLE.pop_back();
        INSTR_TABLE.push_back(INSTR(Instr_Addr,OP,oprnd));
        INSTR_TABLE.push_back(Op);
        INSTR_TABLE.push_back(Jumpz);
        OpSign = false;
    }else{
        INSTR_TABLE.push_back(INSTR(Instr_Addr,OP,oprnd));
    }
    Instr_Addr++;
}


void display_Instr(){
    for (int i = 1; i<=INSTR_TABLE.size()-1; i++){
        cout <<setw(5) << i;
        INSTR_TABLE[i].display();
    }
}

void display_Symbol(){
    for (int i = 0; i<=Symbol_Table.size()-1; i++){
        Symbol_Table[i].display();
    }
}


void gen_SymbolTable(token Declaration){
    for (vector<SYMBOL>::iterator it = Symbol_Table.begin(); it != Symbol_Table.end(); ++it){
        if(Declaration.content == it->Ident){
            return;
        }
    }
    Symbol_Table.push_back(SYMBOL(Declaration.content,Memory_Location++,DataType));
}


int get_address(token token){
    for (vector<SYMBOL>::iterator it = Symbol_Table.begin(); it != Symbol_Table.end(); ++it){
        if(token.content == it->Ident){
            return it->MemoryLoc;
        }
    }
    cout << "Does not exist" << endl;
    return -1;
}

void back_patch(int jump_addr){
    int jump_Cmd = jumpstack.top();
    jumpstack.pop();
    INSTR_TABLE[jump_Cmd+1].oprnd = jump_addr;
}



class Parser{
private:
    fstream inFile, outFile;
    struct token TOKEN;
    struct token SAVE;
    ostream& OS = outFile;
    bool SyntaxOut = true;
    
    
    
    
 
    
    
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
        INSTR_TABLE.push_back(INSTR(0,"null",0));
        OptFunctionDefinitions();
        TakeToken();
        if(TOKEN.content == "$$"){
            printTOKEN();
            OptDeclarationList();
            TakeToken();
            if(TOKEN.content == "$$"){
                Declarating = false;
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
            DataType = TOKEN.content;
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
            DataType = TOKEN.content;
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
            SAVE = TOKEN;
            Identifier();
            TakeToken();
            if(TOKEN.content ==":="){
                printTOKEN();
                Expression();
                Memory_Location = get_address(SAVE);
                gen_instr("POPM", Memory_Location);
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
            //Addr = get_address(TOKEN);
            printTOKEN();
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                Condition();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    Statement();
                    back_patch(Instr_Addr);
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
            gen_instr("LABEL", -999);
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
                        gen_instr("STDOUT", -999);
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
            gen_instr("STDIN", -999);
            printTOKEN();
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                IDs();
                //Memory_Location = get_address(TOKEN);
                gen_instr("POPM", Memory_Location);
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
            int jump_Cmd = Instr_Addr;
            gen_instr("LABEL", -999);
            TakeToken();
            if(TOKEN.content == "("){
                printTOKEN();
                Condition();
                TakeToken();
                if(TOKEN.content == ")"){
                    printTOKEN();
                    Statement();
                    gen_instr("JUMP", jump_Cmd);
                    back_patch(Instr_Addr);
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
        
        if(TOKEN.content == "="){
            gen_instr("EQU", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }else if(TOKEN.content == "/="){
            gen_instr("NEQ", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }else if(TOKEN.content == ">"){
            gen_instr("GRT", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }else if(TOKEN.content == "<"){
            gen_instr("LES", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }else if(TOKEN.content == "=>"){
            gen_instr("GRE", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }else if(TOKEN.content == "<="){
            gen_instr("LEE", -999);
            jumpstack.push(Instr_Addr);
            gen_instr("JUMPZ", -999);
        }
        
     
        
    }
    
    void  Expression(){
        PrintParseInfo(outFile,"<Expression>","<Term> <Expression Prime>");
        Term();
        ExpressionPrime();
    }
    
    
    void  ExpressionPrime(){
        TakeToken();
        if(TOKEN.content == "+" ){
            printTOKEN();
            PrintParseInfo(OS, "<Expression Prime>", TOKEN.content +" <Term> <Expression Prime>");
            Term();
            gen_instr("Add", -999);
            ExpressionPrime();
        }else if(TOKEN.content == "-" ){
            printTOKEN();
            PrintParseInfo(OS, "<Expression Prime>", TOKEN.content +" <Term> <Expression Prime>");
            Term();
            gen_instr("SUB", -999);
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
        if(TOKEN.content == "*" ){
            printTOKEN();
            PrintParseInfo(OS, "<Term Prime>", TOKEN.content+" <Factor> <Term Prime>");
            Factor();
            gen_instr("MUL", -999);
            TermPrime();
        }else if(TOKEN.content == "/" ){
            printTOKEN();
            PrintParseInfo(OS, "<Term Prime>", TOKEN.content+" <Factor> <Term Prime>");
            Factor();
            gen_instr("DIV", -999);
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
            Memory_Location = get_address(TOKEN);
            gen_instr("PUSHM", Memory_Location);
            cout << Instr_Addr-1 <<"   "<< TOKEN.content<<endl;
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
        if(Declarating){
            gen_SymbolTable(TOKEN);
        }else{
            Memory_Location = get_address(TOKEN);
        }
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
