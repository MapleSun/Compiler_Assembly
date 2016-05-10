//
//  lexel.h
//  Syntax_Analysis
//
//  Created by SYJ on 4/3/16.
//  Copyright © 2016 SunYijie. All rights reserved.
//

#ifndef lexel_h
#define lexel_h

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

struct token{
    string type;
    string content;
    int Addr;
};

// character can be solved by FSM
bool FSM_char(char curr_char){
    // ASCII: A=65, Z=91, 0=48, 9=57, .=46, _=95
    // First ||: A-Z; Second ||:0-9; Third: .; Fouth: _;
    
    return ((int)toupper(curr_char)>64 && (int)toupper(curr_char)<91) || ((int)(curr_char)>47 && (int)(curr_char)<58) || (int)curr_char == 46 || (int)curr_char == 95 ;
}

bool Keyword(string token){
    vector<string> keyword={"function", "integer", "boolean", "real", "if", "else", "endif", "return", "printf", "scanf", "while", "true", "false"};
    string token_low;
    for (int i=0; i<token.size(); ++i) {
        token_low += tolower(token[i]);
    }
    int size = (int)keyword.size();
    for (int j=0; j<size; ++j) {
        if(token_low == keyword[j])
            return true;
    }
    return false;
}

// other operator and sepearator
bool is_OP(char curr_char){
    vector<char> OP = {':', '>', '<', '=', '+', '-', '*', '/'};
    bool is_OP = false;
    for (int i = 0; i < OP.size(); i++) {
        if(curr_char == OP[i]){
            is_OP = true;
        }
    }
    return is_OP;
}

bool is_SP(char curr_char){
    vector<char> SP = {'$', '(', ')', ';', '{', '}', ','};
    bool is_SP = false;
    for (int i = 0; i < SP.size(); i++) {
        if(curr_char == SP[i]){
            is_SP = true;
        }
    }
    return is_SP;
}

void FSM_table(char curr_char, int& curr_state){
    vector<int> accecpt_state = {1, 4, 5, 7}; // int:1, real:4, id:5, 7
    vector<vector<int>> FSM_table = {
        /*     d,.,l,_ */
        /*0*/ {1,3,5,0},
        /*1*/ {1,3,0,0}, //{2,3,-1,-1} -> {1,3,-1,0}
        /*2*/ {2,3,0,0},
        /*3*/ {4,0,0,0},
        /*4*/ {4,0,0,0},
        /*5*/ {0,0,7,6},
        /*6*/ {0,0,7,6},
        /*7*/ {0,0,7,6}
    };
    int row_state = 0, col_input = 0;
    
    // digits
    if((int)(curr_char)>47 && (int)(curr_char)<58){
        col_input = 0;
        row_state = curr_state;
        curr_state=FSM_table[row_state][col_input];
    }
    // dot
    else if ((int)curr_char == 46){
        col_input = 1;
        row_state = curr_state;
        curr_state=FSM_table[row_state][col_input];
    }
    // letter
    else if((int)toupper(curr_char)>64 && (int)toupper(curr_char)<92){
        //cout << curr_char << endl;
        col_input = 2;
        row_state = curr_state;
        curr_state=FSM_table[row_state][col_input];
    }
    //underscore
    else if ((int)curr_char == 95){
        // cout << curr_char << endl;
        col_input = 3;
        row_state = curr_state;
        curr_state=FSM_table[row_state][col_input];
    }
    // for unknown char, it means end of one token
    
    else{
        curr_state = 0;
    }
    
    
}

void print(ostream& outfile, string tokentype, token& lexeme){
    int WIDTH = 15;
    lexeme.type = tokentype;
    //outfile << left << "Token: " << setw(WIDTH) << lexeme.type << "       Lexeme: " << lexeme.content << endl;
    //cout << left << "Token: " << setw(WIDTH) << lexeme.type << "       Lexeme: " << lexeme.content << endl;
}

