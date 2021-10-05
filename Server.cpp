#include "Server.h"

int main()
{
    Server server;
    while (true)
    {
        server.processMessage();
    }
    return 0;
}
