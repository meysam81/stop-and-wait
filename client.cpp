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
int _main(char *argv[]);
int client(char *argv[])
{
    int N = 10; // number of client threads
    thread t[N];
    for (int i = 0; i < N; i++)
        t[i] = thread(&_main, argv); // give the thread: 1) the function name, 2) the arguments
    for (int i = 0; i < N; i++)
        t[i].join(); // start threads
    return 0;
}

int _main(char *argv[]) // the real deal
{
    // ============================================== Initializing ==============================================
    int sock, n;
    unsigned int length = sizeof(struct sockaddr_in); // gonna use it later
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    int error_rate = atoi(argv[2]) * 10; // we get the <error_rate> in form of float e.g. 0.1; so we need the int
    double secondsPassed;
    double secondsToDelay = atof("600"); // 600 seconds = 10 minutes
    bool flag = true;
    int i = 0;


    sock= socket(AF_INET, SOCK_DGRAM, 0); // create a udp socket
    if (sock < 0) error("socket"); // if not created for any reason what so ever

    server.sin_family = AF_INET; // ipv4 kind of socket
    hp = gethostbyname("127.0.0.1"); // ip address
    if (hp==0) error("Unknown host"); // couldn't resolve

    bcopy((char *)hp->h_addr, // <destinaton>
          (char *)&server.sin_addr, // <source>
          hp->h_length); // copies h_lenght bytes from <source> to <destination>

    server.sin_port = htons(atoi(argv[1])); // port number

    bzero(buffer,1000); // a message from client to server filled with zeroz; you can edit this message yourself

    clock_t startTime = clock(); //Start timer; for benchmarking
    while(flag)
    {
        if (i % 10 == error_rate) // artificial errors occuring to packets
        {
            i++;
            continue;
        }

        char* tempMsg = "Packet ";
        char* temp1 = const_cast<char*>(to_string(i).c_str());
        strcat(tempMsg, temp1);
        temp1 = " : ";
        strcat(tempMsg, temp1);
        strcat(tempMsg, buffer);

        n = sendto(sock,tempMsg,
                 strlen(tempMsg),0,(const struct sockaddr *)&server,length);
        if (n < 0) // that's the part where stop and wait comes to the play to send the packet again until it is sent
            continue;
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
        if (n < 0) // don't proceed if no ack is received
            continue;
        i++; // go to the next packet
        write(1,"Got an ack: ",12);
        write(1,buffer,n);


        secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
        if(secondsPassed >= secondsToDelay) // the timer to count for seconds
        {
            flag = false;
        }
    }

    close(sock); // close the socket before exiting
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
