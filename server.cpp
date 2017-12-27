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
extern void error(const char *msg); // extern: it is going to be defined else where
int server(char *argv[])
{
    // ============================================== Initializing ==============================================
    struct sockaddr_in server;
    int sock, length = sizeof(server), n;
    socklen_t fromlen = sizeof(struct sockaddr_in);;
    struct sockaddr_in from;
    char buf[1024];
    int i = 0;
    int error_rate = atoi(argv[2]) * 10;

    sock = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket for ipv4
    if (sock < 0) error("Opening socket"); // failed initializing socket
    bzero(&server,length); // fill the struct with zeroz
    server.sin_family=AF_INET; // ipv4 address
    server.sin_addr.s_addr=INADDR_ANY; // get packets from all ip's
    server.sin_port=htons(atoi(argv[1])); // port
    if (bind(sock,(struct sockaddr *)&server,length)<0) // bind to the indicated ip & port
        error("binding");

    while (1) {
        if (i % 10 == error_rate) // artificial packet loss
        {
            i++;
            continue;
        }
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen); //receive from client
        if (n < 0) continue; // wait for another packet i.e stop & wait
        this_thread::sleep_for(chrono::milliseconds(10)); // process the packet for 10 0.1 second

        // ack message
        char * t = "Got your message: ";
        char * temp1 = const_cast<char*>(to_string(i).c_str());


        n = sendto(sock, t, sizeof(t), // send ack
                   0,(struct sockaddr *)&from,fromlen);
        if (n  < 0) // failed to send ack
            continue;
        write(1,"Received a datagram: ",21);
        write(1,buf,n);
        i++; // wait for next message
    }
    return 0;
}

