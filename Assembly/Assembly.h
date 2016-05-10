//
//  Assembly.h
//  Assembly
//
//  Created by SYJ on 5/5/16.
//  Copyright © 2016 SunYijie. All rights reserved.
//

#ifndef Assembly_h
#define Assembly_h

#include <string>
#include <iomanip>
#include "lexer.h"
using namespace std;



/*
 INSTR_TABLE
 
 
 
 */

struct INSTR{
    int address;
    string OP;
    int oprnd;
    INSTR(int address_,string OP_,int oprnd_):address(address_),OP(OP_),oprnd(oprnd_){}
    void display();
};

void INSTR::display(){
    int width = 8;
    cout  << setw(width) << OP << setw(width) << oprnd << endl;
}


struct SYMBOL{
    string Ident;
    int MemoryLoc;
    string Type;
    SYMBOL(string Ident_,int MemoryLoc_, string Type_):Ident(Ident_),MemoryLoc(MemoryLoc_),Type(Type_){}
    void display();
};

void SYMBOL::display(){
    int width = 8;
    cout << setw(width) <<Ident << setw(width) << MemoryLoc << setw(width) << Type << endl;
}

#endif /* Assembly_h */
