#include "connectionSocket.h"

#define PORT 9909
#define IP "127.0.0.1"

//the "IP" value represents the local ip address of the machine the server is running on,
//assuming both programs are to be used on the same machine it was given the value "127.0.0.1"
//which represents the ip of the machine the corresponding program is running on,
//should somebody want to use both on different computers, this value has to be changed to public ip
//of the server and if that is working behind a nat the port forwarding shall be applied,
//i.e. the value of the port associated with the server should be mapped to a local ipv4 address

ConnectionSocket::ConnectionSocket(){

    srvIp = IP;
    srvPort = PORT;

    request = (char *)malloc(8 * sizeof(char));

    request = "";

}

int ConnectionSocket::getSocketId(){

    return socketId;
}

void ConnectionSocket::activateSocket(){

    sockThread = std::thread(sockLoop,this);

}

void ConnectionSocket::initConnection(){

    if(WSAStartup(MAKEWORD(2, 2), &ws) < 0){

        std::cout << "WSAStartup() in initConnection failed...";

        WSACleanup();
        exit(EXIT_FAILURE);
    }

    socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socketId < 0){

        std::cout << "socket() in initConnection failed... ";

        WSACleanup();
        exit(EXIT_FAILURE);
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(srvPort);
    srv.sin_addr.s_addr = inet_addr(srvIp);
    memset(&(srv.sin_zero), 0, 8);

    if(connect(socketId, (struct sockaddr*)&srv, sizeof(srv)) < 0){

        std::cout << "connect() in initConnection failed..." << std::endl;

        WSACleanup();
        exit(EXIT_FAILURE);
    }

}

void ConnectionSocket::sockLoop(){

    initConnection();

    static char *buff;
    buff = (char*)malloc(8*sizeof(char));

    while(true){

        if(request != ""){

        if(std::string(request) == "crtgame"){

            send(socketId,request,7,0);

            recv(socketId,buff,7,0);

            buff[7] = '\0';

            sessionId = static_cast<int>(buff[6]);

            request = "";

        }if(std::string(request)=="siolist"){

            std::cout << "siolist has been taken" << std::endl;

            send(socketId,request,7,0);

            int c = 0;

            activeSessionsBuffer = "";

            while(c < 246){

                char *t = {0,};

                t = (char *)malloc(7*sizeof(char));

                int a = recv(socketId,t,7,0);

                if(a!=-1){
                    for(int x = 0;x < a;x++){

                        activeSessionsBuffer += t[x];
                    }

                   c+=a;
                }

            }

            std::cout << "the list is come" << std::endl;

               Command tempCommand;

               tempCommand.setHeader("sessionListGot");

               commandVector.push_back(tempCommand);

               request = "";

            }else if(std::string(request).substr(0,6) == "joinse"){

                send(socketId, request, 7, 0);

                int y = recv(socketId, buff, 7, 0);

                buff[7] = '\0';

                if(std::string(buff) == "sejoind"){

                    shareData::won = false;

                    shareData::lost = false;

                    sessionId = (unsigned int)request[6];

                    Command tempCommand;
                    tempCommand.setHeader("startSession");

                    commandVector.push_back(tempCommand);
                }

                request = "";

            }else if(std::string(request).substr(0,5) == "cmmnd"){

                send(socketId, request, 7, 0);

                recv(socketId, buff, 7, 0);

                buff[7] = '\0';

                request = "";

            }else if(std::string(request) == "endssio"){

                send(socketId, request, 7, 0);

                recv(socketId, buff, 7, 0);

                buff[7] = '\0';

                if(std::string(buff) == "erasedd"){

                    waitingForRival = false;
                }

                request = "";

            }

            }

            if(waitingForRival){

                send(socketId, "waiting", 7, 0);

                recv(socketId, buff, 7, 0);

                buff[7] = '\0';

                if(std::string(buff).substr(0,6) == "rivlfn"){

                    sessionId = (uint8_t)buff[6];

                    waitingForRival = false;

                    Command tempCommand;
                    tempCommand.setHeader("startSession");

                    commandVector.push_back(tempCommand);

                }else if(std::string(buff) == "looking"){

                    std::cout << "still looking for rival" << std::endl;

                }
            }

            if(gameIsOn){

                char tt[7] = {'n','e','x','t','u','r',static_cast<char>(sessionId)};

                send(socketId,tt , 7, 0);

                buff[7] = '\0';

                recv(socketId, buff, 7, 0);

                if(std::string(buff)[6] == '+'){

                    Command tempCommand;
                    tempCommand.setHeader("gameDataIsHere");
                    tempCommand.setData(buff);

                    commandVector.push_back(tempCommand);

                }else if(std::string(buff) == "youlost"){

                    shareData::lost = true;

                }else if(std::string(buff) == "youwonn"){

                    shareData::won = true;

                }else if(std::string(buff) == "gmended"){

                    Command tempCommand;
                    tempCommand.setHeader("endGame");

                    commandVector.push_back(tempCommand);


                }

            }

            if(shareData::applEaten){

                char buff[7] = {'a','p','p','l','e','e',(char)sessionId};

                send(socketId ,buff , 7, 0);

                shareData::applEaten = false;

                recv(socketId, buff, 7, 0);
            }

            if(shareData::lostPlayerSocketId != -1){

                char buff[7] = {'l','o','s','t','p',(char)shareData::lostPlayerSocketId,(char)sessionId};

                send(socketId,buff , 7, 0);

                shareData::lostPlayerSocketId = -1;

                recv(socketId, buff, 7, 0);

                buff[7] = '\0';

            }

    }

}

void ConnectionSocket::setRequest(char *req){

    request = req;

}

std::string ConnectionSocket::getRequest(){

    return request;

}
