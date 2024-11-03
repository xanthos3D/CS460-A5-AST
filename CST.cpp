#include "Token.hpp"
#include "CST.hpp"
#include "CSTNode.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <queue>


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