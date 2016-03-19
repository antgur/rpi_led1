#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include "client.h"

#define MYPORT 4441
// startuoju programa, kuri atrisama nuo terminalo

int pagr();

int main()
{
    switch(fork())
    {
    case -1:
        printf("Fork error\n");
        return -1;
    case 0:
        if(setsid() == -1)
            break;
        pagr();
        exit(0);
        break;
    default:
        break;
    }
    return 0;
}

// pagrindine programa - jau atrista nuo terminalo
int pagr()
{
int sockMain, sockClient, length, child;
struct sockaddr_in servAddr;
    signal(SIGCHLD,SIG_IGN);

  // 1. Sukuriu pagrindini serverio bloka
  if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { perror("Serveris negali atidaryti pagrindinio soketo.");
    exit(1);
    }
  // 2. Sudaroma duomenu struktura lokaliu IP adresu saugojimui...
  bzero( &(servAddr.sin_zero), 8);
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons (MYPORT);
  // 3. Socketui suteikiu adresa, parenku porto numeri, irasau ji i TCB
  if ( bind(sockMain, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)) \
                                                                ==-1 )
    { printf("Serveri priristi nepavyko:%d\n", ntohs(servAddr.sin_port));
    perror("Uzimtas portas>");
    exit(1);
    }

  printf("SERVERIS: porto numeris - %d\n", ntohs(servAddr.sin_port));
  // 4. Sukuriu eile 5 klientu saugojimui
    if ( listen (sockMain, 5) ==-1) {
    perror("listen");
    exit(1);
    }
  // 5. Laukiu kliento. Sulaukus - grazinu nauja socket deskriptoriu,
  //    kuri naudos klientas.
  for ( ; ; ) {
  if ( (sockClient = accept(sockMain, 0, 0)) < 0)
    { perror("Neteisingas kliento socket.");
    exit(1);
    }
  // 6. Sukuriu dukterini procesa kliento aptarnavimui
  if ( (child = fork()) < 0)
    { 
    perror("Dukterinio proceso sukurimo klaida.");
    exit(1);
    }
  else if (child == 0)  // tai - dukterinio proceso vykdymo kodas
    { 
    close(sockMain);  // dukterinis procesas sockMain daugiau neidomus
    childWork(sockClient);
    close(sockClient);
    exit(0);
    }
  // 7. Tai - pagrindinis procesas, kurio daugiau nedomina dukterinis procesas
  close(sockClient);
  }
}
