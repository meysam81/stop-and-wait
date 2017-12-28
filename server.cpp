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
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//int server (int argc, char *argv[])
int main(int argc, char *argv[])
{
    struct sockaddr_in server,
            from;
    int sock,
            length = sizeof(server),
            n;
    socklen_t fromlen = sizeof(struct sockaddr_in);
    char buf[1024];

    if (argc != 3)
        error("Usage: <port> <error_rate>");

    sock=socket(AF_INET, SOCK_DGRAM, 0); // AF_INET = ipv4 address type; SOCK_DGRAM = UDP socket

    if (sock < 0) error("Opening socket"); // if socket is not created

    bzero(&server, length); // fill with zeros

    // a socket address object with ipv4, listening to every ip address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&server, length) < 0) // if binding does not work
        error("binding");

    // in UDP protocol, there's no need for "Listen", so we just receive packets and that's all
    while (1)
    {
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);

        if (n < 0) error("recvfrom"); // if nothing received

        this_thread::sleep_for(chrono::milliseconds(10)); // processing for 0.01 seconds

        // STDOUT for monitoring purpose
        write(1,"Received a datagram: ",21);
        write(1,buf,n);
        while(1)
        {
            n = sendto(sock,"Got your message\n",17, // ack
                       0,(struct sockaddr *)&from,fromlen);

            if (n  < 0) continue; // "stop and wait" until packet reaches its destination
            else break;
        }
    }
    return 0;
}
