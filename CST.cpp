#include "Token.hpp"
#include "CST.hpp"
#include "CSTNode.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include<stack>
#include "CSTNode.hpp"
#include "CST.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include <vector>

/** **************************************************************************************
recursively adds a child/
@pre:
@post:
 *****************************************************************************************/
void CST::addChild(CSTNode *root, Token token) {
    if ( root == nullptr )
        return;

    if (root->getLeft() == nullptr && root->getRight() == nullptr) {
        CSTNode* newNode = new CSTNode(token);
        root->leftChild(newNode);
        root->getLeft()->setChild(true);
    }else if (root->getLeft() != nullptr) {
        addChild(root->getLeft(), token) ;
    }else if (root->getRight() != nullptr) {
        addChild(root->getRight(), token);
    }
    return;
}





/** **************************************************************************************
recursively adds a sibling.
@pre:
@post:
 *****************************************************************************************/
void CST::addSibling(CSTNode *root, Token token) {

    if ( root == nullptr )
        return;

    if ( root->getRight() == nullptr && root->getLeft() == nullptr ){
        CSTNode* newNode = new CSTNode( token );
        root->rightSibling( newNode );
    }else if ( root->getRight() != nullptr ){
        addSibling( root->getRight(), token );
    } else if ( root->getLeft() != nullptr ){
        addSibling( root->getLeft(), token );
    }
    return;
}




/** **************************************************************************************
Prints the TREE using BFS
@pre:
@post:
 *****************************************************************************************/
void CST::printTree(){
    if ( root == nullptr ) {
        return;
    }
    std::queue<CSTNode*> q;
    q.push( root );

    while ( !q.empty() ) {

        CSTNode* current = q.front();
        //checks if its a child, if yess then it drops to a new line
        if ( current->isChild() ){
            std::cout <<std::endl;
            std::cout <<" |"<<std::endl;
            std::cout <<"\\|/";
            std::cout << "\n" + current->getToken().getTokenString()<< "--> ";
        } else {
            std::cout << current->getToken().getTokenString() << "--> ";
        }
        q.pop();

        if ( current->getLeft()!= nullptr ){
            q.push( current->getLeft() );
        }
        if ( current->getRight() != nullptr ){
            q.push( current->getRight() );
        }
    }
    std::cout <<std::endl;
}

/** **************************************************************************************
creates a abstract syntax tree out of our concrete syntax tree. 
@pre:takes our CST 
@post:converst CST to a AST
notes:
use yard's algorithm to convert numerical expressions and boolean expressions to postfix
{ = begin block
} = end block
function headers like function int sum_of_first_n_squares (int n) = declaration
int sum; = declaration
number = 3; = assignment ->assignment statements tokens in postfix.
return a; return ->value.
 *****************************************************************************************/
