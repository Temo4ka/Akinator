#include "tree.h"
#include "config.h"
#define  TX_USE_SPEAK
#include "TXLib.h"
#include <cstdarg>
#include <sys/types.h>
#include <sys/stat.h>

static FILE *LogFile  = fopen("logs/logFile.txt", "w");

#define CUR_SYMB (*CUR_STR)

#define NEXT_SYMB CUR_STR++

#define CUR_STR (*buffer)

int treeNodeCtor(TreeNode *elem, const Elem_t val) {
    catchNullptr(elem);

    elem -> rgt  = nullptr;
    elem -> lft  = nullptr;

    elem -> data =   val  ;

    return TreeIsOk;
}


int _treeCtor(Tree *root, const char * name, const char *file, const char *function, size_t line) {
    catchNullptr(root);
    if (root -> status == Active) return TreeDoubleConstruction;

    root -> tree = (TreeNode *) calloc(1, sizeof(TreeNode));
    catchNullptr(root -> tree);

    int err = treeNodeCtor(root -> tree, "Random object");
    if (err) return err;

    root -> info. pointer = (char *)   root  ;
    root -> info.  name   = (char *)   name  ;
    root -> info.  file   = (char *)   file  ;
    root -> info.function = (char *) function;
    root -> info.  line   =        line      ;

    root ->     status    =      Active      ;

    return TreeIsOk;
}

int treeDtor(Tree *root) {
    catchNullptr(root);
    if (root -> status == InActive) return TreeDoubleDestruction;

    free(root -> tree);

    root -> status = InActive;

    return TreeIsOk;

}

int treeExecuteNode(TreeNode *node) {
    catchNullptr(node);

    if (node -> lft == nullptr && node -> rgt == nullptr)
        return treeExecuteLeaf(node);
    
    catchNullptr(node -> data);

    tell("Is it %s?", node -> data);
    printf("\n");

    char *userAns = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
    catchNullptr(userAns);

    while (strcmpi(userAns, "yes") && strcmpi(userAns, "no")) {
        printf("Please enter yes or no.\n", NULL);

        scanf("%s", userAns);

        if (strcmpi(userAns, "yes") && strcmpi(userAns, "no")) {
            tell("Wrong command", NULL);
            printf("!\n");
            printf("Please enter yes or no.\n", NULL);
        }
    }

    if (!strcmpi(userAns, "yes")) {
        free(userAns);

        return treeExecuteNode(node -> rgt);
    }
    if (!strcmpi(userAns, "no" )) {
        free(userAns);

        return treeExecuteNode(node -> lft);
    }

    return TreeInputUserCommandError;
}

int treeExecuteLeaf(TreeNode *node) {
    catchNullptr(    node    );
    
    if (node -> data == nullptr) node -> data = nullptr;

    tell("Your object is %s?\n", node -> data);
    
    char *userAns = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
    catchNullptr(userAns);

    while (strcmpi(userAns, "yes") && strcmpi(userAns, "no")) {
        flash();
        scanf("%s", userAns);

        if (strcmpi(userAns, "yes") && strcmpi(userAns, "no")) {
            tell("Wrong command", NULL);
            printf("!\n");
            printf("Please enter yes or no.\n", NULL);
        }
    }

    if (!strcmpi(userAns, "yes")) {
        tell("That was too easy, bratanskii.", NULL);
        tell("Maybe u will try ", NULL);
        tell("smth more difficult?", NULL);
        printf("\n");
    } else if (!strcmpi(userAns, "no" )) {
        tell("Ok than.", NULL);
        tell(" What was ur object?", NULL);
        printf("\n");

        char *newVal  = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
        char *newChar = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
        catchNullptr(newVal );
        catchNullptr(newChar);

        flash();
        newVal = fgets(newVal, MAX_STRING_SIZE, stdin);
        catchNullptr(newVal);
        *(strchr(newVal, '\n')) = '\0';

        node -> rgt = (TreeNode *) calloc(1, sizeof(TreeNode));
        int err = treeNodeCtor(node -> rgt, newVal);

        node -> lft = (TreeNode *) calloc(1, sizeof(TreeNode));
        err |= treeNodeCtor(node -> lft, node -> data);

        if (err) return err;

        tell("What characteristic", NULL);
        tell(" differs %s from %s?", newVal, node -> data);
        printf("\n");

        newChar = fgets(newChar, MAX_STRING_SIZE, stdin);
        catchNullptr(newChar);
        char *lastSymb = (strchr(newChar, '\n'));
        if (lastSymb != nullptr)
            *lastSymb =  '\0';
        node -> data = newChar;

        tell("Ok,", NULL);
        tell(" I will try to remember ", NULL); 
        tell("ur useless information,", NULL);
        tell(" UwU", NULL);
        printf("\n");
    } else {
        free(userAns);

        return TreeInputUserCommandError;
    }
    free(userAns);

    return TreeIsOk;
}

