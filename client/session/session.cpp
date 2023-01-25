#include "session.h"

Session::Session(){

    std::fill_n(primaryPlayerX,30,-1);
    std::fill_n(primaryPlayerY,30,-1);
    primaryPlayerX[0] = 5;
    primaryPlayerY[0] = 5;

    std::fill_n(secondaryPlayerX,30 ,-1);
    std::fill_n(secondaryPlayerY,30 ,-1);
    secondaryPlayerX[0] = 10;
    secondaryPlayerY[0] = 10;

}

void Session::initSession(){

    std::fill_n(primaryPlayerX,30,-1);
    std::fill_n(primaryPlayerY,30,-1);
    primaryPlayerX[0] = 5;
    primaryPlayerY[0] = 5;

    std::fill_n(secondaryPlayerX,30 ,-1);
    std::fill_n(secondaryPlayerY,30 ,-1);
    secondaryPlayerX[0] = 10;
    secondaryPlayerY[0] = 10;

}

Session::~Session(){

}

void Session::drawTable(sf::RenderWindow &win){

    sf::RectangleShape tempRectangle(sf::Vector2f(tileSize,tileSize));

    float rest1 = (600 - (tileSize * xTileNumber))/(xTileNumber);
    float rest2 = (600 - (tileSize * yTileNumber))/(yTileNumber);

    tempRectangle.setFillColor(sf::Color::Green);

    for(int i = 0; i < xTileNumber; i++){

        for(int j = 0; j < yTileNumber; j++){

            tempRectangle.setPosition(sf::Vector2f(rest1*(i+1)+tileSize*i,rest2*(j+1)+tileSize*j));

            win.draw(tempRectangle);


        }
    }

    tempRectangle.setFillColor(sf::Color::Red);

    tempRectangle.setPosition(sf::Vector2f(rest1*(appleX+1)+tileSize*appleX,rest2*(appleY+1)+tileSize*appleY));

    win.draw(tempRectangle);

    tempRectangle.setFillColor(sf::Color::Blue);

    for(int m = 0; m < primaryPlayerSize; m++){

            tempRectangle.setPosition(sf::Vector2f(rest1*(primaryPlayerX[m]+1)+tileSize*primaryPlayerX[m],rest2*(primaryPlayerY[m]+1)+tileSize*primaryPlayerY[m]));

            win.draw(tempRectangle);


    }

    tempRectangle.setFillColor(sf::Color::Cyan);

    for(int n = 0; n < secondaryPlayerSize; n++){

            tempRectangle.setPosition(sf::Vector2f(rest1*(secondaryPlayerX[n]+1)+tileSize*secondaryPlayerX[n],rest2*(secondaryPlayerY[n]+1)+tileSize*secondaryPlayerY[n]));

            win.draw(tempRectangle);


    }

    if(shareData::won){

            messagePopUpTexture.loadFromFile("youLostPopUpImage.jpg");

            messagePopUpSprite.setTexture(messagePopUpTexture);

            win.draw(messagePopUpSprite);


    }else if(shareData::lost){

            messagePopUpTexture.loadFromFile("youWonPopUpImage.jpg");

            messagePopUpSprite.setTexture(messagePopUpTexture);

            win.draw(messagePopUpSprite);

    }

}

void Session::forwardSnake(short *snakeBodyX,short *snakeBodyY, char direction){

    if(direction == 'N'){

        *snakeBodyY -= 1;

    }else if(direction == 'S'){

        *snakeBodyY += 1;

    }else if(direction == 'W'){

        *snakeBodyX -= 1;

    }else if(direction == 'E'){

        *snakeBodyX += 1;

    }

    if(*snakeBodyX > 14){

        *snakeBodyX = 0;

    }else if(*snakeBodyX < 1){

        *snakeBodyX = 14;

    }

    if(*snakeBodyY > 14){

        *snakeBodyY = 0;

    }else if(*snakeBodyY < 1){

        *snakeBodyY = 14;

    }

}

void Session::forwardTurn(std::string data,int sockId){

    primaryPlayerDirection = data[1];
    secondaryPlayerDirection = data[3];

    if(primaryPlayerSocketId == -1 || secondaryPlayerSocketId == -1){
        primaryPlayerSocketId= (uint8_t)data[2];
        secondaryPlayerSocketId = (uint8_t)data[4];
    }

    appleX = (int)data[4];
    appleY = (int)data[5];

    if(true){

        for(int i = primaryPlayerSize-1; i > 0; i--){

            primaryPlayerX[i] = primaryPlayerX[i-1];
            primaryPlayerY[i] = primaryPlayerY[i-1];
        }

        forwardSnake(primaryPlayerX, primaryPlayerY, primaryPlayerDirection);

        for(int j = secondaryPlayerSize-1; j > 0; j--){

            secondaryPlayerX[j] = secondaryPlayerX[j-1];
            secondaryPlayerY[j] = secondaryPlayerY[j-1];
        }

        forwardSnake(secondaryPlayerX, secondaryPlayerY, secondaryPlayerDirection);

    }

    if(primaryPlayerX[0] == appleX && primaryPlayerY[0] == appleY){

        primaryPlayerSize += 1;

        shareData::applEaten = true;

    }else if(secondaryPlayerX[0] == appleX && secondaryPlayerY[0] == appleY){

        secondaryPlayerSize += 1;

        shareData::applEaten = true;
    }

    for(int i = 0; i < primaryPlayerSize; i++){

        if(secondaryPlayerX[0] == primaryPlayerX[i] && secondaryPlayerY[0] == primaryPlayerY[i]){

            shareData::lostPlayerSocketId = secondaryPlayerSocketId;

        }
    }

    for(int i = 0; i < secondaryPlayerSize; i++){

        if(primaryPlayerX[0] == secondaryPlayerX[i] && primaryPlayerY[0] == secondaryPlayerY[i]){

            shareData::lostPlayerSocketId = primaryPlayerSocketId;


        }
    }

}
