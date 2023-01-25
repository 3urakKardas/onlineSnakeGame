#include "../tools/libraries.h"

class Command{

    private:

        std::string header;

        std::string data;

        char primaryChar;

        char secondaryChar;

    public:

        Command();

        std::string getHeader();

        std::string getData();

        char getPrimaryChar();

        char getSecondaryChar();

        void setHeader(std::string hdr);

        void setData(std::string dta);

        void setPrimaryChar(char primary);

        void setSecondaryChar(char secondary);


};
