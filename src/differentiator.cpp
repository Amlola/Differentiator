#include "dsl.h"




Node* Diff(Data data, Node* node) 
    {
    if (node->type == NUM) 
        return D_NUM();

    if (node->type == VAR) 
        return D_VAR();

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
            result = D_DIV(D_SUB(D_MULT(DIFF_L, COPY_R), D_MULT(COPY_L, DIFF_R)), D_POW(COPY_R, FOR_DIV()));
            break;
            }

        case SIN:
            {
            result = D_MULT(D_COS(COPY_R, nullptr), DIFF_R);
            break;
            }

        case COS:
            {
            result = D_MULT(D_MULT(D_SIN(COPY_R, nullptr), DIFF_R), TO_NEGATIVE());
            break;
            }

        case LN:
            {
            result = D_MULT(D_DIV(D_VAR(), COPY_R), DIFF_R);
            break;
            }
        
        default:
            printf("Error\n");
            return nullptr;

        }
    return result;
    }



Node* D_NUM() 
    {
    CreateNode({.num = 0.0}, NUM, nullptr, nullptr);
    }


Node* D_VAR() 
    {
    CreateNode({.num = 1.0}, NUM, nullptr, nullptr);
    }


Node* FOR_DIV() 
    {
    CreateNode({.num = 2.0}, NUM, nullptr, nullptr);
    }


Node* TO_NEGATIVE()
    {
    CreateNode({.num = -1.0}, NUM, nullptr, nullptr);
    }


double Eval(Node* node)
    {
    if (!node) 
        {
        return NODE_PTR_IS_NULL;
        }

    if (node->type == NUM) 
        return node->data.num;

    double left_value  = Eval(node->left);

    double right_value = Eval(node->right);

    switch (node->data.op)
        {
        case ADD:  return left_value + right_value;
        case SUB:  return left_value - right_value;
        case MULT: return left_value * right_value;
        case DIV:  return left_value / right_value;
        case SIN:  return sin(right_value);
        case COS:  return cos(right_value);
        case POW:  return pow(left_value, right_value);
        }

    return 0;
    } 



void Optimize(Tree* tree, Node* node)
    {
    assert (node);

    bool has_optimized = false;

    while (true)
        {
        OptimizeConst(tree, node, &has_optimized);

        if (has_optimized == false) 
            return;

        has_optimized = false;
        }
    }


bool OptimizeConst(Tree* tree, Node* node, bool* has_optimized)
    {
    if (node->type == NUM) 
        return true;

    if (node->type == VAR) 
        return false;

    bool optimize_left  = false;
    bool optimize_right = false;

    if (node->left)  
        optimize_left  = OptimizeConst(tree, node->left, has_optimized);

    if (node->right) 
        optimize_right = OptimizeConst(tree, node->right, has_optimized);

    if (optimize_left && optimize_right)
        {  
        node->data.num = Eval(node);
        node->type = NUM;

        TreeDelete(tree, node->left);
        TreeDelete(tree, node->right);

        node->left  = nullptr;
        node->right = nullptr;

        *has_optimized = true;
        return true;
        }

    return false;
    }   