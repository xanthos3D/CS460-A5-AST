/* ***********************************************************************
team justice names here:
xander maniaci
Luis Galvez
Christian Gonzalez
Anthony Manese

how to compile program through terminal
1. g++ main.cpp Token.cpp Tokenizer.cpp Parser.cpp CST.cpp SymbolTable.cpp
2. ./a.out programming_assignment_1-test_file_1.c

how to run makefile
1.  make
2.  ./ST programming_assignment_4-test_file_1.c

description:
Write a program in C or C++ that creates a symbol table (a linked list) 
of all the defined variables (including their type and scope) and the names
of all functions and procedures. Functions and procedures may also have 
an input parameter list of variables and types. These too should be added
(with appropriate scope) to the symbol table. Lastly, functions have a 
return datatype which must be noted in the symbol table as well.
*********************************************************************** */
#include <iostream>
#include <fstream>
#include<stack>
#include "CSTNode.hpp"
#include "CST.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include <vector>
#include <stack>

using namespace std;

/** **************************************************************************************
function Headers
 *****************************************************************************************/
string commentParser(std::ifstream& inputStream,string fileName);
string parseTokens(Tokenizer& tempTokenizer);
std::vector<Token> buildTokenVector(Tokenizer& sourceFile);
vector<Token> yardAlgorithm(vector<Token>& infix);
void testYard1();

// functions to handle the open and close tags as they come into the parser.
/** **************************************************************************************
driver program
@pre: takes a input c file
@post: removes the comments in the .c file input and outputs a c file
 *****************************************************************************************/
int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile" << std::endl;
        exit(1);
    }

    std::ifstream inputStream;
    inputStream.open(argv[1], std::ios::in);    // open for reading

    if(!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        exit(2);
    }

    inputStream.close();
    std::cout << "found file: " <<argv[1] << std::endl;
    string filename = argv[1];
    filename.pop_back();
    filename.pop_back();

    //function to remove comments
    string output = commentParser(inputStream,argv[1]);
    std::cout<<"Removing comments -------------------------------------------------------"<<std::endl;

    string tokenizeFile = filename + " without comments.c";
    ofstream result("./" + tokenizeFile, ios::out);
    result << output;
    result.close();

    Tokenizer tokenizer(tokenizeFile);
    //output = parseTokens(tokenizer);

    //function to get tokens out of comment free file
    std::vector<Token> tokenVector = buildTokenVector(tokenizer);

    std::cout<<"Tokenized Program ------------------------------------------------------"<<std::endl;

    //make the parse object, passing it in our token vector.
    Parser CSTparser(tokenVector);

    //call our parse function built into our parser to make the CST
    CST* tree = CSTparser.parse();
    //tree->printTree();

    CSTparser.printTree();
    std::cout<<"Sucessfully Created CST ------------------------------------------------------"<<std::endl;

    CSTparser.PrintSymbolTableLL();
    std::cout<<"Sucessfully Created Symbol Table Linked List ------------------------------------------------------"<<std::endl;

    return 0;
}

/** **************************************************************************************
function to build our token vector for the recursive decent parsing
@pre: takes a tokenizer object
@post: returns a vector of tokens
 *****************************************************************************************/
std::vector<Token> buildTokenVector(Tokenizer& sourceFile){
    //cout<<"making token vector. ++++++++++++++++++++++++++++++++++++ <<"<<endl;
    //make a vector to stre tokens
    std::vector<Token> tokenVector;
    //gets the first token
    Token token = sourceFile.getToken();
    //token.print();
    //add first token to vector
    tokenVector.push_back(token);
    //loop through until we reach a eof token
    while (!token.isEOF()) {
        token = sourceFile.getToken();
        token.print();
        tokenVector.push_back(token);
    }
    //cout<<"token vector complete. ++++++++++++++++++++++++++++++++++++ <<"<<endl;
    return tokenVector;
}

/** **************************************************************************************
//function to take a infix expression and conver it to postfix
@pre: takes a vector of tokens in infix
@post: returns a vector with token in infix
 *****************************************************************************************/
