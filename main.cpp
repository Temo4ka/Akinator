#include "akinator.h"
#include "config.h"

int main (int argc, const char* argv[]) {
    FILE *outStream = (argc > 1? fopen (argv[1], "w") : stdout);

    Tree root = {};
    treeCtor(&root);

     int   err = 0;
    size_t pic = 0;

    if (argc > 2) {
        err = treeLoadBase(&root, argv[2]);
        if (err) return EXIT_FAILURE;
        
        const char *picture = treeGraphVizDump(&root, "./logs/GraphDump.txt", ++pic);
        if (picture == nullptr) return EXIT_FAILURE;
    }
    
    akinatorStart();

    printModes();

    char *userCmd = (char *) calloc(MAX_CMD_SIZE, sizeof(char));

    while (true) {
        scanf("%s", userCmd);

        if (!strcmpi("exit", userCmd))
            break;
        else if (!strcmpi("game", userCmd)) {
            err = akinatorGame(root.tree);
            if (err) return EXIT_FAILURE;

        } else if (!strcmpi("Print", userCmd)) {
            err = akinatorPrint(&root, &pic);
            if (err) return EXIT_FAILURE;
            
        } else if (!strcmpi("Def", userCmd)) {
            err = akinatorDef(&root);
            if (err) return EXIT_FAILURE;

        } else if (!strcmpi("Comp", userCmd)) {
            err = akinatorComp(&root);
            if (err) return EXIT_FAILURE;
        } else {
            tell("Unknown command!", NULL);
            tell(" Try again\n", NULL);
        }
        
        printModes();
    }
    
    akinatorExit();

    free(userCmd);

    err = treeMakeBase(&root, outStream);

    if (argc > 1) fclose(outStream);

    if (err) return EXIT_FAILURE;

    if (treeDtor(&root)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
