#include "../include/dsl.h"


int main() 
    {    
    Tree tree = {};

    TreeCtor(&tree);

    Text data = {};

    Data type = {};

    Read read = {};

    FILE* file_input = fopen("input.txt", "r");

    FileInput(file_input, &data);

    char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN] = {};

    TreeRead(&tree, &data, &read, Variables);

    Optimize(&tree, &tree.root);
    
    char variable[MAX_VARIABLE_LEN] = {};

    size_t ind_param = 0;

    puts("on which variable do you want to differentiate?");

    scanf("%s", &variable);

    if (FindVariable(&ind_param, variable, Variables) == false) 
        {
        printf("there is no such variable in your example\n");
        }

    else
        {
        TreeDump(&tree, tree.root, Variables);

        srand(time(NULL));
        
        Tree diff_tree = {};

        TreeCtor(&diff_tree);

        TexDumpBegin();

        TexDump(tree.root, tree.root, "\\ Производная Функции \\\\", variable, false, Variables);

        diff_tree.root = Diff(&tree, type, tree.root, ind_param, Variables, true);

        TreeDump(&diff_tree, diff_tree.root, Variables);

        Optimize(&tree, &diff_tree.root);

        TreeDump(&diff_tree, diff_tree.root, Variables);

        TexDump(tree.root, diff_tree.root, "В результате получаем: \\\\", variable, true, Variables);

        Tree_type x_dot = 0;

        printf("In which premises do you want to obtain Taylor's permission?\n");

        scanf("%lf", &x_dot);

        GetTaylor(type, &tree, tree.root, x_dot, ind_param, Variables, variable);

        TexDestroy();

        TreeDtor(&diff_tree);
        }
    
    TreeDtor(&tree);

    return 0;
    }