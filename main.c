#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int num_children = 10;

void handler(int sig)
{
    int pid, status;

    /* signal handler added to fork exercise */
    printf("Parent handler: Got signal %d\n", sig);

    if (sig == SIGCHLD)
    {
        while((pid=waitpid(-1, &status, WNOHANG)) > 0)
        {
            printf("Parent handler: Reaped child %d\n", pid);
            --num_children;
            printf("Parent handler: Children left %d\n", num_children);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t child[10];
    int i, pid, status;

    signal(SIGCHLD, handler);

    printf("main: pid %d, ppid %d\n", getpid(), getppid());
    for(i=0; i<10; i++)
    {
    child[i] = fork();
    if (child[i] == 0)
        {
        pid = getpid();
        printf("child: created with pid %d, ppid %d\n", pid, getppid());
        if (pid % 2 ==0 )
            execvp("fortune", argv);
        else{
            printf("\n ===write fortune to file here as pid is %d===\n",pid);
            freopen("./lauseet.txt","a",stdout);
            execvp("fortune", argv);
        }
        exit(EXIT_SUCCESS);
        }
    else if (child[i] == -1)
         { perror("problem creating a child");return -1;}
    else
        {
          printf("main: created child %d\n", child[i]);
        }
    }

    while(i)
    {
    waitpid(child[i], &status, 0);
    printf("Waiting for %d - %d\n", i, child[i]);
    --i;
    }
    return EXIT_SUCCESS;
}
