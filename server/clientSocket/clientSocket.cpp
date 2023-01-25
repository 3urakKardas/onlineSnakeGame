#include "clientSocket.h"

void ClientSocket::setSockId(int socketId){

    sockId = socketId;

}

int ClientSocket::getSockId(){

    return sockId;

}

ClientSocket::ClientSocket(int socketId){

    sockId = socketId;

}

void ClientSocket::insertToPipe(std::string command){

    pipeOfCommandsToBeSent.insert(pipeOfCommandsToBeSent.begin(), command);

}

std::string ClientSocket::getFromPipe(){

    return pipeOfCommandsToBeSent[pipeOfCommandsToBeSent.size()-1];

}