static void printElem(FILE *stream, const Elem_t stackElement = nullptr) {
    fprintf(stream, "%s", stackElement);
    fprintf(stderr, "%s", stackElement);
}

int treePrintDefRequest(Tree *head, const char *request, int *err) {
    if (head == nullptr || request == nullptr) return NotFound;

    Stack def = {};
    stackCtor(&def, printElem);

    int res = treeGetDefinition(head -> tree, &def, request, err);
    if (res == NotFound) return NotFound;

    *err |= stackDefPrint(&def, request);
    if (*err) return NotFound;

    return Found;
}

int treePrintCompareRequest(Tree *head, const char *request1, const char *request2, int* err) {
    if (head == nullptr || request1 == nullptr || request2 == nullptr) return NotFound;

    Stack def1 = {};
    Stack def2 = {};
    stackCtor(&def1, printElem);
    stackCtor(&def2, printElem);

    int res1 = treeGetDefinition(head -> tree, &def1, request1, err);
    int res2 = treeGetDefinition(head -> tree, &def2, request2, err);

    if (!res1) {
        tell("%s was not found!", request1);
        printf("\n");
    }
    if (!res2) {
        tell("%s was not found!", request2);
    }
    if (!res1 || !res2) return NotFound;

    *err |= treePrintComp(&def1, &def2, request1, request2);
    if (*err) return NotFound;

    return Found;
}

int treeGetDefinition(TreeNode *tree, Stack *def, const char *request, int *err) {
    if (tree == nullptr || request == nullptr) return NotFound;

    if (strcmpi(request, tree -> data)) {
        char *newCharacteristic = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
              newCharacteristic = (char *) strcpy(newCharacteristic, DENIAL);

        *err = stackPush(def, strcat(newCharacteristic, tree -> data));
        if (treeGetDefinition(tree -> lft, def, request, err)) return Found;
        if (*err) return NotFound;

        stackPop(def, err);
        *err |= stackPush(def, tree -> data);
        if (*err) return NotFound;

        if (treeGetDefinition(tree -> rgt, def, request, err)) return Found;
        if (*err) return NotFound;

        stackPop(def, err);
        if (*err) return NotFound;
    } else {
        return Found;
    }

    return NotFound;
}

int treePrintComp(Stack *def1, Stack *def2, const char *request1, const char *request2) {
    catchNullptr(  def1  );
    catchNullptr(  def2  );
    catchNullptr(request1);
    catchNullptr(request2);

    size_t commonChar = 0;
    tell("Common characteristic are: ", NULL);
    for (commonChar = 0; commonChar < def1 -> size && commonChar < def2 -> size; commonChar++ ) {
        if (strcmpi(def1 -> data[commonChar], def2 -> data[commonChar])) break;
        
        tell("%s, ", def1 -> data[commonChar]);
    }
    if (!commonChar)
        tell("none", NULL);
    if (commonChar < def1 -> size && commonChar < def2 -> size) {
        tell("\nBut %s: ", request1);
        for (size_t cur1 = commonChar; cur1 < def1 -> size; cur1++)
            tell("%s, ", def1 -> data[cur1]);

        tell("\nWhile %s: ", request2);
        for (size_t cur2 = commonChar; cur2 < def2 -> size; cur2++)
            tell("%s, ", def2 -> data[cur2]);
        printf("\n");

        return TreeIsOk;
    } 
    if (commonChar < def1 -> size) {
        tell("\nBut %s: ", request1);
        for (size_t cur1 = commonChar; cur1 < def1 -> size; cur1++)
            tell("%s, ", def1 -> data[cur1]);
        printf("\n");

    } 
    if (commonChar < def2 -> size) { //if
        tell("\nBut %s: ", request2);
        for (size_t cur2 = commonChar; cur2 < def2 -> size; cur2++)
            tell("%s, ", def2 -> data[cur2]);
        printf("\n");

        return TreeIsOk;
    }

    return TreeIsOk;
}

