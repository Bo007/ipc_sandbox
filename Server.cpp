#include "Server.h"

int main()
{
    Server server;
    while (true)
    {
        server.recieveDataFromClient();
        server.processData();
        server.sendDataToClient();
        server.reopenFifo();
    }
    return 0;
}
