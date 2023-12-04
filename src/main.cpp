#include "dsl.h"


int main() 
    {
    Tree tree = {};

    TreeCtor(&tree);

    Text data = {};

    Data type = {};

    Read read = {};

    InitRead(&read);

    FILE* file_input = fopen("input.txt", "r");

    FileInput(file_input, &data);

    TreeRead(&tree, &data, &read);
    
    TreeDump(&tree, tree.root);

    Node* diff = Diff(type, tree.root);

    TreeDump(&tree, diff);

    Optimize(&tree, diff);

    TreeDump(&tree, diff);

    TreeDtor(&tree);

    return 0;
    }