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

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int _main(char *argv[]);
//int client(int argc, char *argv[])
int main(int argc, char *argv[])
{
    if (argc != 4)
        error("missing arguments: <ip_address> <port> <error_rate>");

    int N = 10; // number of threads
    thread t[N];
    for (int i = 0; i < N; i++)
        t[i] = thread(&_main, argv);
    for (int i = 0; i < N; i++)
        t[i].join();
    return 0;
}

int _main(char *argv[])
{
    int sock, n;
    unsigned int length = sizeof(struct sockaddr_in);;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];

    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) error("Unknown host");

    bcopy((char *)hp->h_addr,
          (char *)&server.sin_addr,
          hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    bzero(buffer, 1000);

    clock_t startTime = clock(); //Start timer
    double secondsPassed;
    double secondsToDelay = atof("600"); // continue for 10 minutes
    bool flag = true;
    while (flag)
    {
        while (1)
        {
            n = sendto(sock,buffer,
                       strlen(buffer),0,(const struct sockaddr *)&server,length);
            if (n < 0) continue;
            else break;
        }
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
        if (n < 0) error("recvfrom");

        // STDOUT
        write(1,"Got an ack: ",12);
        write(1,buffer,n);

        secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
        if(secondsPassed >= secondsToDelay)
            flag = false;
    }
    close(sock);
    return 0;
}