void CST::cstToAst(){

    
    //if we find nullptr stop
    if ( root == nullptr ) {
        return;
    }

    //our new root
    Token token(0,0);
    token.setIdentifier("DECLARATION");
    CSTNode* astRoot = new CSTNode(token);    

    //set up a queue
    std::queue<CSTNode*> q;

    //vector to store statement until we reach null ptr for right sibling.
    std::vector<Token> lineByLine;

    //push the root onto the queue
    q.push( root );

    //traverse through our queue
    while ( !q.empty() ) {

        CSTNode* current = q.front();

        //checks if its a child, if yes then it drops to a new line
        if ( current->isChild() ){
            //in here we want to process the line we just recieved

            //if the front of our vector is a identifier with the 
            if(lineByLine.front().isIdentifier() && lineByLine.front().getTokenString() == "function" ){

                //insert "DECLARATION" token, and progress to next line.
                std::cout<<"found DECLARATION with string: "<<lineByLine.front().getTokenString() <<std::endl;
                Token token(lineByLine.front().getLineNum(), lineByLine.front().getCharPos());
                token.setIdentifier("DECLARATION");
                addChild(astRoot,token);

                lineByLine.clear();

            //if we find a datatype as a identifier
            }else if(lineByLine.front().isIdentifier() && (lineByLine.front().getTokenString() == "int" || lineByLine.front().getTokenString() == "string"|| lineByLine.front().getTokenString() == "char" )){

                //insert "DECLARATION" token, and progress to next line.
                for(int i = 0;i < lineByLine.size();i++){
                    std::cout<<"found DECLARATION with string: "<<lineByLine[i].getTokenString() <<std::endl;
                    Token token(lineByLine[i].getLineNum(), lineByLine[i].getCharPos());
                    token.setIdentifier("DECLARATION");
                    addChild(astRoot,token);
                }
                //note if multiuple variable are declared, add a declarations token for each of them before moving to the next line.
                lineByLine.clear();

            }else if(lineByLine.front().isIdentifier() && lineByLine.front().getTokenString() == "if" ){

                //insert "IF" token

                //then look through the rest of this line to get the relivant boolean operations

                //apply yards algorithm on those operations

                //insert them into our ast and move to next line.

            }else if(lineByLine.front().isIdentifier() && lineByLine.front().getTokenString() == "else" ){

                //insert "ELSE" token



            }else if(lineByLine.front().isIdentifier() && lineByLine.front().getTokenString() == "for" ){

                //loop through to get each foor loop part note, each part goes down as a left child of the previous statement.

                //insert "FOR EXPRESSION 1" token with the postfix operations after it.

                //insert "FOR EXPRESSION 2" token with the postfix operations after it.

                //insert "FOR EXPRESSION 3" token with the postfix operations after it.


            
            }else if(lineByLine.front().isIdentifier() && lineByLine.front().getTokenString() == "printf" ){

                //insert "PRINTF" token

                //insert string plus the variables associated with that print f statement


            //if we find a brace
            }else if(lineByLine.front().isLBrace()){

                //insert "BEGIN BLOCK" token, and progress to next line.
                std::cout<<"found BEGIN BLOCK with string: "<<lineByLine.front().getTokenString() <<std::endl;
                Token token(lineByLine.front().getLineNum(), lineByLine.front().getCharPos());
                token.setIdentifier("BEGIN BLOCK");
                addChild(astRoot,token);

                lineByLine.clear();

            }else if(lineByLine.front().isRBrace()){

                //insert "END BLOCK" token, and progress to next line.
                std::cout<<"found END BLOCK with string: "<<lineByLine.front().getTokenString() <<std::endl;
                Token token(lineByLine.front().getLineNum(), lineByLine.front().getCharPos());
                token.setIdentifier("END BLOCK");

                lineByLine.clear();

                addChild(astRoot,token);
            }else if(lineByLine.front().isIdentifier()){

                //check next element in the vector to see if its a equals 
                if(lineByLine[1].isAssignmentOperator()){


                    //if so traverse through our line until adding only the relivant operational characters.
                    //ie, operators, operands paren's ect.
                }
                
            }

            //then push its new processed tokens onto the AST

            //then clear our vector for the next line in our CST

            //lineByLine.clear();
            //std::cout <<std::endl;
            //std::cout <<" |"<<std::endl;
            //std::cout <<"\\|/";
            //std::cout << "\n" + current->getToken().getTokenString()<< "--> ";
        } else {
            //std::cout << current->getToken().getTokenString() << "--> ";
        }
        q.pop();

        if ( current->getLeft()!= nullptr ){
            q.push( current->getLeft() );
        }
        if ( current->getRight() != nullptr ){

            // add our elements to our vector 
            lineByLine.push_back(current->getToken());

            q.push( current->getRight() );
        }
    }
    //std::cout <<std::endl;

    root = astRoot;

    std::cout<<"Sucessfully Created AST ------------------------------------------------------"<<std::endl;

}


/** **************************************************************************************
//function to take a infix expression and conver it to postfix
@pre: takes a vector of tokens in infix
@post: returns a vector with token in infix
 *****************************************************************************************/
std::vector<Token> CST::yardAlgorithm (std::vector<Token>& infix){

    std::vector<Token> postfix;
    std::stack<Token> stack;
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
