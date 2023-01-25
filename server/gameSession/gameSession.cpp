#include "gameSession.h"

GameSession::GameSession(int primaryPlayerSock, int sessionId){

    primaryPlayerSocket = primaryPlayerSock;

    id = sessionId;

}

int GameSession::getClientTurnCount(int sockId){

    if(primaryPlayerSocket == sockId){

        return primaryPlayerTurnCount;

    }else if(secondaryPlayerSocket == sockId){

        return secondaryPlayerTurnCount;

    }else{

        return -1;
    }

}

int GameSession::getOtherClientTurnCount(int sockId){

    if(primaryPlayerSocket == sockId){

        return secondaryPlayerTurnCount;

    }else if(secondaryPlayerSocket == sockId){

        return primaryPlayerTurnCount;

    }else{

        return -1;
    }

}

void GameSession::forwardTurn(int sockId){

    if(primaryPlayerSocket == sockId){

        primaryPlayerTurnCount++;

    }else if(secondaryPlayerSocket == sockId){

        secondaryPlayerTurnCount++;

    }

}

int GameSession::getClientTurnCountsDifference(){

    int temp = primaryPlayerTurnCount-secondaryPlayerTurnCount;

    if(temp > 0){

        return temp;
    }else{

        temp *= -1;

        return temp;
    }

    return -1;

}

void GameSession::changePlayerDataSent(int sockId){

    if(sockId == primaryPlayerSocket){

        primaryPlayerDataSent = true;

    }else if(sockId == secondaryPlayerSocket){

        secondaryPlayerDataSent = true;
    }

}

bool GameSession::getPlayerDataSent(int sockId){

    if(sockId == primaryPlayerSocket){

        return primaryPlayerDataSent;

    }else if(sockId == secondaryPlayerSocket){

        return secondaryPlayerDataSent;
    }

}

bool GameSession::getPlayerLost(int sockId){

    if(sockId == primaryPlayerSocket){

        return primaryPlayerLost;

    }else if(sockId == secondaryPlayerSocket){

        return secondaryPlayerLost;
    }

}

int GameSession::getOtherPlayerSocket(int sockId){

    if(sockId == primaryPlayerSocket){

        return secondaryPlayerSocket;

    }else if(sockId == secondaryPlayerSocket){

        return primaryPlayerSocket;
    }

    return -1;

}

void GameSession::setPlayerLost(int sockId){

    if(sockId == primaryPlayerSocket){

        primaryPlayerLost = true;

    }else if(sockId == secondaryPlayerSocket){

        secondaryPlayerLost = true;
    }

}

bool GameSession::getOtherPlayerDataSent(int sockId){

    if(sockId == primaryPlayerSocket){

        return secondaryPlayerDataSent;

    }else if(sockId == secondaryPlayerSocket){

        return primaryPlayerDataSent;
    }

}

void GameSession::setDirection(int sockId, char direction){

    if(sockId == primaryPlayerSocket){

        primaryPlayerDirection = direction;

    }else if(sockId == secondaryPlayerSocket){

        secondaryPlayerDirection = direction;
    }

}

int GameSession::getPrimaryPlayerSocket(){

    return primaryPlayerSocket;

}

int GameSession::getSecondaryPlayerSocket(){

    return secondaryPlayerSocket;

}

int GameSession::getId(){

    return id;

}

void GameSession::setSecondaryPlayerSocket(int secondary){

    secondaryPlayerSocket = secondary;

}

