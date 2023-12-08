#include "dsl.h"




Node* Diff(Data data, Node* node, size_t ind_param) 
    {
    if (node->type == NUM) 
        return D_NUM(0.0);

    if (node->type == VAR)
        { 
        if (node->data.ind_var_arr == ind_param)
            return D_NUM(1.0);
        
        else
            return D_NUM(0.0);
        }

    Node* result = nullptr;

    switch (node->data.op)
        {
        case ADD:
            {
            result = D_ADD(DIFF_L, DIFF_R);
            break;
            }

        case SUB:
            {
            result = D_SUB(DIFF_L, DIFF_R);
            break;
            }

        case MULT:
            {
            result = D_ADD(D_MULT(DIFF_L, COPY_R), D_MULT(COPY_L, DIFF_R));
            break;
            }

        case DIV:
            {
            result = D_DIV(D_SUB(D_MULT(DIFF_L, COPY_R), D_MULT(COPY_L, DIFF_R)), D_POW(COPY_R, D_NUM(2.0)));
            break;
            }

        case SIN:
            {
            result = D_MULT(D_COS(nullptr, COPY_R), DIFF_R);
            break;
            }

        case COS:
            {
            result = D_MULT(D_MULT(D_SIN(nullptr, COPY_R), DIFF_R), D_NUM(-1.0));
            break;
            }

        case TG:
            {
            result = D_MULT(D_DIV(D_NUM(1.0), D_POW(D_COS(nullptr, COPY_R), D_NUM(2.0))), DIFF_R);
            break;
            }

        case CTG:
            {
            result = D_MULT(D_MULT(D_DIV(D_NUM(1.0), D_POW(D_SIN(nullptr, COPY_R), D_NUM(2.0))), DIFF_R), D_NUM(-1.0));
            break;
            }

        case LN:
            {
            result = D_MULT(D_DIV(D_NUM(1.0), COPY_R), DIFF_R);
            break;
            }

        case LG:
            {
            result = D_MULT(D_DIV(D_NUM(1.0), D_MULT(D_NUM(log(10.0)), COPY_R)), DIFF_R);
            break;
            }

        case POW:
            {
            if (node->right->type == NUM && node->left->type == VAR)
                result = D_MULT(D_MULT(COPY_R, D_POW(COPY_L, D_NUM(node->right->data.num - 1))), DIFF_L);

            else if (node->left->type == NUM && node->right->type == VAR)
                result = D_MULT(D_MULT(D_POW(COPY_L, COPY_R), D_NUM(log(node->left->data.num))), DIFF_R);

            else
                result = D_MULT(D_POW(COPY_L, COPY_R), D_ADD(D_MULT(D_LN(nullptr, COPY_L), DIFF_R), D_MULT(D_DIV(DIFF_L, COPY_L), COPY_R)));
            break;
            }
        
        default:
            printf("Error\n");
            return nullptr;
        }

    TexDump(node, result, GetRandomPhraze());

    return result;
    }


Node* D_NUM(double number) 
    {
    return CreateNode({.num = number}, NUM, nullptr, nullptr);
    }



Tree_type Eval(Node* node)
    {
    if (!node) 
        {
        return NODE_PTR_IS_NULL;
        }

    if (node->type == NUM) 
        return node->data.num;

    Tree_type left_data  = Eval(node->left);

    Tree_type right_data = Eval(node->right);

    switch (node->data.op)
        {
        case ADD:  return left_data + right_data;
        case SUB:  return left_data - right_data;
        case MULT: return left_data * right_data;
        case DIV:  return left_data / right_data;
        case SIN:  return sin(right_data);
        case COS:  return cos(right_data);
        case POW:  return pow(left_data, right_data);
        case LN:   return log(right_data);
        }

    return 0;
    }



void Optimize(Tree* tree, Node** node)
    {
    Changes change = NO_CHANGES;

    while (true)
        {
        OptimizeConst(tree, *node, &change);
        OptimizeNeutrals(tree, node, *node, &change);

        if (change == NO_CHANGES) 
            break;

        change = NO_CHANGES;
        }
    }


