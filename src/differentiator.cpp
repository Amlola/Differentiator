#include "../include/dsl.h"




Node* Diff(Tree* tree, Data data, Node* node, size_t ind_param, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN], bool derivative) 
    {
    if (TYPE == NUM) 
        return D_NUM(0.0);

    if (TYPE == VAR)
        { 
        if (DATA.ind_var_arr == ind_param)
            return D_NUM(1.0);
        
        else
            return D_NUM(0.0);
        }

    Node* result = nullptr;

    switch (DATA.op)
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
            if (!ContainsVar(RIGHT))
                {
                result = D_MULT(D_MULT(COPY_R, D_POW(COPY_L, D_NUM(RIGHT->data.num - 1))), DIFF_L);
                }

            else if (!ContainsVar(LEFT))
                result = D_MULT(D_MULT(D_POW(COPY_L, COPY_R), D_NUM(log(LEFT->data.num))), DIFF_R);

            else
                result = D_MULT(D_POW(COPY_L, COPY_R), D_ADD(D_MULT(D_LN(nullptr, COPY_L), DIFF_R), D_MULT(D_DIV(DIFF_L, COPY_L), COPY_R)));
            break;
            }
        
        default:
            printf("Error\n");
            return nullptr;
        }

        Optimize(tree, &result);

        if (derivative) 
            TexDump(node, result, GetRandomPhraze(), Variables[DATA.ind_var_arr], false, Variables);

    return result;
    }



Node* D_NUM(double number) 
    {
    return CreateNode({.num = number}, NUM, nullptr, nullptr);
    }


Tree_type Eval(Node* node, Tree_type var_data)
    {
    if (!node) 
        {
        return NODE_PTR_IS_NULL;
        }

    if (TYPE == NUM) 
        return DATA.num;

    if (TYPE == VAR) 
        return var_data;


    Tree_type left_data  = Eval(LEFT, var_data);

    Tree_type right_data = Eval(RIGHT, var_data);

    switch (DATA.op)
        {
        case ADD:  
            return left_data + right_data;

        case SUB:  
            return left_data - right_data;

        case MULT: 
            return left_data * right_data;

        case DIV:  
            return left_data / right_data;

        case SIN:  
            return sin(right_data);

        case COS:  
            return cos(right_data);

        case POW:  
            return pow(left_data, right_data);

        case LN:   
            return log(right_data);
        }

    return 0;
    }



bool ContainsVar(Node* node) 
    {
    bool left = false;

    bool right = false;

    if (TYPE == VAR) 
        {
        return true;
        }

    if (TYPE == NUM) 
        {
        return false;
        }
    
    if (LEFT)
        left = ContainsVar(LEFT);

    if (RIGHT)
        right = ContainsVar(RIGHT);

    if (left || right) 
        {
        return true;
        }

    return false;
    }



void GetTaylor(Data data, Tree* tree, Node* root, Tree_type x_dot, size_t ind_param, 
                                char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN], char variable[MAX_VARIABLE_LEN])
    {
    const size_t degree = 3;

    print_("\\section{Разложение по Тейлору}");

    print_("\\begin{center}");

    print_("$");

    print_("f\\left(%s\\right) = ", variable);

    DumpNode(root, root, Variables);

    print_("$ \\end{center}\\ \n");

    print_("\\subsection{Решение}\\ \\newline");
    
    Node* cur_node = root;

    Tree_type taylor_coeffs[degree + 1] = {};

    size_t i = 0;

    while (i <= degree)
        {
        taylor_coeffs[i] = Eval(cur_node, x_dot);

        const char* phraze = GetRandomPhraze();

        print_("%s\n", phraze);

        print_("\\begin{center}");

        print_("$");

        print_("f^{\\left(%d\\right)} = ", i);

        DumpNode(cur_node, cur_node, Variables);

        print_("$\\end{center}\\ \n");

        print_("\\begin{center}");

        print_("$");

        print_("f^{\\left(%d\\right)}\\left(%d\\right) = ", i, (int)x_dot);

        if (CmpDouble(taylor_coeffs[i], (int)taylor_coeffs[i]) == 0) 
            print_("%d\n", (int)taylor_coeffs[i]);

        else
            print_("%lf\n", taylor_coeffs[i]);

        print_("$\\end{center}\\ \\newline \\\\ \n");

        Node* result = Diff(tree, data, cur_node, ind_param, Variables, false);

        Optimize(tree, &result);

        cur_node = result;

        i++;
        }

    GiveAnswerTaylor(x_dot, variable, taylor_coeffs);
    }



