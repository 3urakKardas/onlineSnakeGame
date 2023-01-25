#include "../tools/libraries.h"

namespace socketInit{

    int initWSADATA(WSADATA &ws);

    int getSocket(int &socketId);

    int initSockaddr(sockaddr_in &srv, int port);

    int initSocketOptions(int &socketId,int &nOptVal,int nOptLen);

    int initBind(int &socketId, sockaddr_in &srv);

    int initListen(int &socketId, int syncClientNum);

}
