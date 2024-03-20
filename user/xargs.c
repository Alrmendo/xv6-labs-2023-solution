#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char *args[MAXARG];
    char line[512];
    int i;

    if(argc < 2){
        fprintf(2, "Usage: xargs command ...\n");
        exit(1);
    }

    // Check for -n option and shift arguments if found
    if(argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0') {
        for(i = 0; i < argc-3; i++){
            args[i] = argv[i+3];
        }
    } else {
        for(i = 0; i < argc-1; i++){
            args[i] = argv[i+1];
        }
    }

    args[i] = line;
    args[i+1] = 0;

    while(1){
        gets(line, sizeof(line));
        if(line[0] == '\0') break;  // break the loop if gets returns an empty string

        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = 0;  // replace newline with null
        }

        if(fork() == 0){
            exec(args[0], args);
            fprintf(2, "\n", args[0]);
            exit(1);
        }
        wait(0);
    }

    exit(0);
}