int stackDefPrint(Stack *def, const char *request) {
    catchNullptr(request);
    int err = TreeIsOk;

    tell("%s: ", request);
    while (def -> size) {
        tell("%s, ", stackPop(def, &err));
        if (err) return err;
    }
    printf("\n");

    return TreeIsOk;
}

// void treeDump_(Tree *tree, const char* functionName, const char *fileName, size_t line) {
//     myfPrintf(LogFile, "\n%s at %s(%zu);\n", functionName, fileName, line);
//     myfPrintf(LogFile, "Head[%08X] '%s' at '%s' at %s(%zu);\n{\n", tree->info.pointer, tree->info.name, tree->info.function, tree->info.file, tree->info.line);

//     myfPrintf(LogFile, "    head -> tree[%08X];\n");
//     myfPrintf(LogFile, "\n}\n");
// }

static assignBuffer(char **buffer, const char *fileName);

static size_t getFileSize(const char *fileName);

int treeLoadBase(Tree *head, const char *fileName) {
    catchNullptr(  head  );
    catchNullptr(fileName);

    char *buffer = nullptr;
    int err = assignBuffer(&buffer, fileName);
    if (err) return err;

    err = treeBaseScanf(&(head -> tree), &buffer);
    if (err) return err;

    // fprintf(stderr, "Suka!\n");

    return TreeIsOk;
}

static int assignBuffer(char **buffer, const char *fileName) {
    catchNullptr(fileName);

    size_t fileSize = getFileSize(fileName);

    FILE *stream = fopen(fileName, "r");
    catchNullptr(stream);

    *buffer = (char *) calloc(fileSize + 1, sizeof(char));
    size_t gotSymbols = fread(*buffer, sizeof(char), fileSize, stream);
    CUR_STR[gotSymbols] = '\0';

    if (!feof(stream)) return EXIT_FAILURE;

    fclose(stream);

    return EXIT_SUCCESS;
}

static size_t getFileSize(const char *fileName) {
    catchNullptr(fileName);

    struct stat buf = {};
    if (stat(fileName, &buf)) {
        fprintf(stderr, "An Error in reading file occured\n");
        return 0;
    }

    return buf.st_size;
}

static char *flashBuf(char *buffer);

static char *skipWord (char *buffer);

int treeBaseScanf(TreeNode **node, char **buffer) {
    catchNullptr(buffer);
    catchNullptr( node );

    int err = EXIT_SUCCESS;
    
    CUR_STR = flashBuf(CUR_STR);

    // fprintf(stderr, "!%c!\n", *buffer);

    if (CUR_SYMB != '{') return EXIT_FAILURE;

    NEXT_SYMB;
    CUR_STR = flashBuf(CUR_STR);

    if (CUR_SYMB == '}') {
        NEXT_SYMB;
        return EXIT_SUCCESS;
    }

    NEXT_SYMB;

    // CUR_STR  = (strchr(CUR_STR, '\"') + 1);
    // catchNullptr(CUR_STR);

    char  *curCmd  =       CUR_STR          ;
    catchNullptr(curCmd);
    char *lastSymb = (strchr(curCmd, '\"'));

    if (lastSymb != nullptr)
        *lastSymb =  '\0';

    NEXT_SYMB;
    CUR_STR = skipWord(CUR_STR);
    catchNullptr(CUR_STR);

    err |= treeNew(node, curCmd);

    // fprintf(stderr, "!%c!\n", CUR_SYMB);

    // fprintf(stderr, "Blyat in -> %s\n", curCmd);
    // fprintf(stderr, "   Blyat in -> %s\n", *buffer);
    err |= treeBaseScanf(&((*node) -> lft), buffer);
    // fprintf(stderr, "Blyat -> %s\n", curCmd);
    // fprintf(stderr, "   Blyat -> %s\n", *buffer);
    err |= treeBaseScanf(&((*node) -> rgt), buffer);
    // fprintf(stderr, "Blyat -> %s\n", curCmd);

    CUR_STR = flashBuf(CUR_STR);
    if (CUR_SYMB != '}') return EXIT_FAILURE;
    NEXT_SYMB;
    
    // fprintf(stderr, "Blyat out -> %s\n", curCmd);

    return err;
}

