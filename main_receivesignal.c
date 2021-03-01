/*
 * main.c
 *
 *  Created on: 12-Sep-2020
 *      Author: embeddedcraft
 */
// run this program as ...
// signal_receiver &

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#define MYSIGNAL SIGUSR1     // 10
#define MYEXITSIGNAL SIGINT  //2
void errExit (char *msg){
perror(msg);
exit(1);
}

#define LOOP 5
int led_toggle =0, int_exit=0, int_sig_received=0;
static void handler(int sig, siginfo_t *si, void *uc)
{
if (sig == MYSIGNAL)
{
	int_sig_received=1;
	switch (led_toggle){
	case 0: led_toggle =1;
		break;
	case 1: led_toggle =0;
		break;
	default:
		led_toggle =0;
	}
}
 else if  (sig == MYEXITSIGNAL){
	 int_sig_received=1;
	int_exit=1;
 }
else
	printf("\n Unknown Signal !!! \n");
} // endof handler function



int main (void)
{
	struct sigaction sa_my,sa_int; 	int f=0;
	printf("\n Signal Receiver 3\n");
	printf("\n My Process ID = %d", getpid());

	// registering MYSIGNAL
	sa_my.sa_flags = SA_SIGINFO;   // signal handler will be called with three arguments
	sa_my.sa_sigaction = handler;
	sigemptyset(&sa_my.sa_mask);  // clearing all signal
	// registering signal handler for MYSIGNAL
	if (sigaction(MYSIGNAL, &sa_my, NULL) == -1)
		errExit("sigaction");

	// registering SIGINT
	sa_int.sa_flags = SA_SIGINFO;
	sa_int.sa_sigaction = handler;
	sigemptyset(&sa_int.sa_mask);
	// registering signal handler
	if (sigaction(MYEXITSIGNAL, &sa_int, NULL) == -1)
		errExit("sigaction");


	// set direction out for GPIO 60
	printf("\nSignal Receiver: set direction !!!");
	f=open("/sys/class/gpio/gpio60/direction",O_RDWR);
	if(f == -1){ 		errExit("\n Open_gpio60_direction !!!"); 	}
	write(f,"out",3);
	close(f);

	f=open("/sys/class/gpio/gpio60/value",O_WRONLY);
	if(f == -1){ 		errExit("\n Open_gpio60_value !!! "); 	}

	while(int_exit ==0 )  // we will be in loop till int_exit is zero
	{
		pause(); // block until a signal is caught
		if(int_sig_received==1)
		{
			if (led_toggle == 0)
			write(f,"0",1);  // LED off
			else
			write(f,"1",1); // LED on;

			printf("\nSignal Receiver: LED write done!!!");
			int_sig_received=0;
		}

	}  // end of while
	close(f);

	printf("\nSignal Receiver: terminated !!!");
} // end of main



