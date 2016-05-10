//
//  main.cpp
//  Syntax_Analysis
//
//  Created by SYJ on 3/26/16.
//  Copyright © 2016 SunYijie. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "lexer.h"
#include "Rules.h"
using namespace std;


int main(int argc, const char * argv[]) {
    fstream inFile, outFile;
    string in_file = argv[1];
    string out_file = argv[2];
    inFile.open(in_file);
    outFile.open(out_file, std::ios::out);
    Parser Parser(in_file, out_file);
    
    Parser.Rat16S();
    display_Instr();
    display_Symbol();
    //Parser.Statement();
    
    
    
    
    //Parser.inFile = inFile;
    //Parser.outFile = outFile;
    
    
    
    
    
    
    inFile.close();
    outFile.close();
    return 0;
    
    
    
}








