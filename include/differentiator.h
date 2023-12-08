#pragma once



#include "tree.h"


enum Changes
    {
    NO_CHANGES =  0,
    HAS_CHANGES = 1,
    };


Tree_type Eval(Node* node);

Node* Diff(Data data, Node* node, size_t ind_param);

void Optimize(Tree* tree, Node** node);

bool OptimizeConst(Tree* tree, Node* node, Changes* change);

Node* D_NUM(double number); 

Tree_type OptimizeNeutrals(Tree* tree, Node** parent, Node* node, Changes* change);

void ReConnect(Node** parent, Node* node, Node* child);

bool IsOne(const double num); 

void GiveNum(Tree* tree, Node** node, const double num); 

