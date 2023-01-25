#include "../tools/libraries.h"
#include "../shareData.h"
#include "../connectionSocket/connectionSocket.h"
#include "../session/session.h"


#ifndef activeScreen

enum activeScreen{

    mainMenuScreen,
    sessionMenuScreen,
    sessionWaitsSecondPlayer,
    inGameScreen

};
#endif

class Game{

    private:

        ConnectionSocket associatedConnectionSocket;

        activeScreen currentScreen;

        std::vector<int> sessionList;

    public:

        Session associatedSession;

        static const short gameScreenWidth = 600,gameScreenHeight = 600;

        void manageConnection();

        void initializeVisualProcess();

        void run();

        Game();

};
