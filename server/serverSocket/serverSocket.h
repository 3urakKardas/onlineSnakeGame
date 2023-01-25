#include "../socketInit/socketInit.h"
#include "../clientSocket/clientSocket.h"
#include"../gameSession/gameSession.h"

class serverSocket{


    private:

        const static int port = 9909;

        WSADATA ws;

        int socketId;

        struct sockaddr_in srv;

        int bindStatus;

        fd_set fr, fw, fe;

        int  writingSocket;

        struct timeval tv;

        int nOptVal;
        int nOptLen;

        std::vector <ClientSocket> clientSocketVector;

        std::vector <GameSession> gameSessionVector;


    public:

        void processMessage(int sockId);

        void processNewConnection();

        void run();

        serverSocket();

        ~serverSocket(){

            std::cout << "jobs ended" << std::endl;

        }



};
