/* Creates a datagram server.  The port
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <thread>
#include <chrono>
using namespace std;
extern void error(const char *msg);
int server(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];

    if (argc != 3) {
        fprintf(stderr, "Usage: port error_rate");
        exit(0);
    }
//    int error_rate = argv[2] * 10;
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&server,length)<0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    int i = 0;
    while (1) {
//        if (i % 10 == error_rate)
//        {
//            i++;
//            continue;
//        }
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0) error("recvfrom");
        this_thread::sleep_for(chrono::milliseconds(10));
//        string t = "Got your message " + i;
        char * t = "Got your message";
        n = sendto(sock, t, sizeof(t),
                   0,(struct sockaddr *)&from,fromlen);
        if (n  < 0)
            continue;
        write(1,"Received a datagram: ",21);
        write(1,buf,n);
        i++;
    }
    return 0;
}

