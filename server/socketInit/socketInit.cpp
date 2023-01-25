#include "socketInit.h"

int socketInit::initWSADATA(WSADATA &ws){

    int rp = WSAStartup(MAKEWORD(2,2),&ws);

    if(rp < 0){

        std::cout << "Error in initializing WSADATA!!!" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }else{
        std::cout << "WSADATA succesfully initialize!!! " << rp << std::endl;
    }

    return rp;

}


int socketInit::getSocket(int &socketId){

    socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socketId < 0){

        std::cout << "Error in initializing socket!!!" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }else{

        std::cout << "Socket initialized succsfully " << socketId << std::endl;

    }

    return socketId;

}
int socketInit::initSockaddr(sockaddr_in &srv, int port){

    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = INADDR_ANY;

    memset(&(srv.sin_zero), 0, 8);

}

int socketInit::initSocketOptions(int &socketId,int &nOptVal,int nOptLen){

    int rp = setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);

    if(rp < 0){

        std::cout << "Error in initsocketoptions!!!" << std::endl;

        WSACleanup();
        exit(EXIT_FAILURE);


    }else{

        std::cout << "initsocketoptions worked fine" << std::endl;

    }

}

int socketInit::initBind(int &socketId, sockaddr_in &srv){

    int rp = bind(socketId, (sockaddr*)&srv, sizeof(sockaddr));

    if(rp < 0){

        std::cout << "error occured in binding !!!" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);

    }else{

        std::cout << "binding succcccccccccccc" << std::endl;

    }

}

int socketInit::initListen(int &socketId, int syncClientNum){

    int rp = listen(socketId, syncClientNum);

    if(rp < 0){

        std::cout << "Error in initializing listen!!!" << std::endl;

        WSACleanup();
        exit(EXIT_FAILURE);

    }else{

        std::cout << "listening has begun " << rp << std::endl;

    }

    return rp;

}


