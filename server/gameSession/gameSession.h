#include "../tools/libraries.h"

class GameSession{

    private:

        unsigned long long turn = {0};

        int primaryPlayerSocket = {0};

        int secondaryPlayerSocket = {0};

        int id;

    public:

        short primaryPlayerTurnCount = 0;
        short secondaryPlayerTurnCount = 0;

        bool primaryPlayerDataSent = false;
        bool secondaryPlayerDataSent = false;

        void changePlayerDataSent(int sockId);
        bool getPlayerDataSent(int sockId);
        bool getOtherPlayerDataSent(int sockId);

        char primaryPlayerDirection = 'N';
        char secondaryPlayerDirection = 'S';

        void setDirection(int sockId ,char direction);

        int getClientTurnCount(int sockId);
        int getOtherClientTurnCount(int sockId);

        int appleX = 7, appleY = 7;

        short sentCount = 0;
        short gotCount = 0;

        bool primaryPlayerLost = false;
        bool secondaryPlayerLost = false;

        bool getPlayerLost(int sockId);
        void setPlayerLost(int sockId);

        int getOtherPlayerSocket(int sockId);

        char sharedSessionMessage[7];

        void forwardTurn(int sockId);

        int getClientTurnCountsDifference();

        int getPrimaryPlayerSocket();

        int getSecondaryPlayerSocket();

        void setSecondaryPlayerSocket(int secondary);

        int getId();

        GameSession(int primaryPlayerSock, int sessionId);

        ~GameSession(){
        }


};