int treeMakeBase(Tree *head, FILE *stream) {
    catchNullptr(head);
    catchNullptr(stream);

    char nullString[MAX_STRING_SIZE] = "";
    int err = treeBasePrint(head -> tree, nullString, stream);
    if (err) return err;

    return TreeIsOk;
}

int treeBasePrint(TreeNode *node, char *nullString, FILE *stream) {
    catchNullptr( node );
    catchNullptr(stream);

    int      err    = EXIT_SUCCESS;
    char *curString =  nullString ;

    fprintf(stream, "%s{ \"%s\" \n", nullString, node -> data);
    strcat(nullString, "    ");

    if (node -> lft != nullptr)
        err |= treeBasePrint(node -> lft, nullString, stream);
    else 
        fprintf(stream, "%s{    }\n", nullString);
    
    if (node -> rgt != nullptr)
        err |= treeBasePrint(node -> rgt, nullString, stream);
    else 
        fprintf(stream, "%s{    }\n", nullString);

    nullString[strlen(nullString) - 4] = '\0';
    fprintf(stream, "%s}\n", nullString);

    return err;
}

const char *treeGraphVizDump(Tree *tree, const char *fileName, int cmd) {
    if (fileName == nullptr) return nullptr;
    if (  tree   == nullptr) return nullptr;

    FILE *stream = fopen(fileName, "w");
    if (stream == nullptr) return nullptr;

    fprintf(stream, "digraph Tree {\n"
                    "   rankdir = TB;\n");

    fprintf(stream, "   subgraph varriables {\n"
                    "       node[shape = component, style = filled, fillcolor = orange];\n"

                    "         Tree   [label = \"Head = %08X\"];\n"
                    "        Status  [label = \"Status = %d\"];\n"
                    "   }\n",
            tree -> tree, tree -> status);

    fprintf(stream, "   subgraph Nodes {\n"
                    "       node[shape = Mrecord, style = filled, fillcolor = lightgreen];\n");
    size_t cnt = 1;
    treePrintNode(tree -> tree, &cnt, stream);
    fprintf(stream, "   }\n");
    fprintf(stream, "   Tree->node1;\n}\n");

    fclose(stream);

    static char picName[MAX_CMD_SIZE] = "";
    sprintf(picName, "logs/logPic%zu.png", cmd);

    char CmdBuffer[MAX_CMD_SIZE] = {0};
    sprintf(CmdBuffer, "dot -Tpng %s -o %s", fileName, picName);

    if (system(CmdBuffer)) return nullptr;

    FILE *LogGraph = fopen("logs/HtmlLog.html", "w");
    
    fprintf(LogGraph, "<center>\n<h1>\nPicture[%zu]\n</h1>\n</center>\n", cmd);
    fprintf(LogGraph, "<img src= logPic%zu.png />\n", cmd);

    fclose(LogGraph);

    return picName;
}

void treePrintNode(TreeNode *tree, size_t *cur, FILE *stream) {
    if (tree == nullptr || cur == nullptr || stream == nullptr) return;

    int nodeNum = *cur;
    fprintf(stream, "       node%zu[label = \"{ Node: %zu | LeftSon: %08X | RigthSon: %08X | Data: %s }\"];\n",
                            nodeNum,        nodeNum,    tree -> lft,    tree -> rgt,     tree -> data
            );
    
    if (tree -> lft != nullptr) {
        (*cur)++;
        fprintf(stream, "       node%zu -> node%zu[xlabel = \"No\", color = \"darkblue\"];\n", nodeNum, *cur);
        treePrintNode(tree -> lft, cur, stream);
    }
    if (tree -> rgt != nullptr) {
        (*cur)++;
        fprintf(stream, "       node%zu -> node%zu[xlabel = \"Yes\", color = \"darkgreen\"];\n", nodeNum, *cur);
        treePrintNode(tree -> rgt, cur, stream);
    }
}

