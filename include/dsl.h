#pragma once


#include "differentiator.h"



/*#define OP    arg->oper
#define TYPE  value->type
#define NUM   value->num
#define VAR   value->var
#define OPER  value->oper
#define LEFT  node->left 
#define RIGHT node->right*/


#define SIGN(x) ((x > 0) - (x < 0));                                    


#define DELETE(way) TreeDelete(tree, node->way);    \
                    node = nullptr;                  \



#define DIFF_L Diff(data, node->left,  ind_param)
#define DIFF_R Diff(data, node->right, ind_param)
#define COPY_L CopyNode(node->left)
#define COPY_R CopyNode(node->right)


#define D_ADD(L, R)  CreateNode({.op = ADD},  OP, L, R)
#define D_SUB(L, R)  CreateNode({.op = SUB},  OP, L, R)
#define D_MULT(L, R) CreateNode({.op = MULT}, OP, L, R)
#define D_DIV(L, R)  CreateNode({.op = DIV},  OP, L, R)
#define D_SIN(L, R)  CreateNode({.op = SIN},  OP, L, R)
#define D_COS(L, R)  CreateNode({.op = COS},  OP, L, R)
#define D_LN(L, R)  CreateNode({.op = LN},   OP, L, R)
#define D_POW(L, R)  CreateNode({.op = POW},  OP, L, R)