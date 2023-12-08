#include "dsl.h"


int main() 
    {
    Tree tree = {};

    TreeCtor(&tree);

    Text data = {};

    Data type = {};

    Read read = {};

    FILE* file_input = fopen("input.txt", "r");

    FileInput(file_input, &data);

    TreeRead(&tree, &data, &read);
    
    TreeDump(&tree, tree.root);

    char variable[MAX_VARIABLE_LEN] = {};

    size_t ind_param = 0;

    puts("on which variable do you want to differentiate?");

    scanf("%s", &variable);

    if (FindVariable(&ind_param, variable) == false) 
        {
        printf("there is no such variable in your example\n");
        }

    else 
        {
        Tree diff_tree = {};

        TreeCtor(&diff_tree);

        TexDumpBegin();

        TexDump(tree.root, tree.root, "\\ Функция \\\\");

        print_("\\section{Решение}");

        diff_tree.root = Diff(type, tree.root, ind_param);

        TreeDump(&tree, diff_tree.root);

        Optimize(&tree, &diff_tree.root);

        TexDump(tree.root, diff_tree.root, "Итого имеем: \\\\");

        TexDestroy();

        TreeDump(&tree, diff_tree.root);
        }
    
    TreeDtor(&tree);

    return 0;
    }