static void myfPrintf(FILE *stream = nullptr, const char *format = nullptr, ...) {
    if (format == nullptr)
        return;

    va_list arguments;
    va_start (arguments, format);
    vfprintf(stderr, format, arguments);
    if (stream != nullptr)
        vfprintf(stream, format, arguments);
    va_end(arguments);
}


void treePrintErrorMessage(int error) {
    if (error == 0) {
        myfPrintf(LogFile, "No Errors occured :)\n");
        return;
    }

    size_t numberOfErrors = 0;
    for (size_t currentError = 0; currentError < 20; currentError++)
        if (error & (1 << currentError))
            numberOfErrors++;
    myfPrintf(LogFile, "Program stopped with %d errors: \n", numberOfErrors);

    size_t currentError = 1;
    if (error & (1 <<  0))
        myfPrintf(LogFile, "%zu)  Struct Tree was nullptr!\n", currentError++);
    if (error & (1 <<  1))
        myfPrintf(LogFile, "%zu)  Tree is already full!\n", currentError++);
    if (error & (1 <<  2))
        myfPrintf(LogFile, "%zu)  Tree is already empty!\n", currentError++);
    if (error & (1 <<  3))
        myfPrintf(LogFile, "%zu)  tree -> head cannot be erased!\n", currentError++);
    if (error & (1 <<  4))
        myfPrintf(LogFile, "%zu)  Data in struct Tree was Null!\n", currentError++);
    if (error & (1 <<  5))
        myfPrintf(LogFile, "%zu)  Next in struct Tree was Null!\n", currentError++);
    if (error & (1 <<  6))
        myfPrintf(LogFile, "%zu)  There is no element with this index!\n", currentError++);
    if (error & (1 <<  7))
        myfPrintf(LogFile, "%zu)  Prev in struct Tree was Null!\n", currentError++);
    if (error & (1 <<  8))
        myfPrintf(LogFile, "%zu)  Struct Tree was inactive!\n", currentError++);
    if (error & (1 <<  9))
        myfPrintf(LogFile, "%zu)  Data in struct Tree was POISONED!\n", currentError++);
    if (error & (1 << 10))
        myfPrintf(LogFile, "%zu)  The System of free blocks is damaged in struct Tree\n", currentError++);
    if (error & (1 << 11))
        myfPrintf(LogFile, "%zu)  Struct Tree was Destructed two times!\n", currentError++);
    if (error & (1 << 12))
        myfPrintf(LogFile, "%zu)  Struct Tree was Constructed two times!\n", currentError++);
    if (error & (1 << 13))
        myfPrintf(LogFile, "%zu)  GraphViz execution error!\n", currentError++);
}

void treeLogClose() {
    if (LogFile != nullptr)
        fclose(LogFile);
}

// void vizLogClose() {
//     fclose(LogGraph);
// }

void flash() {
    int c = getchar();
    while (c != '\n' && c != EOF) c = getchar();
}



void tell(const char * format, ...) {
    if (format == nullptr)
        return;

    va_list arguments = {0};
    va_start (arguments, format);

    char buffer[MAX_STRING_SIZE] = {0};
    vsprintf(buffer, format, arguments);

    va_end(arguments);
    
    char voiceCmd[MAX_STRING_SIZE] = {0};
        sprintf (voiceCmd, "%s%s%s", "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis'"
                                                                " xml:lang='EN'>", buffer, "</speak>");
    txSpeak (voiceCmd);                                                 
    printf("%s", buffer);                                                           
}

int treeNew(TreeNode **node, char *curCmd) {
    catchNullptr( node );
    catchNullptr(curCmd);

    *node = (TreeNode *) calloc(1, sizeof(TreeNode));
    catchNullptr(*node);

    int err = treeNodeCtor(*node, curCmd);

    return err;
}

static char *flashBuf(char *buffer) {
    while (*buffer == '\n' || *buffer == ' ') ++buffer;

    return buffer;
}

static char *skipWord (char *buffer) {
    while (*buffer != '}' && *buffer != '{') buffer++;

    return buffer;
}