void GiveAnswerTaylor(Tree_type x, char variable[MAX_VARIABLE_LEN], Tree_type taylor_coeffs[]) 
    {
    print_("\\subsection{Ответ}\\ \\newline");

    print_("В результате получаем разложение ряда Тейлора в точке %d:", (int)x);

    print_("\\begin{center}");

    print_("$");

    print_("f\\left(%s\\right) = ", variable);

    size_t j = 0;

    while (j <= 3) 
        {
        if (CmpDouble(taylor_coeffs[j], 0) == 0) 
            {
            j++;
            continue;
            }
        
        if (j == 0) 
            {
            if (CmpDouble(taylor_coeffs[j], (int)taylor_coeffs[j]) == 0)
                print_("\\frac{%d}{%d} + ", (int)taylor_coeffs[j], factorial(j));

            else
                print_("\\frac{%lf}{%d} + ", taylor_coeffs[j], factorial(j));

            j++;
            continue;
            }
        

        if (CmpDouble(x, 0) == 0)
            {
            if (CmpDouble(taylor_coeffs[j], (int)taylor_coeffs[j]) == 0)
                print_("\\frac{%d}{%d} \\cdot{(%s)^{%d}} + ", (int)taylor_coeffs[j], factorial(j), variable, j);

            else
                print_("\\frac{%lf}{%d} \\cdot{(%s)^{%d}} + ", taylor_coeffs[j], factorial(j), variable, j);
            }

        else
            {
             if (CmpDouble(taylor_coeffs[j], (int)taylor_coeffs[j]) == 0)
                print_("\\frac{%d}{%d} \\cdot{(%s - %d)^{%d}} + ", (int)taylor_coeffs[j], factorial(j), variable, (int)x, j);

            else
                print_("\\frac{%lf}{%d} \\cdot{(%s - %d)^{%d}} + ", taylor_coeffs[j], factorial(j), variable, (int)x, j);
            }
        
        j++;
        }
    
    if (CmpDouble(x, 0) == 0) 
        print_("o(%s^{3})", variable, (int)x);
    else 
        print_("o((%s - %d)^{3})", variable, (int)x);

    print_("$ \\end{center}\\ \n");
    }



void TexDump(Node* node1, Node* node2, const char* phraze, const char* var, bool optimize, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN])
    {
    if (node1 == node2) 
        {
        print_("\\section{%s}", phraze);

        print_("\\begin{center}");

        print_("$");

        print_("f\\left(%s\\right) = ", var);

        DumpNode(node1, node1, Variables);

        print_("$ \\end{center}\\  \n");

        print_("\\subsection{Решение}\\ \\newline");
        }
    
    else 
        {
        if (optimize)
            print_("\\subsection{Ответ}\\ \\newline");


        print_("%s\n", phraze);

        print_("\\begin{center}");

        print_("$");

        print_("\\left(");

        DumpNode(node1, node1, Variables);

        print_(" \\right)' = ");

        DumpNode(node2, node2, Variables);

        print_("$\\end{center}\\ \n");
        }

    }