vector<Token> yardAlgorithm (vector<Token>& infix){

    vector<Token> postfix;
    stack<Token> stack;
    bool finished = false;

    //foreach token in token list
    for(int i = 0; i < infix.size(); i++){
        if ((infix[i].isInt()) || (infix[i].isIdentifier()) || (infix[i].isSingleQuote()) || (infix[i].isDoubleQuote()) ||
            (infix[i].isString()) || (infix[i].isLBracket()) || (infix[i].isRBracket())){
            //display token
            infix[i].print();
        }else{
            if (infix[i].isLParen())
            {
                //push token on stack
                stack.push(infix[i]);

            }else{

                if (infix[i].isRParen()){

                    finished = false;
                    while (!finished){
                        //top of stack token == LEFT PARENTHESIS
                        if (stack.top().isLParen()){
                            stack.pop();
                            finished = true;
                        }else{
                            //display token at top of stack
                            stack.top().print();
                            postfix.push_back(stack.top());
                            //pop stack
                            stack.pop();
                        }
                    }
                }else{
                    if ((infix[i].isBoolE())      || (infix[i].isBoolNE())             || (infix[i].isBoolLT())   || (infix[i].isBoolGT()) ||
                        (infix[i].isBoolLTE()) || (infix[i].isBoolGTE()) || (infix[i].isBoolAnd()) || (infix[i].isBoolOr())   ||
                        (infix[i].isBoolNot())        || (infix[i].isPlus())                  || (infix[i].isMinus())       || (infix[i].isAsterisk())     ||
                        (infix[i].isDivide())             || (infix[i].isModulo())){
                        if (stack.empty()){
                            //push token on stack
                            stack.push(infix[i]);
                        }else{
                            //top of stack token == LEFT PARENTHESIS
                            if (infix[i].isLParen()){
                                stack.push(infix[i]);
                            }else{
                                //token == BOOLEAN_NOT
                                if (infix[i].isBoolNot())
                                {
                                    finished = false;

                                    while (!finished){
                                        if (!stack.empty()){
                                            //top of stack token == BOOLEAN NOT
                                            if (stack.top().isBoolNot()){
                                                //display token at top of stack
                                                stack.top().print();
                                                postfix.push_back(stack.top());
                                                //pop stack
                                                stack.pop();
                                            }else{
                                                //push token on stack
                                                stack.push(infix[i]);
                                                finished = true;
                                            }
                                        }else{
                                            //push token on stack
                                            finished = true;
                                        }
                                    }
                                }else{
                                    if ((infix[i].isAsterisk()) || (infix[i].isDivide()) || (infix[i].isModulo())){
                                        finished = false;
                                        while (!finished){
                                            if (!stack.empty()){
                                                if ((stack.top().isBoolNot())  || (stack.top().isAsterisk()) || (stack.top().isDivide()) || (stack.top().isModulo())){
                                                    //display token at top of stack
                                                    stack.top().print();
                                                    postfix.push_back(stack.top());
                                                    stack.pop();
                                                }else{
                                                    //push token on stack
                                                    stack.push(infix[i]);
                                                    finished = true;
                                                }
                                            }else{
                                                //push token on stack
                                                stack.push(infix[i]);
                                                finished = true;
                                            }
                                        }
                                    }else{
                                        if ((infix[i].isPlus()) || (infix[i].isMinus())){
                                            finished = false;
                                            while (!finished){
                                                if (!stack.empty())
                                                {
                                                    if ((stack.top().isBoolNot()) || (stack.top().isAsterisk()) ||
                                                        (stack.top().isDivide())      || (stack.top().isModulo())   ||
                                                        (stack.top().isPlus())        || (stack.top().isMinus()))
                                                    {
                                                        //display token at top of stack
                                                        stack.top().print();
                                                        postfix.push_back(stack.top());
                                                        stack.pop();
                                                    }else{
                                                        //push token on stack
                                                        stack.push(infix[i]);
                                                        finished = true;
                                                    }
                                                }else{
                                                    //push token on stack
                                                    stack.push(infix[i]);
                                                    finished = true;
                                                }
                                            }
                                        }else{
                                            if ((infix[i].isBoolE())      || (infix[i].isBoolNE()) || (infix[i].isBoolLT()) || (infix[i].isBoolGT()) ||
                                                (infix[i].isBoolLTE()) || (infix[i].isBoolGTE()))
                                            {
                                                finished = false;
                                                while (!finished){
                                                    if (!stack.empty())
                                                    {
                                                        if ((stack.top().isBoolNot())        || (stack.top().isAsterisk())              ||
                                                            (stack.top().isDivide())             || (stack.top().isModulo())                ||
                                                            (stack.top().isPlus())               || (stack.top().isMinus())                 ||
                                                            (stack.top().isBoolE())      || (stack.top().isBoolNE())             ||
                                                            (stack.top().isBoolLT())          || (stack.top().isBoolGT())          ||
                                                            (stack.top().isBoolLTE()) || (stack.top().isBoolGTE()) ||
                                                            (stack.top().isBoolNE()))
                                                        {
                                                            //display token at top of stack
                                                            stack.top().print();
                                                            postfix.push_back(stack.top());
                                                            stack.pop();
                                                        }else{
                                                            //push token on stack
                                                            stack.push(infix[i]);
                                                            finished = true;
                                                        }
                                                    }else{
                                                        //push token on stack
                                                        stack.push(infix[i]);
                                                        finished = true;
                                                    }
                                                }
                                            }else{
                                                if (infix[i].isBoolAnd()){
                                                    finished = false;
                                                    while (!finished){
                                                        if (!stack.empty()){
                                                            if ((stack.top().isBoolNot())        || (stack.top().isAsterisk())              ||
                                                                (stack.top().isDivide())             || (stack.top().isModulo())                ||
                                                                (stack.top().isPlus())               || (stack.top().isMinus())                 ||
                                                                (stack.top().isBoolE())      || (stack.top().isBoolNE())             ||
                                                                (stack.top().isBoolLT())          || (stack.top().isBoolGT())          ||
                                                                (stack.top().isBoolLTE()) || (stack.top().isBoolGTE()) ||
                                                                (stack.top().isBoolNE()))
                                                            {
                                                                //display token at top of stack
                                                                stack.top().print();
                                                                postfix.push_back(stack.top());
                                                                stack.pop();
                                                            }else{
                                                                //push token on stack
                                                                stack.push(infix[i]);
                                                                finished = true;
                                                            }
                                                        }else{
                                                            //push token on stack
                                                            stack.push(infix[i]);
                                                            finished = true;
                                                        }
                                                    }
                                                }else{
                                                    if (infix[i].isBoolOr())
                                                    {
                                                        finished = false;
                                                        while (!finished)
                                                        {
                                                            if (!stack.empty())
                                                            {
                                                                if ((stack.top().isBoolNot())        || (stack.top().isAsterisk())              ||
                                                                    (stack.top().isDivide())             || (stack.top().isModulo())                ||
                                                                    (stack.top().isPlus())               || (stack.top().isMinus())                 ||
                                                                    (stack.top().isBoolE())      || (stack.top().isBoolNE())             ||
                                                                    (stack.top().isBoolLT())          || (stack.top().isBoolGT())          ||
                                                                    (stack.top().isBoolLTE()) || (stack.top().isBoolGTE()) ||
                                                                    (stack.top().isBoolNE()))
                                                                {
                                                                    //display token at top of stack
                                                                    stack.top().print();
                                                                    postfix.push_back(stack.top());
                                                                    stack.pop();
                                                                }else{
                                                                    //push token on stack
                                                                    stack.push(infix[i]);
                                                                    finished = true;
                                                                }
                                                            }else{
                                                                //push token on stack
                                                                stack.push(infix[i]);
                                                                finished = true;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    while (!stack.empty()){
        //display token at top of stack
        stack.top().print();
        postfix.push_back(stack.top());
        stack.pop();
    }

    return postfix;
}

/** **************************************************************************************
//function to test the yard algorithm
 *****************************************************************************************/
void testYard1(){
    cout<<"testing yard algorithm"<< endl;
    cout<<endl;

    vector<Token> test1;
    Token token(1, 1);
    token.setLParen();
    test1.push_back(token);

    Token token1(1, 1);
    token1.setIdentifier("aaa");
    test1.push_back(token1);

    Token token2(1, 1);
    token2.setBoolNE();
    test1.push_back(token2);

    Token token3(1, 1);
    token3.setInt("0");
    test1.push_back(token3);

    Token token4(1, 1);
    token4.setRParen();
    test1.push_back(token4);

    Token token5(1, 1);
    token5.setBoolOr();
    test1.push_back(token5);

    Token token6(1, 1);
    token6.setLParen();
    test1.push_back(token6);

    Token token7(1, 1);
    token7.setIdentifier("bbb");
    test1.push_back(token7);

    Token token8(1, 1);
    token8.setBoolGT();
    test1.push_back(token8);

    Token token9(1, 1);
    token9.setLParen();
    test1.push_back(token9);

    Token token10(1, 1);
    token10.setIdentifier("ccc");
    test1.push_back(token10);

    Token token11(1, 1);
    token11.setPlus();
    test1.push_back(token11);

    Token token12(1, 1);
    token12.setIdentifier("ddd");
    test1.push_back(token12);

    Token token13(1, 1);
    token13.setAsterisk();
    test1.push_back(token13);

    Token token14(1, 1);
    token14.setIdentifier("eee");
    test1.push_back(token14);

    Token token15(1, 1);
    token15.setRParen();
    test1.push_back(token15);

    Token token16(1, 1);
    token16.setRParen();
    test1.push_back(token16);

    Token token17(1, 1);
    token17.setBoolAnd();
    test1.push_back(token17);

    Token token18(1, 1);
    token18.setLParen();
    test1.push_back(token18);

    Token token19(1, 1);
    token19.setIdentifier("fff");
    test1.push_back(token19);

    Token token20(1, 1);
    token20.setBoolGT();
    test1.push_back(token20);

    Token token21(1, 1);
    token21.setInt("6");
    test1.push_back(token21);

    Token token22(1, 1);
    token22.setRParen();
    test1.push_back(token22);

    yardAlgorithm(test1);
}
/** **************************************************************************************
function to get tokens loop through file getting tokens from the tokenizer until we get to a eof token.
@pre: takes a tokenizer object
@post: makes a list of tokens out of our comment fre output file.
 *****************************************************************************************/
string parseTokens(Tokenizer& tempTokenizer){


    cout<< "Token list:"<<endl;

    std::string file = "";
    file+= "\nToken list:\n\n";

    //gets the first token of the file
    Token tempToken = tempTokenizer.getToken();

    file += tempToken.print();

    //as long as the token recieved is not a eof keep looping through the file.
    while (!tempToken.isEOF()) {
        //keep grabbing new tokens and setting the next token as temptoken until we
        //run out of tokens.

        tempToken = tempTokenizer.getToken();
        file += tempToken.print();

    }

    return file;



}

/** **************************************************************************************
function to remove comments by using a dfa to traverse through our file.
@pre: takes a ifstream object and a filename
@post: removes the comments in the .c file input and outputs a c file
 *****************************************************************************************/
string commentParser(std::ifstream& inputStream,string fileName){

    inputStream.open(fileName, std::ios::in);

    string file = "";
    int state = 0;
    char c;
    int lineNumber = 1;
    int startOfComment = -1;

    //keep looping through our dfa as long as we have input to read
    while(!inputStream.eof()){

        cout<<c;
        inputStream.get(c);

        //index 0 grabs everything until we reach a /
        if(state == 0 ) {
            //if we find a slash then we want to transition to index 1
            if (c == '/') {
                state = 1;
                //otherwise add the input to the file.
            } else{
                if (c == '"') {
                    state = 5;
                } else if (c == '\n') {
                    ++lineNumber;
                } else if (c == '*') {
                    state = 6;
                }
                file += c;
            }
            // if we find a / then we need to check if the next char is a * or a /
        }else if(state == 1){
            //if we find a slash after the initial slash then we are in a c style comment so progress to index 2
            if(c == '/'){
                //adds two spaces to the file to account for the double slash.
                file +="  ";
                state = 2;
                //otherwise if we find a /* then progress to index 3 c++ style comment
            }else if(c == '*'){
                //adds two spaces to the file to account for the slash star.
                file +="  ";
                state = 3;
                //The beginning of a comment block that we need to keep track of if never closed
                startOfComment = lineNumber;
                // otherwise we are not in a comment back to index 0
            }else{
                //add the / since not a comment
                file +='/';
                //add the new char c as well since not in comment
                file +=c;
                state = 0;
            }
            //if we are in a c style comment then
        }else if(state == 2){
            //if we find a next line char then c style comment is done, back to index 0
            if(c == '\n'){
                file +=c;
                state = 0;
                //keep track of the line we are by increasing every time we encounter a new line
                ++lineNumber;
            }else{
                //replace that comment with spaces
                file +=' ';
            }
            //index for being inside a c++ style comment
        }else if(state == 3){
            //if we find a next line char then c style comment is done, back to index 0
            if(c == '*'){
                file +=' ';
                state = 4;
                //if we find a next line char then preserve it.
            }else if(c == '\n') {
                file += c;
                ++lineNumber;
            }else{
                //replace that comment with spaces
                file +=' ';
            }
            //index to check if c++ style comment should be ending
        }else if(state == 4){
            //if we find a next line char then c style comment is done, back to index 0
            if(c == '/'){
                //set double space to handle places where */ should be
                file +=" ";
                state = 0;
            }else if(c == '*'){
                //set double space to handle places where */ should be
                file +=" ";
            }else if(c == '\n'){
                file +=c;
                ++lineNumber;
            }else{
                //replace that comment with spaces
                file +=" ";
                state = 3;
            }
            //set input in comments to white space unless we find a next line character
        } else if (state == 5){
            if ( c=='"'){
                state = 0;
            }else if (c == '\\'){
                file += c;
                inputStream.get(c);
            }
            file += c;
        } else if (state == 6){
            if (c == '/'){
                startOfComment = lineNumber;
                break;
            }
            file +=c;
            state = 0;
        }
    }
    //output error message if: inside a never ending c++ comment (/* without the */) or if end of c++ comment is read without the starting of the comment (*/ without the /*)
    if ( state == 3 || state == 4 || !inputStream.eof()){
        cout << "ERROR: Program contains C-style, unterminated comment starting on line " << startOfComment << endl;
        exit(1);
    }

    inputStream.close();

    cout<<endl;
    cout<<"==================output file=================="<< endl;
    cout<< file;

    return file;



}
