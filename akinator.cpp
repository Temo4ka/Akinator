#include "akinator.h"
#include "config.h"

void akinatorStart() {
    tell("Hi, there!!!\n", NULL);
    tell("Do you,", NULL);
    tell(" an ordinary pathetic mortal,", NULL);
    tell(" wanna check my almigthness?\n", NULL);
    tell("Check out,", NULL);
    tell(" what I can do:\n", NULL);
}

void tellModes() {
    tell("Mode[1]: \"Game\", ", NULL);
        tell("You think up an object", NULL);
        tell(" - I guess it;\n", NULL);
    tell("Mode[2]: \"Print\", ", NULL);
        tell("I'll show my base in GraphViz;\n", NULL);
    tell("Mode[3]: \"Def\", ", NULL);
        tell("You give me an object", NULL);
        tell(" - I describe it;\n", NULL);
    tell("Mode[4]: \"Comp\", ", NULL);
        tell("You give me two objects", NULL);
        tell(" - I describe their difference;\n", NULL);
    tell("Mode[5]: \"Exit\", ", NULL);
        tell("We finally end our conversation;\n", NULL);
}

void printModes() {
    printf("Mode[1]: \"Game\", ", NULL);
        printf("You think up an object", NULL);
        printf(" - I guess it;\n", NULL);
    printf("Mode[2]: \"Print\", ", NULL);
        printf("I'll show my base in GraphViz;\n", NULL);
    printf("Mode[3]: \"Def\", ", NULL);
        printf("You give me an object", NULL);
        printf(" - I describe it;\n", NULL);
    printf("Mode[4]: \"Comp\", ", NULL);
        printf("You give me two objects", NULL);
        printf(" - I describe their difference;\n", NULL);
    printf("Mode[5]: \"Exit\", ", NULL);
        printf("We finally end our conversation;\n", NULL);
}

void akinatorExit() {
    tell("Bye,", NULL);
    tell("see u later UwU!\n", NULL);
}

int akinatorGame(TreeNode *root) {
    catchNullptr(root);

    int err = treeExecuteNode(root);
    if (err) return err;

    tell("Suck s Ass\n", NULL);
    return EXIT_SUCCESS;
}

int akinatorPrint(Tree *root, size_t *pic) {
    const char* picName = treeGraphVizDump(root, "./logs/GraphDump.dot", ++(*pic));
    char cmd[MAX_CMD_SIZE] = "";

    sprintf(cmd, "start %s", picName);
    system(cmd);
    if (picName == nullptr) return EXIT_FAILURE;

    tell("Suck s Ass\n", NULL);
    return EXIT_SUCCESS;
}

static int askForString(char *toPrint, char *toGet);

int akinatorDef(Tree *root) {
    catchNullptr(root);
    
    flash();  

    char *cmd1 = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
    int err = askForString("Object name: ", cmd1);

    int res = treePrintDefRequest(root, cmd1, &err);
    if (err) {
        fprintf(stderr, "%d", err);
        return EXIT_FAILURE;
    }
    if (res == NotFound) {
        tell("The object was not found!\n", NULL);
        tell(" Try again.\n", NULL);
    } else {
        tell("Suck s Ass\n", NULL);
    }
    free(cmd1);

    return EXIT_SUCCESS;
}

int akinatorComp(Tree *root) {
    catchNullptr(root);
    
    flash();

    char *cmd1 = (char *) calloc(MAX_STRING_SIZE, sizeof(char));
    char *cmd2 = (char *) calloc(MAX_STRING_SIZE, sizeof(char));

    int err = askForString("Object name: ", cmd1);
    if (err) return EXIT_FAILURE;
        
    err = askForString("Second object name: ", cmd2);
    if (err) return EXIT_FAILURE;

    int res = treePrintCompareRequest(root, cmd1, cmd2, &err);
    if (err) return EXIT_FAILURE;
    
    if (res == NotFound) {
        tell(" Try again.\n", NULL);
    } else {
        tell("Suck s Ass\n", NULL);
    }

    free(cmd1);
    free(cmd2);

    return EXIT_SUCCESS;
}

static int askForString(char *toPrint, char *toGet) {
    catchNullptr(toPrint);
    catchNullptr( toGet );

    tell(toPrint, NULL);
    toGet = fgets(toGet, MAX_STRING_SIZE, stdin);
    char *lastSymb = (strchr(toGet, '\n'));
    if (lastSymb != nullptr)
         *lastSymb =  '\0';

    return EXIT_SUCCESS;
}