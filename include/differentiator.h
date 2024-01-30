#pragma once



#include "tree.h"


enum Changes
    {
    NO_CHANGES  = 0,
    HAS_CHANGES = 1,
    };



Tree_type Eval(Node* node, Tree_type var_data);

Node* Diff(Tree* tree, Data data, Node* node, size_t ind_param, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN], bool derivative);

void Optimize(Tree* tree, Node** node);

bool OptimizeConst(Tree* tree, Node* node, Changes* change);

Node* D_NUM(double number); 

Tree_type OptimizeDeleteNeutrals(Tree* tree, Node** parent, Node* node, Changes* change);

void ReConnect(Node** parent, Node* node, Node* child);

bool IsOne(const double num); 

void GiveNum(Tree* tree, Node** node, const double num); 

int factorial(int n);

void GetTaylor(Data data, Tree* tree, Node* root, Tree_type x_dot, size_t ind_param, 
                                char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN], char variable[MAX_VARIABLE_LEN]);

void GiveAnswerTaylor(Tree_type x, char variable[MAX_VARIABLE_LEN], Tree_type taylor_coeffs[]); 

bool ContainsVar(Node* node);

bool ContainsOnlyOneOp(Node* node, Node* root); 

bool ContainsOnlyPow(Node* node);


void TexDumpBegin();

int TexDestroy();

void TexDump(Node* node1, Node* node2, const char* phraze, const char* var, bool optimize, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

const char* GetRandomPhraze();

void DumpNode(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

void GetLeftAndRight(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

void GetLeftAndRightMult(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]);

void GetRight(Node* node, Node* root, char Variables[MAX_COUNT_VARIABLE][MAX_VARIABLE_LEN]); 

int SkipSpaces(Text* data, size_t i);


const char* const PHRAZES[] =
    {
    "Поэтому в силу непрерывности функции: \\\\ \n",
    "В таком виде она может быть рассмотрена, как суперпозиция: \\\\ \n",
    "Нетрудно видеть, что: \\\\ \n",
    "Методом пристального взгляда получаем: \\\\ \n",
    "Очевидно, что: \\\\ \n",
    "Заметим, что: \\\\ \n",
    "Теперь докажем теорему 4.17 из приложения 2.18 по определению 2.18.28: \\\\ \n",
    "Необходимо сделать предостережение о неверном применении правила Лопиталя: \\\\ \n",
    "При этом f(x) и g(x) могут вести себя как угодно: \\\\ \n",
    "Аналогично определению 4.3 кривой на плоскости дается определение: \\\\ \n",
    "Гладкая кривая в любой точке имеет: \\\\ \n",
    "Применим метод индукции: \\\\ \n",
    "Теорема Ферма дает необходимое условие: \\\\ \n",
    "Необходимые условия выпуклости: \\\\ \n"
    };