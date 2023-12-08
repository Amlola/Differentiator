#include "tree.h"

#include <random>
#include <time.h>



char Variable[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN] = {};


void TreeCtor(Tree* tree)
    {   
    tree->status = TREE_OK;

    tree->root = (Node*)calloc(1, sizeof(Node));

    tree->root->left = tree->root->right = tree->root->parent = nullptr;

    tree->root->type = NO_TYPE;

    tree->size = 0;
    }



Node* CreateNode(Data data, Types type, Node* left, Node* right)
    {
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    if (left)
        left->parent = new_node; 
        
    if (right)
        right->parent = new_node;
    
    new_node->left  = left;
    
    new_node->right = right;

    new_node->type  = type;

    if (type == NUM)
        {
        new_node->data.num = data.num;
        }

    else if (type == OP)
        {
        new_node->data.op = data.op;
        }

    else if (type == VAR)
        {
        new_node->data.ind_var_arr = data.ind_var_arr; 
        }

    else
        {
        return nullptr;
        }


    return new_node;    
    }



Node* GetG(Read* read)
    {
	Node* val = GetE(read);

    printf("END INDEX: %d\n", read->index);

	return val;
    }


Node* GetN(Read* read)
    {
    return CreateNode({.num = read->str[read->index++].data.num}, NUM, nullptr, nullptr);
    }



Node* GetE(Read* read)
    {
	Node* val = GetT(read);

	while (read->str[read->index].data.op == ADD || read->str[read->index].data.op == SUB)
	    {
        size_t old_index = read->index;

		read->index++;

		Node* val2 = GetT(read);

        val = CreateNode({.op = read->str[old_index].data.op}, OP, val, val2);
        }

	return val;
    }



Node* GetT(Read* read)  
    {
    Node* val = GetD(read);

	while (read->str[read->index].data.op == MULT || read->str[read->index].data.op == DIV)
	    {
        size_t old_index = read->index;

		read->index++;

		Node* val2 = GetD(read);

        val = CreateNode({.op = read->str[old_index].data.op}, OP, val, val2);
	    }

	return val;
    }



Node* GetD(Read* read)
    {
	Node* val = GetP(read);

	if (read->str[read->index].data.op == POW)
	    {
		read->index++;

		Node* val2 = GetD(read);
        
		val = CreateNode({.op = POW}, OP, val, val2);
	    }

	return val;
    }


Node* GetP(Read* read)
    {
	if (read->str[read->index].type == OP)
	    {
        if (read->str[read->index].data.op == OPEN_BRACKET)
            {
            read->index++;

            Node* val = GetE(read);

            read->index++;

            return val;
            }
        
        else 
            return GetF(read);
	    }

	else 
        return GetF(read);
    }



Node* GetF(Read* read)
    {	    
	if (read->str[read->index].type == OP)
	    {
        size_t old_index = read->index;

        read->index++;

		Node* val = GetP(read);

		return CreateNode({.op = read->str[old_index].data.op}, OP, nullptr, val);
	    }

	else 
        return GetV(read);
    }



Node* GetV(Read* read)
    {
	if (read->str[read->index].type == VAR)
	    {
        size_t old_index = read->index;

        read->index++;

		return CreateNode({.ind_var_arr = read->str[old_index].data.ind_var_arr}, VAR, nullptr, nullptr);
	    }

	else 
        return GetN(read);
    }



