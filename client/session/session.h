#include "../tools/libraries.h"
#include "../shareData.h"

class Session{

    private:

        int xTileNumber = 15, yTileNumber = 15;
        int tileSize = 35;

    public:

        sf::Texture messagePopUpTexture;
        sf::Sprite messagePopUpSprite;

        short primaryPlayerX[30], primaryPlayerY[30];
        int primaryPlayerSize = 1;
        short primaryPlayerSocketId = -1;
        char primaryPlayerDirection = 'S';

        short secondaryPlayerX[30], secondaryPlayerY[30];
        int secondaryPlayerSize = 1;
        short secondaryPlayerSocketId = -1;
        char secondaryPlayerDirection = 'N';

        short appleX = 7;
        short appleY = 7;

        unsigned int turnCount = 0;

        void initSession();

        Session();

        ~Session();

        void drawTable(sf::RenderWindow &win);

        void forwardSnake(short *snakeBodyX,short *snakeBodyY, char direction);

        void forwardTurn(std::string data,int sockId);


};
