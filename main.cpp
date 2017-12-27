#include <iostream>
using namespace std;
#include <thread>

// these 2 babies are going to be defined else where
extern int client(char *argv[]);
extern int server(char *argv[]);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Usage: <port> <error_rate>");
        exit(1);
    }
//    thread t1(&client, argv);
    thread t2(&server, argv);

//    t1.join();
    t2.join();

    return 0;
}

