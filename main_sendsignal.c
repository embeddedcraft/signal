#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
 #include <sys/types.h>

void errExit (char *msg){
perror(msg);
exit(1);
}
// run program
// ./signal_send -s <signal_number> -p <process_id>
// ./signal_send -s 10 -p 123456

int main (int argc, char *argv[])
{
	int opt=0,signum=0;
	pid_t pid;
	// parsing command line argument
	 while ((opt = getopt(argc, argv, "s:p:")) != -1) {
		 switch(opt){
		 case 's':
			 signum= atoi(optarg);
			 break;
		 case 'p':
			 pid= atoi(optarg);
			 break;
		 default :
			 signum = SIGINT;
			 break;
		 } // end of switch
	 } // end of while


	// sending signal

	 printf("sending signal# %d to Process ID %d !!!",signum, pid);

     if (kill(pid, signum) == -1)
         errExit("kill");
	printf("\nsend signal is terminated !!!");
} // end of main

