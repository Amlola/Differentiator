#pragma once



#include "tree.h"


/*enum Changes
    {
    NO_CHANGES =  0,
    HAS_CHANGES = 1,
    };*/


double Eval(Node* node);

Node* Diff(Data data, Node* node);

void Optimize(Tree* tree, Node* node);

bool OptimizeConst(Tree* tree, Node* node, bool* has_optimized);

Node* D_NUM();

Node* D_VAR();

Node* FOR_DIV();

Node* TO_NEGATIVE();


