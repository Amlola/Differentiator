#pragma once


#include "differentiator.h"



#define DIFF_L Diff(data, node->left)
#define DIFF_R Diff(data, node->right)
#define COPY_L CopyNode(node->left)
#define COPY_R CopyNode(node->right)


#define D_ADD(L, R)  CreateNode({.op = ADD},  OP, L, R)
#define D_SUB(L, R)  CreateNode({.op = SUB},  OP, L, R)
#define D_MULT(L, R) CreateNode({.op = MULT}, OP, L, R)
#define D_DIV(L, R)  CreateNode({.op = DIV},  OP, L, R)
#define D_SIN(L, R)  CreateNode({.op = SIN},  OP, L, R)
#define D_COS(L, R)  CreateNode({.op = COS},  OP, L, R)
#define D_POW(L, R)  CreateNode({.op = POW},  OP, L, R)