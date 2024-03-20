#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p2c[2], c2p[2];
    int child_id;
    char* ping = "ping";
    char* pong = "pong";
    char buf[512] = {0};

    // Create pipes
    if (pipe(p2c) == -1 || pipe(c2p) == -1) 
    {
        fprintf(2, "Error: pipe creation error.\n");
        exit(1);
    }

    child_id = fork();
    if (child_id < 0) 
    {
        fprintf(2, "Error: fork failed.\n");
        exit(1);
    }

    if(child_id != 0)
    {
        // parent process
        close(p2c[0]); // close read only write
        close(c2p[1]); // close write only read

        // write ping to the child process (p2c)
        write(p2c[1], ping, strlen(ping));

        // after write then read from the child process (c2p)
        read(c2p[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    else
    {
        // child process
        close(p2c[1]); // close write only read
        close(c2p[0]); // close read only write

        // read from the parent process
        read(p2c[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);

        // write to the parent process
        write(c2p[1], pong, strlen(pong));
        exit(0);
    }
}
