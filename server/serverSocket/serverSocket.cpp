#include "serverSocket.h"

#ifndef messageCode

enum messageCode{
    srvison,
    siolist,
    crtgame,
    joingme,
    joinse,
    noncode
    };

#endif

#ifndef getMessageCode

messageCode getMessageCode(char *message){

    std::string str = std::string(message);

    if(str == "srvison"){

        std::cout << "server status requested" << std::endl;

        return srvison;

    }if(str == "siolist"){

        std::cout << "list of available sessisons requested" << std::endl;

        return siolist;

    }else if(str == "crtgame"){

        std::cout << "game is being created !!!" << std::endl;

        return crtgame;

    }else if(str == "joingme"){

        return joingme;

    }else if(str.substr(0,6) == "joinse"){

        std::cout << "join sesssion" << std::endl;

        return joinse;

    }

    return noncode;
}


#endif

serverSocket::serverSocket(){

}

void serverSocket::processMessage(int sockId){

    char messageBuff[8];

    int re = recv(sockId, messageBuff, 7, 0);

    messageBuff[7] = '\0';

    if(re < 0){

        std::cout << "error in receiving message " << std::endl;

        closesocket(sockId); //does not affect the data on file descriptor

        for(unsigned short i = 0; i < clientSocketVector.size(); i++){

            if(clientSocketVector[i].getSockId() == sockId){

                clientSocketVector.erase(clientSocketVector.begin() + i);

                break;
            }
        }
    }else{

        for(auto i : gameSessionVector){

                std::cout << "one game session ,primary player:" << i.getPrimaryPlayerSocket() << " second player:" << i.getSecondaryPlayerSocket() << std::endl;
        }

        std::string str = std::string(messageBuff);

        if(str=="srvison"){

            send(sockId, "srvamon", 7, 0);

        }else if(str=="siolist"){

            char *availableSessionsBuffer = new char[246];

            memset(availableSessionsBuffer, 0, 246);

            unsigned short sessionCount = 0;

            for(auto i : gameSessionVector){

                if(i.getSecondaryPlayerSocket() == 0){

                    char *temp = new char[1];

                    temp[0] = i.getId();

                    availableSessionsBuffer[sessionCount] = temp[0];

                    sessionCount++;

                    }
                }

            availableSessionsBuffer[245] = '\0';

            send(sockId, availableSessionsBuffer, 246, 0);

        }else if(std::string(str) == "crtgame"){

            int idCount = 1;

            while(true){

                bool isTaken = false;

                for(auto i : gameSessionVector){

                    if(idCount == i.getId()){

                        isTaken = true;

                    idCount++;
                    break;

                    }
                }

                if(!isTaken){

                    break;
                }
            }

            gameSessionVector.push_back(GameSession(sockId,idCount));

            char buff[7] = {'g','m','s','t','r','t',static_cast<char>(idCount)};

            send(sockId,buff,7,0);

            }else if(str.substr(0,6)=="joinse"){

                char *a = new char[2];

                for(auto &i : gameSessionVector){

                    if(i.getId() == (int)(unsigned char)str[6]){

                        i.setSecondaryPlayerSocket(sockId);

                        break;

                    }
                }

                char *buff;
                buff = (char*)malloc(7*sizeof(char));
                buff = "sejoind";
                int ter = send(sockId ,buff, 7, 0);

            }else if(str=="waiting"){

                bool found = false;

                for(auto i : gameSessionVector){

                    if(i.getPrimaryPlayerSocket() == sockId && i.getSecondaryPlayerSocket() != 0){

                    found = true;

                    char temp[7] = {'r','i','v','l','f','n',(char)i.getId()};

                    send(sockId ,temp, 7, 0);

                    break;

                  }
               }

               if(!found){

                    send(sockId, "looking", 7, 0);

               }

           }else if(std::string(str).substr(0,6)=="ssiodt"){

               bool responded = false;

               for(auto &i : gameSessionVector){

                    if(i.getId() == (unsigned int)str[6]){

                       if(!i.getPlayerDataSent(sockId)){

                           if(i.getClientTurnCount(sockId) <= 3){

                               send(sockId, "moveonn", 7, 0);

                            }else{

                                send(sockId, "abcdef+", 7, 0);
                            }

                            i.changePlayerDataSent(sockId);
                            responded = true;

                       }
                   }
                }

                if(!responded){

                    send(sockId, "errorrrr", 7, 0);

                }

           }else if(std::string(str).substr(0,6)=="nextur"){

               bool forwarded = false;

               for(auto &i : gameSessionVector){

                    if(i.getId() == (unsigned int)str[6]){

                        if(i.sentCount == 2){

                            send(sockId, "gmended", 7, 0);

                            i.gotCount += 1;

                            if(i.gotCount == 2){
                                for(short i = 0; i < gameSessionVector.size(); i++){

                                    gameSessionVector.erase(gameSessionVector.begin() + i);
                                }
                            }

                        }else if(i.getPlayerLost(sockId)){

                            send(sockId, "youlost", 7, 0);

                            i.sentCount += 1;

                        }else if(i.getPlayerLost(i.getOtherPlayerSocket(sockId))){

                            send(sockId, "youwonn", 7, 0);

                            i.sentCount += 1;

                        }else if(!i.getPlayerDataSent(sockId) && !i.getOtherPlayerDataSent(sockId)){


                            char temp[7] = {static_cast<char>(i.getPrimaryPlayerSocket()),i.primaryPlayerDirection,static_cast<char>(i.getSecondaryPlayerSocket()),i.secondaryPlayerDirection,(char)i.appleX,(char)i.appleY,'+'};
                            for(short k = 0; k < 7; k++){*(i.sharedSessionMessage+k)=*(temp+k);}

                            send(sockId,temp, 7, 0);

                            i.changePlayerDataSent(sockId);

                        }else if(!i.getPlayerDataSent(sockId) && i.getOtherPlayerDataSent(sockId)){

                            send(sockId, i.sharedSessionMessage, 7, 0);

                            i.changePlayerDataSent(sockId);


                        }else if(i.primaryPlayerDataSent == true && i.secondaryPlayerDataSent == true){

                            send(sockId, "forward", 7, 0);

                            i.primaryPlayerDataSent = false;
                            i.secondaryPlayerDataSent = false;

                        }else{

                            send(sockId,"waitttt", 7, 0);

                        }

                        forwarded = true;

                    }
               }

               if(!forwarded){

                   send(sockId,"waitttt", 7, 0);
               }


           }else if(std::string(str).substr(0,5) == "cmmnd"){

              for(auto &i : gameSessionVector){

                  if(i.getId() == (int)str[6]){

                        i.setDirection(sockId, str[5]);

                  }
              }

              send(sockId, "hehehhh", 7, 0);

           }else if(std::string(str).substr(0,6) == "applee"){

               for(auto &i : gameSessionVector){

                    if(i.getId() == (unsigned int)str[6]){

                        i.appleX = rand()%15;
                        i.appleY = rand()%15;

                    }

               }

               send(sockId, "itsdone", 7, 0);

           }else if(std::string(str).substr(0,5) == "lostp"){

               for(auto &i : gameSessionVector){

                    if(i.getId() == (uint8_t)str[6]){

                        i.setPlayerLost((uint8_t)str[5]);

                        send(sockId, "jobsdon", 7, 0);

                    }
               }


           }else if(std::string(str) == "endssio"){

               for(short i = 0; i < gameSessionVector.size(); i++){

                    if(gameSessionVector[i].getPrimaryPlayerSocket() == sockId || gameSessionVector[i].getSecondaryPlayerSocket() == sockId){

                        gameSessionVector.erase(gameSessionVector.begin() + i);

                        send(sockId, "erasedd", 7, 0);

                    }

               }

           }


        }

}

