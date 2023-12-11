#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include <assert.h>
#include "ctype.h"
#include <math.h>
#include <random>
#include <time.h>



#define PARENT node->parent
#define LEFT node->left
#define RIGHT node->right
#define TYPE node->type
#define DATA node->data


#define CHECK_TREE_ERROR(tree)                           \
                    if (TreeVerify(tree) != TREE_OK)     \
                        {                                \
                        ON_DUMP                          \
                            (                            \
                            TreeDump(tree);              \
                            )                            \
                        return tree->status;             \
                        }


#ifdef DUMP
    #define ON_DUMP(...) __VA_ARGS__
    #define TreeDump(tree_ptr, node_ptr, Variables) TreeDumpFunction(tree_ptr, node_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__, Variables);

#else
    #define ON_DUMP(...)
#endif

typedef double Tree_type;

typedef int Type_error;

typedef int Syn_error;

const int MAX_VARIABLE_LEN = 10;

const int MAX_COUNT_VARIABLE = 20;

const int MAX_TOKENS = 400;

static FILE* graph_file = NULL;

static FILE* tex_file = NULL;

static FILE* logfile = NULL;

const int MAX_COMMAND_LENGTH = 125;

const int MAX_OP_LEN = 10;

#define print(...) fprintf(graph_file, __VA_ARGS__)

#define print_(...) fprintf(tex_file, __VA_ARGS__)

#define SPECIFICATOR_TYPE "%s "


enum Tree_status
    {
    TREE_OK                       = 0,
    TREE_IS_NULL                  = 1 << 0,
    TREE_ROOT_IS_NULL             = 1 << 1,
    NODE_PTR_IS_NULL              = 1 << 2,  
    TREE_CANT_HAVE_THIS_CHILD     = 1 << 3,
    TREE_SIZE_LESS_THAN_ZERO      = 1 << 4,
    TREE_LINKING_ERROR            = 1 << 5,
    NUMBER_OF_ERROR               = 6
    };


struct TREE_STATUS
    {
    Tree_status CodeError;
    const char* NameError;
    };


const TREE_STATUS ErrorMas[] = 
    {
    {TREE_OK,                     "NO ERROR"},
    {TREE_IS_NULL,                "TREE_IS_NUL"},
    {TREE_ROOT_IS_NULL,           "TREE_ROOT_IS_NULL"},
    {NODE_PTR_IS_NULL,            "NODE_PTR_IS_NULL"},
    {TREE_CANT_HAVE_THIS_CHILD,   "TREE_CANT_HAVE_THIS_CHILD"},
    {TREE_SIZE_LESS_THAN_ZERO,    "TREE_SIZE_LESS_THAN_ZERO"},
    {TREE_LINKING_ERROR,          "TREE_LINKING_ERROR"}
    };


enum Operations 
    {
    UKNOWN_TYPE,
    ADD, 
    SUB,
    DIV,
    MULT, 
    COS, 
    SIN,
    POW, 
    TG, 
    CTG, 
    LN,
    LG,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    END
    };


struct Operation 
    {
    Operations op_name;
    char op_char_name[MAX_OP_LEN];
    };


const Operation OperationArray[] = 
    {
    {ADD,  "+"},
    {SUB,  "-"},
    {DIV,  "/"},
    {MULT, "*"},
    {COS,  "cos"},
    {SIN,  "sin"},
    {POW,  "^"},
    {TG,   "tg"},
    {CTG,  "ctg"},
    {LN,   "ln"},
    {LG,   "lg"},
    };


enum Types
    {
    NO_TYPE,
    OP,
    NUM,
    VAR,
    };


union Data
    {
    Tree_type num;
    Operations op;
    size_t ind_var_arr;
    };


struct Node 
    {
    Node* left;
    Node* right;
    Node* parent;
    Data  data;
    Types type;
    };


struct Tree 
    {
    Node* root;
    size_t size;
    Type_error status;
    };


struct Token
    {
    Types type;
    Data  data;
    Syn_error err;
    };


struct Read
    {
    size_t index;
    Token* str;
    };


enum Child
    {
    L_CHILD = 0,
    R_CHILD = 1,
    };


struct Text
    {
    char* Buf;
    long long BufSize;
    };



void TreeCtor(Tree* tree);

Type_error TreeDelete(Tree* tree, Node* node);

bool TreeVerify(Tree* tree);

Type_error TreeDtor(Tree* tree);

Node* CreateNode(Data data, Types type, Node* left, Node* right);

void TreeDumpFunction(Tree* tree, Node* node, const char* path, const char* signature, unsigned line, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]); 

void NodeDump(Node* node, size_t* number_of_node, Child child, const char* color, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]); 

void PrintGraphEdge(size_t from, size_t to, Child child, const char* color); 

void PrintGraphNode(Node* node, size_t* number_of_node, Child child, const char* color, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

long long GetFileSize(FILE* file);

void FileInput(FILE* file, Text* data);

Type_error TreeRead(Tree* tree, Text* data, Read* read, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

Type_error CheckTreeLinks(Tree* tree, Node* node);

Type_error ParseOperation(Text* data, size_t* i, Operations* type);

Node* GetV(Read* read);

Node* GetF(Read* read);

Node* GetP(Read* read);

Node* GetT(Read* read);

Node* GetE(Read* read);

Node* GetD(Read* read);

Node* GetN(Read* read);

Node* GetG(Read* read);

Node* CopyNode(Node* node);

void Lexer(Token** tokens, Text* data, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]); 

int SkipSpaces(Text* data, size_t i);

bool FindVariable(size_t* ind_param, char variable[MAX_VARIABLE_LEN], char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

bool IsZero(const double num);

int CmpDouble(const double a, const double b);