bool OptimizeConst(Tree* tree, Node* node, Changes* change)
    {
    if (node->type == NUM) 
        return true;

    if (node->type == VAR) 
        return false;

    bool left  = false;

    bool right = false;

    if (node->left)  
        left  = OptimizeConst(tree, node->left, change);

    if (node->right) 
        right = OptimizeConst(tree, node->right, change);

    if (left && right)
        { 
        *change = HAS_CHANGES;
 
        node->data.num = Eval(node);

        node->type = NUM;

        TreeDelete(tree, node->left);

        TreeDelete(tree, node->right);

        node->left  = nullptr;
        
        node->right = nullptr;

        return true;
        }

    return false;
    } 



void ReConnect(Node** parent, Node* node, Node* child) 
    {
    if (node == *parent)
        *parent = child;

    else
        {
        if (node == (*parent)->left) 
            (*parent)->left = child;

        else 
            (*parent)->right = child;
        }
    }


void GiveNum(Tree* tree, Node** node, const double num) 
    {
    (*node)->data.num = num;
    (*node)->type = NUM;

    TreeDelete(tree, (*node)->left);
    TreeDelete(tree, (*node)->right);

    (*node)->left  = nullptr;
    (*node)->right = nullptr;
    }


Tree_type OptimizeNeutrals(Tree* tree, Node** parent, Node* node, Changes* change)
    {
    if (node->type == NUM) 
        return node->data.num;

    if (node->type == VAR) 
        return -1;

    Tree_type left  = NAN;

    Tree_type right = NAN;

    if (node->left)  
        left  = OptimizeNeutrals(tree, &node, node->left,  change);

    if (node->right) 
        right = OptimizeNeutrals(tree, &node, node->right, change);


    if (node->type == OP)
        {
        switch (node->data.op)
            {
            case ADD:
                if (IsZero(left)) 
                    {
                    ReConnect(parent, node, node->right);

                    DELETE(left);

                    *change = HAS_CHANGES;
                    }

                else if (IsZero(right)) 
                    {
                    ReConnect(parent, node, node->left);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }
                break;


            case SUB:
                if (IsZero(left))
                    {
                    node->left->type = NUM;

                    node->left->data.num = -1;

                    node->data.op = MULT;

                    *change = HAS_CHANGES;
                    }

                else if (IsZero(right)) 
                    {
                    ReConnect(parent, node, node->left);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }
                break;

            case MULT:
                 if (IsZero(right) || IsZero(left)) 
                    {
                    GiveNum(tree, &node, 0.0);

                    *change = HAS_CHANGES;  
                    }
                
                else if (IsOne(right))
                    {
                    ReConnect(parent, node, node->left);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }

                else if (IsOne(left)) 
                    {
                    ReConnect(parent, node, node->right);

                    DELETE(left);

                    *change = HAS_CHANGES;  
                    }
                break;

            case DIV:
                if (IsZero(left)) 
                    {
                    GiveNum(tree, &node, 0.0);

                    *change = HAS_CHANGES;
                    }
                break;

            case POW:
                if (IsZero(left)) 
                    {
                    GiveNum(tree, &node, 0.0);

                    *change = HAS_CHANGES;
                    }

                else if (IsOne(left)) 
                    {
                    GiveNum(tree, &node, 1.0);

                    *change = HAS_CHANGES;
                    }

                else if (IsOne(right)) 
                    {
                    ReConnect(parent, node, node->left);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }
                break;

            default:
                break;
            }
        }
    return -1;
    }



bool IsZero(const double num) 
    {
    return CmpDouble(num, 0) == 0;
    }



bool IsOne(const double num) 
    {
    return CmpDouble(num, 1) == 0;
    }



int CmpDouble(const double a, const double b) 
    {
    const double EPS = 1e-9;

    if (fabs(b - a) < EPS) 
        {
        return 0;
        }

    return SIGN(a - b);
    }
