#include "Client.h"

int main()
{
    Client client;
    while (!client.connectToServer())
    {
        std::this_thread::sleep_for(TIMEOUT);
        std::cout << "client waits for server " << TIMEOUT.count() << std::endl;
    }

    client.readString();
    client.callMethod();

    return 0;
}
