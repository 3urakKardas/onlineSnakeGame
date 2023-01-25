#include "serverSocket/serverSocket.h"

int main()
{
    serverSocket mySocket = serverSocket();

    mySocket.run();

    return 0;
}
