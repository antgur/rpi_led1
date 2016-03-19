#include "client.h"
void sighandler(int sig) { }

int siusti(int sockClient,int code) 
{
    char bu[256];
    int n, snt;
    FILE *out = fopen("b.log","a");
    if (code == -1)
    {
	sprintf(bu,"Isvadai uzimti\r\n");
	n = strlen(bu);
	if ((snt = send(sockClient, bu, n, 0)) == -1)
	{
	    fprintf(out,"Siuntimas neivyko -> %d\n",sockClient);
	    perror("Siuntimas neivyko ");
	    return -2;
	}
    } else if (code == 0)
    {
	sprintf(bu,"OK\r\n");
	n = strlen(bu);
	if ((snt = send(sockClient, bu, n, 0)) == -1)
	{
	    fprintf(out,"Siuntimas neivyko -> %d\n",sockClient);
	    perror("Siuntimas neivyko ");
	    return -2;
	}
    } else
    {
    }
    fclose(out);
    return 0;
}

int childWork(int sockClient)
{
    double start;
    FILE *out = fopen("b.log","w+");
    fclose(out);
    if (gpioInitialise() < 0)
    {
	siusti(sockClient, -1);
	return -1;
    }
    gpioSetMode(19, PI_OUTPUT);
    start = time_time();
    while ((time_time() - start) < 10.0)
    {
      gpioWrite(19, 1); /* on */
      time_sleep(0.1);
      gpioWrite(19, 0); /* off */
      time_sleep(0.1);
    }
    gpioTerminate();
    siusti(sockClient,0);
    return 0;
}