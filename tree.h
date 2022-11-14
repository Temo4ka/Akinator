#include "stack.h"
#ifndef Elem_t
    #include "treeType.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum TreeErrors {
             TreeIsOk         =       0,
            TreeIsNull        = 1 <<  0,
            TreeIsFull        = 1 <<  1, 
            TreeIsEmpty       = 1 <<  2,
          TreeHeadErase       = 1 <<  3,
          TreeDataIsNull      = 1 <<  4,
          TreeNextIsNull      = 1 <<  5,
          TreeWrongIndex      = 1 <<  6,
          TreePrevIsNull      = 1 <<  7,
          TreeIsInActive      = 1 <<  8,
         TreePoisonInData     = 1 <<  9,
         TreeFreeBlocksErr    = 1 << 10,
       TreeDoubleDestruction  = 1 << 11,
      TreeDoubleConstruction  = 1 << 12,
     TreeGraphVizExecutionErr = 1 << 13,
    TreeInputUserCommandError = 1 << 14
};

enum TreeStatus {
     Active = 0,
    InActive = 1
};

enum Result {
    NotFound = 0,
     Found   = 1, 
};

struct TreeInfo {
    size_t  line   =    0   ;

    char *  name   = nullptr;
    char *  file   = nullptr;
    char * pointer = nullptr;
    char *function = nullptr;
};

struct TreeNode {
    Elem_t data =  POISON;
 
    TreeNode  *rgt  = nullptr;
    TreeNode  *lft  = nullptr;
};


struct Tree {
    TreeNode *tree   =    {}   ;
    TreeInfo  info   =    {}   ;
     size_t   size   =    0    ;
     bool    status  = InActive;
};

#define treeCtor(TREE) {                                                    \
    _treeCtor((TREE), #TREE, __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
}

#define catchNullptr(POINTER) {                                                                                    \
    if ((POINTER) == nullptr) {                                                                                     \
        fprintf(stderr, "%s pointer at %s at %s(%d) is NULL\n", #POINTER, __PRETTY_FUNCTION__, __FILE__, __LINE__);  \
        return EXIT_FAILURE;                                                                                          \
    }                                                                                                                  \
}

#define treeDump(TREE) {                                             \
    treeDump_((TREE), __PRETTY_FUNCTION__, __FILE__, __LINE__);    \
}

int treeNodeCtor(TreeNode *elem, const Elem_t val = POISON);

int _treeCtor(Tree *root, const char * name, const char *file, const char *function, size_t line);

int treeDtor(Tree *root);

int treeExecuteNode(TreeNode *node);

int treeExecuteLeaf(TreeNode *node);

int treePrintDefRequest(Tree *head, const char *request, int *err);

int treePrintCompareRequest(Tree *head, const char *request1, const char *request2, int* err);

int treeGetDefinition(TreeNode *tree, Stack *def, const char *request, int *err);

int treePrintComp(Stack *def1, Stack *def2, const char *request1, const char *request2);

int stackDefPrint(Stack *def, const char *request);

// void treeDump_(Tree *tree, const char* functionName, const char *fileName, size_t line);

int treeLoadBase(Tree *head, const char *fileName);

int treeBaseScanf(TreeNode **node, char **buffer);

int treeNew(TreeNode **node, char *curCmd);

int treeMakeBase(Tree *head, FILE *stream = stdout);

int treeBasePrint(TreeNode *node, char *nullString, FILE *stream = stdout);

const char *treeGraphVizDump(Tree *tree, const char *fileName, int cmd);

void treePrintNode(TreeNode *tree, size_t *cur, FILE *stream = stdout);

void printErrorMessage(int error);

void treeLogClose();

void flash();

void tell(const char * format, ...);