Type_error ParseOperation(Text* data, size_t* i, Operations* type)
    {    
    if (strncmp("sin", data->Buf + *i, 3) == 0)
        {
        (*type) = SIN;
        (*i) += 3;
        }
        
    else if (strncmp("cos", data->Buf + *i, 3) == 0)
        {
        *type = COS;
        (*i) += 3;        
        }

    else if (strncmp("tg", data->Buf + *i, 2) == 0)
        {
        *type = TG;
        (*i) += 2;
        }

    else if (strncmp("ctg", data->Buf + *i, 3) == 0)
        {
        *type = CTG;
        (*i) += 3;
        }

    else if (strncmp("ln", data->Buf + *i, 2) == 0)
        {
        *type = LN;
        (*i) += 2;
        }

    else if (strncmp("lg", data->Buf + *i, 2) == 0)
        {
        *type = LG;
        (*i) += 2;
        }

    else if ('+' == data->Buf[*i])
        {
        *type = ADD;
        (*i) += 1;
        }

    else if ('-' == data->Buf[*i])
        {
        *type = SUB;
        (*i) += 1;
        }

    
    else if ('*' == data->Buf[*i])
        {
        *type = MULT;
        (*i) += 1;
        }

    
    else if ('/' == data->Buf[*i])
        {
        *type = DIV;
        (*i) += 1;
        }

    
    else if ('^' == data->Buf[*i])
        {
        *type = POW;
        (*i) += 1;
        }

    else 
        {
        *type = UKNOWN_TYPE;
        }
    
    return TREE_OK;
    }


Type_error TreeRead(Tree* tree, Text* data, Read* read)
    {
    Token* tokens = (Token*)calloc(MAX_TOKENS, sizeof(Token));

    Lexer(&tokens, data);

    read->str = tokens;

    tree->root = GetG(read);
    
    return tree->status;
    }



void Lexer(Token** tokens, Text* data) 
    { 
    size_t i = 0;

    size_t count_token = 0;

    i = SkipSpaces(data, i);

    size_t p = 0;

    while (data->Buf[i] != '\0') 
        {
        i = SkipSpaces(data, i);

        if (data->Buf[i] == '(') 
            {
            (*tokens)[count_token].type = OP;

            (*tokens)[count_token].data.op = OPEN_BRACKET;

            i++;
            }

        else if (data->Buf[i] == ')') 
            {
            (*tokens)[count_token].type = OP;

            (*tokens)[count_token].data.op = CLOSE_BRACKET;

            i++;
            }

        else if (isdigit(data->Buf[i])) 
            {
            double val = 0;

	        int check  = 0;

            sscanf(data->Buf + i, "%lf%n", &val, &check);

            (*tokens)[count_token].data.num = val;

            (*tokens)[count_token].type = NUM;

            i += check;
            }

        else 
            {
            Operations type = UKNOWN_TYPE;

            ParseOperation(data, &i, &type);

            if (type != UKNOWN_TYPE) 
                {
                (*tokens)[count_token].type = OP;

                (*tokens)[count_token].data.op = type; 
                }
            
            else 
                {
                (*tokens)[count_token].type = VAR;

                size_t ind = 0;

                while (isalpha(data->Buf[i]))
                    {
                    Variable[p][ind++] = data->Buf[i++];
                    }

                Variable[p][ind] = '\0'; 

                if (p == 0) 
                    {
                    (*tokens)[count_token].data.ind_var_arr = p;

                    p++;
                    }

                else 
                    {
                    if (strcmp(Variable[p], Variable[p - 1]) == 0)
                        (*tokens)[count_token].data.ind_var_arr = p - 1;

                    else 
                        {
                        (*tokens)[count_token].data.ind_var_arr = p;

                        p++;
                        }
                    }
                }
            }
        
        count_token++;
        }

    printf("COUNT TOKEN: %d\n", count_token);
    
    (*tokens)[count_token].type = OP;

    (*tokens)[count_token].data.op = END;
    }



bool FindVariable(size_t* ind_param, char variable[MAX_VARIABLE_LEN]) 
    {
    for (size_t i = 0; i < sizeof(Variable) / sizeof(Variable[0][0]); i++) 
        {
        if (strcmp(variable, Variable[i]) == 0) 
            {
            *ind_param = i;

            return true;
            }
        }

    return false;
    }




Node* CopyNode(Node* node)
    {
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->type = node->type;

    switch (node->type)
        {
        case NUM:
            {
            new_node->data.num = node->data.num;
            break;
            }

        case OP:
            {
            new_node->data.op = node->data.op;
            break;
            }    

        case VAR:
            {
            new_node->data.ind_var_arr = node->data.ind_var_arr;
            break;
            }
        }

    if (node->left)
        new_node->left  = CopyNode(node->left);
    
    if (node->right)
        new_node->right = CopyNode(node->right);

    return new_node;
    }



