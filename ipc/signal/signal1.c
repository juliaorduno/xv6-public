#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

//Executed when signal received
void sigint_handler(int sig)
{
    printf("Ahhh! SIGINT!\n");
}

int main(void)
{
    char s[200];
    struct sigaction sa;

    sa.sa_handler = sigint_handler; //Name of function to be executed
    sa.sa_flags = 0; // or SA_RESTART Set of flags
    sigemptyset(&sa.sa_mask); //Can't be interrupted

    //Number of interruption
    //When SIGINT is received, execute sa. Third parameter, to restore previous value
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Enter a string:\n");

    if (fgets(s, sizeof s, stdin) == NULL)
        perror("fgets");
    else 
        printf("You entered: %s\n", s);

    return 0;
}