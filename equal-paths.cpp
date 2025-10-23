#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){return true;}

    if(height(root->left) == height(root->right)){
        return true;
    }
    else if(height(root->left) == 0 && height(root->right) == 1){
        return true;
    }
    else if(height(root->right) == 0 && height(root->left) == 1){
        return true;
    }
    return false;
}

int height(Node* root) {
    if (root == nullptr) return 0;
    if(height(root->left) >= height(root->right)){
        return 1+height(root->left);
    }
    else{
        return 1+height(root->right);
    }
}