struct token lexer(fstream& file, ostream& outfile/*, vector<string>* token = NULL*/)
{
    struct token token;
    char curr_char;
    // keep get char from file
    if (file.get(curr_char)) {
        
        // if curr_char is a Seperator
        
        if(curr_char == ' ' && curr_char == '\n'){
            //file.ignore();
            lexer(file,outfile);
        }
        
        else if(is_SP(curr_char)){
            token.content += curr_char;
            // if the curr_char is '$' and file can get next char
            if(curr_char == '$' ){
                if (file.get(curr_char) && curr_char == '$') {
                    token.content += curr_char;
                    //outfile << left << setw(width) << "Sepreator " << token << endl;
                    print(outfile, "Seperator", token);
                    
                }
                else{
                    file.putback(curr_char);
                    //outfile << left << setw(width) << "unknow " << token << endl;
                    print(outfile, "Unknow", token);
                }
            }else{
                //outfile << left << setw(width) << "Sepreator " << token << endl;
                print(outfile, "Seperator", token);
                
            }
            return token;
        }
        
        // if curr_char is an Operator
        else if(is_OP(curr_char)){
            token.content += curr_char;
            if(curr_char == ':' ){
                if (file.get(curr_char) && curr_char == '=') {
                    token.content += curr_char;
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
                else{
                    file.putback(curr_char);
                    //outfile << left << setw(width) << "unknow " << token << endl;
                    print(outfile, "Operator", token);
                }
            }
            else  if(curr_char == '/' ){
                if (file.get(curr_char) && curr_char == '=') {
                    token.content += curr_char;
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
                else{
                    file.putback(curr_char);
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
            }
            else if(curr_char == '=' ){
                if (file.get(curr_char) && curr_char == '>') {
                    token.content += curr_char;
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
                else{
                    file.putback(curr_char);
                    //outfile << left << setw(width) << "unknow " << token << endl;
                    print(outfile, "Unknow", token);
                }
            }
            else if(curr_char == '<' ){
                if (file.get(curr_char) && curr_char == '=') {
                    token.content += curr_char;
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
                else{
                    file.putback(curr_char);
                    //outfile << left << setw(width) << "Operator " << token << endl;
                    print(outfile, "Operator", token);
                }
            }else{
                //outfile << left << setw(width) << "Operator " << token << endl;
                print(outfile, "Operator", token);
            }
            return token;
            
        }
        
        // look up FSM table
        else if(FSM_char(curr_char)){
            //string token;
            int start_state=0, curr_state, previous_state;
            curr_state = start_state;
            
            do {
                previous_state = curr_state;
                FSM_table(curr_char, curr_state);
                token.content += curr_char;
                
                
            } while (curr_state != 0 && file.get(curr_char) /*&& FSM_char(curr_char)*/ );
            
            
            /*
             after the while loop all current state state should be 0
             which means token = fsm token + current char
             except the end of file.
             */
            
            if (token.content.size() != 1 && !file.eof()) {
                token.content.pop_back();
                file.putback(curr_char);
                
            }
            /*
             check previous state
             */
            
            switch (previous_state) {
                case 1:
                    //outfile << left << setw(width) << "Int  " << token << endl;
                    print(outfile, "integer", token);
                    return token;
                    
                    
                case 4:
                    //outfile << left << setw(width) << "Real  " << token << endl;
                    print(outfile, "real", token);
                    return token;
                    
                case 5:
                    print(outfile, "identifier", token);
                    return token;
                    
                case 7:
                    if (Keyword(token.content)) {
                        //outfile << left << setw(width) << "Keyword  " << token << endl;
                        print(outfile, "Keyword", token);
                        return token;
                        
                    }else{
                        //outfile << left << setw(width) << "Identifier  " << token << endl;
                        print(outfile, "identifier", token);
                        return token;
                    }
                    
                default:
                    print(outfile, "Unknow", token);
                    return token;
            }
        }
        else if(curr_char != ' ' && curr_char != '\n')
        {
            //outfile << left << setw(width) << "Unknow " << curr_char << endl;
            
        }
        
        
    } // end of if
    
    //token.type = "lexer fail";
    //token.content = "nothing";
    return token;
    
}

#endif /* lexel_h */
