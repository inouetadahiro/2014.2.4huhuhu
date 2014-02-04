#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "x_dbg.h"
#include "tsk_id.h"

#define MAX_CHILD  (3)

USHORT         gCurrentTaskID = TSKID_WATCH;

char           *children[] = { "./mgr","./task1", "./task2"};

int
main ()
{
    int             status;
    pid_t           pid;
    int             i, err;
    int             num_child = 0;


    for (i = 0; i < MAX_CHILD; i++) {

        pid = fork ();
        printf("%s pid=%d\n", children[i], pid);

        switch (pid) {

        case -1:
            X_PRINTF(DBG_CRIT,"fork failed for %s errno=%d\n", 
                       children[i],errno);
            break;

        case 0:
            //child
            err = execv (children[i], NULL);
            if (err == -1) {
                X_PRINTF (DBG_CRIT,"exec failed for %s\n", children[i]);
            }
            exit(1); //do not continue on failure

        default:
            //parent
            num_child++;
            break;

        } //switch

    } //for


    while (num_child) {

        pid = wait (&status);

        switch (pid) {

        case -1:
            X_PRINTF (DBG_CRIT,"wait failed, errno=%d\n",errno);
            break;

        default:
            num_child--;
            if (WIFEXITED (status)) {
                X_PRINTF (DBG_INFO,"PID=%d terminated normally with exit status=%d\n",
                        pid, WEXITSTATUS (status));
            }

            if (WIFSIGNALED (status)) {
                X_PRINTF (DBG_WARN,"PID=%d killed by signal=%d%s\n", pid,
                        WTERMSIG (status),
                        WCOREDUMP (status) ? " (dumped core)" : "");
            }

            if (WIFSTOPPED (status)) {
                X_PRINTF (DBG_WARN,"PID=%d Stopped by signal=%d\n", pid, 
                        WSTOPSIG (status));
            }

            break;

        } //switch

    } //while

    return 0;
}
