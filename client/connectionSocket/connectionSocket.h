#include "../tools/libraries.h"
#include "../command/command.h"
#include "../shareData.h"

class ConnectionSocket{

    private:

        std::thread sockThread;

        WSADATA ws;

        int socketId;

        struct sockaddr_in srv;

        char *srvIp;
        int srvPort;

        char *request;

    public:

        int getSocketId();

        short sessionId = 0;

        std::vector<Command> commandVector;

        std::string activeSessionsBuffer;

        bool foo = false;

        bool waitingForRival = false;

        bool gameIsOn = false;

        bool waitingForNextTurn = false;

        void myFunc(){

            while(true){std::cout << "i am thread" << std::endl;}


        }

        void setRequest(char *req);

        std::string getRequest();

        void initConnection();

        void sockLoop();

        void activateSocket();

        ConnectionSocket();

};