Type_error TreeDelete(Tree* tree, Node* node)  
    {
    if (node == nullptr) 
        {
        return NODE_PTR_IS_NULL;
        }

    if (node->left) 
        {
        TreeDelete(tree, node->left);
        }

    if (node->right) 
        {
        TreeDelete(tree, node->right);
        }

    node->right  = nullptr;

    node->left   = nullptr;

    node->parent = nullptr;

    free(node);  
 
    tree->size--; 

    return tree->status;  
    }



Type_error TreeDtor(Tree* tree) 
    {
    if (tree->root == nullptr) 
        {
        tree->status |= TREE_ROOT_IS_NULL;

        return TREE_ROOT_IS_NULL;
        }

    TreeDelete(tree, tree->root);

    tree->root = nullptr;

    tree->size = 0;

    return tree->status;
    }



Type_error CheckTreeLinks(Tree* tree, Node* node)
    {    
    if (node->left != nullptr && node->right != nullptr && (node->left->parent != node || node->right->parent != node))
        {
        tree->status |= TREE_LINKING_ERROR;

        return TREE_LINKING_ERROR;
        }

    if (node->left != nullptr)
        CheckTreeLinks(tree, node->left);
    
    if (node->right != nullptr)
        CheckTreeLinks(tree, node->right);

    return TREE_OK;
    }



bool TreeVerify(Tree* tree) 
    {
    if (tree == nullptr) 
        {
        tree->status |= TREE_IS_NULL;
        }

    if (tree->root == nullptr) 
        {
        tree->status |= TREE_ROOT_IS_NULL;
        }

    else 
        {
        CheckTreeLinks(tree, tree->root);
        }

    if (tree->size < 0) 
        {
        tree->status != TREE_SIZE_LESS_THAN_ZERO;
        }

    return tree->status;
    }


void NodeDump(Node* node, size_t* number_of_node, Child child, const char* color) 
    {
    PrintGraphNode(node, number_of_node, child, color);

    size_t current_number_of_node = *number_of_node;

    if (node->left)
        {
        PrintGraphEdge(current_number_of_node, ++(*number_of_node), L_CHILD, "lime");

        NodeDump(node->left, number_of_node, L_CHILD, "lime");
        }

    if (node->right)
        {
        PrintGraphEdge(current_number_of_node, ++(*number_of_node), R_CHILD, "aqua");

        NodeDump(node->right, number_of_node, R_CHILD, "aqua");
        } 
    }


void PrintGraphEdge(size_t from, size_t to, Child child, const char* color) 
    {
    print("node%d->node%d [color = \"%s\"];\n", from, to, color);
    }


void PrintGraphNode(Node* node, size_t* number_of_node, Child child, const char* color) 
    {
    if (node->type == OP)
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p}\"];\n", 
                                              *number_of_node, color, node, OperationArray[node->data.op - 1].op_char_name, node->parent, node->left, node->right);

    else if (node->type == NUM) 
        {
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %lf | PARENT: %p | LEFT: %p | RIGHT: %p}\"];\n", 
                                              *number_of_node, color, node, node->data.num, node->parent, node->left, node->right);  
        }

    else if (node->type == VAR) 
        {
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p}\"];\n", 
                                              *number_of_node, color, node, Variable[node->data.ind_var_arr], node->parent, node->left, node->right);  
        }
    }