void DumpNode(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN])
    {
    if (TYPE == NUM)
        {
        if (CmpDouble(DATA.num, 0) < 0) 
            print_("\\left(");

        if (CmpDouble((int)DATA.num, DATA.num) == 0) 
            {
            print_("%d", (int)DATA.num);
            }

        else 
            print_("%lf", DATA.num);

        if (CmpDouble(DATA.num, 0) < 0) 
            print_("\\right)");
        }

    if (TYPE == VAR) 
        print_("%s", Variables[DATA.ind_var_arr]);

    if (TYPE == OP)
        {
        if (DATA.op == SIN || DATA.op == COS || DATA.op == LN || DATA.op == CTG || DATA.op == TG)
            {
            if (PARENT)  
                {
                if (PARENT->data.op == SIN || PARENT->data.op == COS || PARENT->data.op == LN || PARENT->data.op == CTG || PARENT->data.op == TG) 
                    {
                    print_("\\left(");
                    GetRight(node, root, Variables);
                    print_("\\right)");
                    }

                else 
                    GetRight(node, root, Variables);
                }

            else
                GetRight(node, root, Variables);
            }

        else if (DATA.op == DIV) 
            {
            print_ ("\\frac{");

            DumpNode(LEFT, root, Variables);

            print_ ("}{");

            DumpNode(RIGHT, root, Variables);

            print_ ("}");   
            }

        else if (DATA.op == MULT) 
            {
            if (PARENT) 
                {
                if (PARENT->data.op == ADD || PARENT->data.op == MULT || PARENT->data.op == SUB || PARENT->data.op == DIV) 
                    {
                    GetLeftAndRightMult(node, root, Variables);
                    }
                 else 
                    {
                    print_("\\left(");

                    GetLeftAndRightMult(node, root, Variables);

                    print_("\\right)");
                    }
                }
            else 
                {
                GetLeftAndRightMult(node, root, Variables);
                }
            }


        else if (DATA.op == POW) 
            {
            if (LEFT->type == OP)
                print_("{\\left(");

            DumpNode(LEFT, root, Variables);

            if (LEFT->type == OP)
                print_("\\right)}");

            print_("%s", OperationArray[DATA.op - 1].op_char_name);

            if (RIGHT->type == OP) 
                print_("{\\left(");

            DumpNode(RIGHT, root, Variables);

            if (RIGHT->type == OP) 
                print_("\\right)}");
            }

        else 
            {
            if (PARENT)
                {
                if (!ContainsOnlyOneOp(node, root))
                    {
                    print_("\\left(");

                    GetLeftAndRight(node, root, Variables);

                    print_("\\right)");
                    }
            
                else 
                    {
                    GetLeftAndRight(node, root, Variables);
                    }
                }
                
            else 
                {
                GetLeftAndRight(node, root, Variables);
                }
            }
        }
    }



bool ContainsOnlyOneOp(Node* node, Node* root) 
    {
    bool contain = false;

    if (DATA.op == PARENT->data.op || PARENT->data.op == DIV || PARENT->data.op == POW)
        return true;
    
    else    
        return false;
    
    if (PARENT)
        contain = ContainsOnlyOneOp(PARENT, root);

    if (contain) 
        return true;

    return false;
    }



bool ContainsOnlyPow(Node* node) 
    {
    bool contain = false;

    if (DATA.op == POW) 
        return true;

    else
        return false;

    if (PARENT) 
        contain = ContainsOnlyPow(PARENT);

    if (contain)
        return true;

    return false;
    }


 
void GetLeftAndRight(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]) 
    {
    DumpNode (LEFT, root, Variables);

    print_("%s", OperationArray[DATA.op - 1].op_char_name);

    DumpNode (RIGHT, root, Variables);
    }



void GetLeftAndRightMult(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]) 
    {
    DumpNode(LEFT, root, Variables);

    print_(" \\cdot ");

    DumpNode(RIGHT, root, Variables);
    }



void GetRight(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]) 
    {
    print_("\\%s{", OperationArray[DATA.op - 1].op_char_name);

    DumpNode (RIGHT, root, Variables);

    print_("}");
    }


