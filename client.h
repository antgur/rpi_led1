#include <pigpio.h>
#include <stdio.h>
#include <string.h>
void sighandler(int sig);
int siusti(int sockClient, int code);
int childWork(int sockClient);