void TreeDumpFunction(Tree* tree, Node* node, const char* path, const char* signature, unsigned line) 
    {
    logfile = fopen("logfile.html", "a");

    fprintf(logfile, "<pre>\n");

    fprintf(logfile, "<font size = \"+1\">path: %s\n</font>", path);

    fprintf(logfile, "<font size = \"+1\">in function: %s\n</font>", signature);

    fprintf(logfile, "<font size = \"+1\">line: %d\n</font>", line);

    if (tree->status != TREE_OK) 
        {
        for (size_t j = 0; j < NUMBER_OF_ERROR; j++)
            {
            if ((tree->status & (1 << j)))
                {
                fprintf(logfile, "<font color = \"red\">ERROR: %s\n</font>", ErrorMas[j + 1].NameError);
                }
            }
        }

    else 
        {
        size_t number_of_node = 0;

        graph_file = fopen("dotfile1.dot", "wb");

        static int dump_number = 1;

        print("digraph struct {bgcolor=RosyBrown rankdir = HR\n\n\n");

        print("label = \"tree_dump from function %s, Tree/%s:%d\";\n", signature, path, line);

        NodeDump(node, &number_of_node, L_CHILD, "red");

        print("\n\n}");

        fclose(graph_file);

        char shell_command[MAX_COMMAND_LENGTH] = "";
            sprintf(shell_command, "dot -Tpng dotfile1.dot -o graph%d.png", dump_number);
            system(shell_command);

        fprintf(logfile, "<img  width=\"1100px\" height=\"600px\" src=\"graph%d.png\">", dump_number);

        fprintf(logfile, ".\n");

        fprintf(logfile, ".\n");

        fprintf(logfile, ".\n");
                
        fclose(logfile);

        dump_number++;
        } 
    }


long long GetFileSize(FILE* file)
    {
    assert(file);

    long long start = ftell(file);
    fseek(file, start, SEEK_END);

    long long bufsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return bufsize;
    }



void FileInput(FILE* file, Text* data)
    {
    data->BufSize = GetFileSize(file);

    data->Buf = (char*)calloc(data->BufSize + 1, sizeof(char));

    fread(data->Buf, sizeof(char), data->BufSize, file);

    data->Buf[data->BufSize] = '\0';

    fclose(file);
    }



int SkipSpaces(Text* data, size_t i) 
    {
    while (isspace(data->Buf[i])) 
        {
        i++;
        }
    
    return i;
    }



void TexDump(Node* node1, Node* node2, const char* phraze)
    {
    if (node1 == node2) 
        {
        print_("\\section{%s}", phraze);

        print_("$");

        DumpNode(node1, node1);

        print_("$ \\newline \\\\ \n");
        }
    
    else 
        {
        print_("%s\n", phraze);

        print_("$");

        print_("(");

        DumpNode(node1, node1);

        print_(")' = ");

        DumpNode(node2, node2);

        print_("$ \\newline \\\\ \n");
        }

    }



void DumpNode(Node* node, Node* root)
    {
    if (!node) return;

    if (node->type == NUM)
        {
        if (CmpDouble(node->data.num, 0) < 0) 
            print_("(");

        if (CmpDouble((int)node->data.num, node->data.num) == 0) 
            {
            print_("%d", (int)node->data.num);
            }

        else 
            print_("%lf", node->data.num);

        if (CmpDouble(node->data.num, 0) < 0) 
            print_(")");
        }

    if (node->type == VAR) 
        print_("%s", Variable[node->data.ind_var_arr]);

    if (node->type == OP)
        {
        if (node->data.op == SIN || node->data.op == COS || node->data.op == LN || node->data.op == CTG || node->data.op == TG)
            {
            print_("\\%s{(", OperationArray[node->data.op - 1].op_char_name);

            DumpNode (node->right, root);

            print_(")}");
            }

        else if (node->data.op == DIV) 
            {
            print_ ("\\frac{");

            DumpNode(node->left, root);

            print_ ("}{");

            DumpNode(node->right, root);

            print_ ("}");   
            }

        else 
            {
            if (node != root) print_("(");

            DumpNode (node->left, root);

            print_ ("%s", OperationArray[node->data.op - 1].op_char_name);

            DumpNode (node->right, root);

            if (node != root) print_(")");
            }
        }
    }



const char* GetRandomPhraze()
    {
    tm time = {};
    size_t phraze_num = (size_t)(rand() % (sizeof(PHRAZES) / sizeof(const char*)));
    return PHRAZES[phraze_num];
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

    print_("\\end{document}");

    fclose(tex_file);

    tex_file = nullptr;

    system("pdflatex out.tex");

    // -interaction=batchmode 

    return 0;
    }