void TexDumpBegin()
    {
    tex_file = fopen("out.tex", "w");

    print_( "\\documentclass[a4paper,14pt]{extarticle}\n"
            "\\usepackage{graphicx}\n"
            "\\usepackage{ucs}\n"
            "\\usepackage[utf8x]{inputenc}\n"
            "\\usepackage[russian]{babel}\n"
            "\\usepackage{multirow}\n"
            "\\usepackage{mathtext}\n"
            "\\usepackage[T2A]{fontenc}\n"
            "\\usepackage{titlesec}\n"
            "\\usepackage{float}\n"
            "\\usepackage{empheq}\n"
            "\\usepackage{amsfonts}\n"
            "\\usepackage{amsmath}\n"
            "\\title{\\textbf{Лабораторная работа по взятию производной }}"
            "\\author{Чурсин Владимир Б01-305}\n"
            "\\begin{document}\n"
            "\\maketitle\n");
    }



int TexDestroy()
    {
    print_("\n");

    print_("\n");

    print_("\\section{Построение графика исходной функции}");

    print_("\\ Используя данные, полученные в пунктах 1 и 2, получаем график:\n");

    print_("\\begin{center} \\includegraphics[scale=0.8]{plot.png} \\end{center}\n");

    print_("\\end{document}");

    fclose(tex_file);

    tex_file = nullptr;

    system("pdflatex -interaction=batchmode out.tex"); 

    return 0;
    }



void Optimize(Tree* tree, Node** node)
    {
    Changes change = NO_CHANGES;

    while (true)
        {
        OptimizeConst(tree, *node, &change);

        OptimizeDeleteNeutrals(tree, node, *node, &change);

        if (change == NO_CHANGES) 
            break;

        change = NO_CHANGES;
        }
    }


bool OptimizeConst(Tree* tree, Node* node, Changes* change)
    {
    if (TYPE == NUM) 
        return true;

    if (TYPE == VAR) 
        return false;

    bool left  = false;

    bool right = false;

    if (LEFT)  
        left  = OptimizeConst(tree, LEFT, change);

    if (RIGHT) 
        right = OptimizeConst(tree, RIGHT, change);

    if (left && right)
        { 
        *change = HAS_CHANGES;
 
        DATA.num = Eval(node, 0);

        TYPE = NUM;

        TreeDelete(tree, LEFT);

        TreeDelete(tree, RIGHT);

        LEFT  = nullptr;
        
        RIGHT = nullptr;

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

    (*node)->type     = NUM;

    TreeDelete(tree, (*node)->left);

    TreeDelete(tree, (*node)->right);

    (*node)->left  = nullptr;

    (*node)->right = nullptr;
    }


Tree_type OptimizeDeleteNeutrals(Tree* tree, Node** parent, Node* node, Changes* change)
    {
    if (TYPE == NUM) 
        return DATA.num;

    if (TYPE == VAR) 
        return -1;

    Tree_type left  = NAN;

    Tree_type right = NAN;

    if (LEFT)  
        left  = OptimizeDeleteNeutrals(tree, &node, LEFT,  change);

    if (RIGHT) 
        right = OptimizeDeleteNeutrals(tree, &node, RIGHT, change);


    if (TYPE == OP)
        {
        switch (DATA.op)
            {
            case ADD:
                if (IsZero(left)) 
                    {
                    ReConnect(parent, node, RIGHT);

                    DELETE(left);

                    *change = HAS_CHANGES;
                    }

                else if (IsZero(right)) 
                    {
                    ReConnect(parent, node, LEFT);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }
                break;


            case SUB:
                if (IsZero(left))
                    {
                    LEFT->type = NUM;

                    LEFT->data.num = -1;

                    DATA.op = MULT;

                    *change = HAS_CHANGES;
                    }

                else if (IsZero(right)) 
                    {
                    ReConnect(parent, node, LEFT);

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
                    ReConnect(parent, node, LEFT);

                    DELETE(right);

                    *change = HAS_CHANGES;
                    }

                else if (IsOne(left)) 
                    {
                    ReConnect(parent, node, RIGHT);

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
                    ReConnect(parent, node, LEFT);

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



int factorial(int n)
    {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
    }



const char* GetRandomPhraze()
    {
    int phraze_num = (rand() % (sizeof(PHRAZES) / sizeof(const char*)));
    return PHRAZES[phraze_num];
    }