void serverSocket::processNewConnection(){

    if(FD_ISSET(socketId, &fr)){

        int len = sizeof(struct sockaddr);

        int clientSocket = accept(socketId, NULL, &len);

        if(clientSocket > 0){

            clientSocketVector.push_back(ClientSocket(clientSocket));
        }
    }else{

        for(ClientSocket &i : clientSocketVector){

            if(FD_ISSET(i.getSockId(), &fr)){

                    processMessage(i.getSockId());

            }
        }

    }

}

void serverSocket::run(){

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    nOptVal = 0;
    nOptLen = sizeof(nOptVal);

    int selectRe = 0;

    socketInit::initWSADATA(ws);

    socketInit::getSocket(socketId);

    socketInit::initSockaddr(srv, port);

    socketInit::initSocketOptions(socketId, nOptVal, nOptLen);

    socketInit::initBind(socketId, srv);

    socketInit::initListen(socketId, 10);

    while(true){

        FD_ZERO(&fw);
        FD_ZERO(&fr);
        FD_ZERO(&fe);

        FD_SET(socketId, &fr);
        FD_SET(socketId, &fe);

        for(ClientSocket &i : clientSocketVector){

            FD_SET(i.getSockId(),&fr);
            FD_SET(i.getSockId(),&fw);

            std::cout << clientSocketVector.size() << std::endl;

        }

        selectRe = select(socketId+1, &fr, &fw, &fe,&tv);

        if(selectRe > 0){

            processNewConnection();

        }else if(selectRe == 0){

            std::cout << "there is nothing"  << std::endl;

        }else{

            std::cout << "error" << std::endl;

        }

        Sleep(300);
        //increasing this value would result in server reading the data accepted faster and as a result,
        //since they are dependent on server responses to perform operations, client programs running faster

    }

    std::cout << "run ended" << std::endl;



}
