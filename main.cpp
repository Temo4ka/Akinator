#include "tree.h"

int main (int argc, const char* argv[]) {
    FILE *outStream = (argc > 1? fopen (argv[1], "w") : stdout);

    Tree root = {};
    treeCtor(&root);

     int   err = 0;
    size_t pic = 0;

    if (argc > 2) {
        err = treeLoadBase(&root, argv[2]);
        if (err) return EXIT_FAILURE;
        err = treeGraphVizDump(&root, "./logs/GraphDump.txt", ++pic);
            if (err) return EXIT_FAILURE;
    }
    
    tell("Hi, there!!!\n", NULL);
    tell("Do you,", NULL);
    tell(" an ordinary pathetic mortal,", NULL);
    tell(" wanna check my almigthness?\n", NULL);
    tell("Check out,", NULL);
    tell(" what I can do:\n", NULL);

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

    while (true) {
        char cmd1[MAX_STRING_SIZE] = "";
        char cmd2[MAX_STRING_SIZE] = "";
        scanf("%s", cmd1);

        if (!strcmpi("exit", cmd1))
            break;
        else if (!strcmpi("game", cmd1)) {
            err = treeExecuteNode(root.tree);
            if (err) return EXIT_FAILURE;
            tell("Suck s Ass\n", NULL);
        } else if (!strcmpi("Print", cmd1)) {
            err = treeGraphVizDump(&root, "./logs/GraphDump.dot", ++pic);
            if (err) return EXIT_FAILURE;
            tell("Suck s Ass\n", NULL);
        } else if (!strcmpi("Def", cmd1)) {
            tell("Object name: ", NULL);
            flash();
            cmd1 = fgets(cmd1, MAX_STRING_SIZE, stdin);
            int res = treePrintDefRequest(&root, cmd1, &err);
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
        } else if (!strcmpi("Comp", cmd1)) {
            tell("First object name: ", NULL);
            flash();
            cmd1 = fgets(cmd1, MAX_STRING_SIZE, stdin);
            tell("Second object name: ", NULL);
            flash();
            cmd2 = fgets(cmd2, MAX_STRING_SIZE, stdin);

            if (!strcmpi(cmd1, cmd2)) {
                tell("The names should be different!\n", NULL);
                continue;
            }
            int res = treePrintCompareRequest(&root, cmd1, cmd2, &err);
            if (err) return EXIT_FAILURE;
            if (res == NotFound) {
                tell("Not both objects were found!", NULL);
                tell(" Try again.\n", NULL);
            }
            else {
                tell("Suck s Ass\n", NULL);
            }
        } else {
            tell("Unknown command!", NULL);
            tell(" Try again\n", NULL);
        }
        
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
    tell("Bye,", NULL);
    tell("see u later UwU!\n", NULL);

    err = treeMakeBase(&root, outStream);

    if (argc > 1) fclose(outStream);

    if (err) return EXIT_FAILURE;

    if (treeDtor(&root)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
