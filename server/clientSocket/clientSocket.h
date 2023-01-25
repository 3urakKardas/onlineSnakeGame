#include "../tools/libraries.h"

class ClientSocket{

    private:

        int sockId;

        std::vector<std::string> pipeOfCommandsToBeSent;

    public:

        void insertToPipe(std::string command);

        std::string getFromPipe();

        void setSockId(int socketId);

        int getSockId();

        ClientSocket(int socketId);

};
