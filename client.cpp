/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <thread>
using namespace std;
void error(const char *);
int _main(int argc, char *argv[]);
int client(int argc, char *argv[])
{
    int N = 10; // number of threads
    thread t[N];
    for (int i = 0; i < N; i++)
        t[i] = thread(_main, argc, argv);
    for (int i = 0; i < N; i++)
        t[i].join();
    return 0;
}

int _main(int argc, char *argv[])
{
    int sock, n;
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];

    if (argc != 4) { printf("Usage: server port error_rate\n");
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");

    bcopy((char *)hp->h_addr,
          (char *)&server.sin_addr,
          hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);
    bzero(buffer,1000);



//    string err = argv[3];
//    int error_rate = int(err);
//    error_rate *= 10;
    clock_t startTime = clock(); //Start timer
    double secondsPassed;
    double secondsToDelay = atof("600");
    bool flag = true;
    int i = 0;
    while(flag)
    {
//        if (i % 10 == error_rate)
//        {
//            i++;
//            continue;
//        }
//        char* t = buffer + " " + i;
        char *t = buffer;
        n=sendto(sock,t,
                 strlen(t),0,(const struct sockaddr *)&server,length);
        if (n < 0)
            continue;
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
        if (n < 0)
            continue;
        i++;
        write(1,"Got an ack: ",12);
        write(1,buffer,n);


        secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
        if(secondsPassed >= secondsToDelay)
        {
            flag = false;
        }
    }





    close(sock);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

