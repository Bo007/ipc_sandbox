#include <nlohmann/json.hpp>

#include "Client.h"
#include "Server.h"

int main()
{
    int clientToServer[2];
    int serverToClient[2];

    if (pipe(clientToServer))
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pipe(serverToClient))
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0: // child
    {
        close(clientToServer[0]);
        close(serverToClient[1]);

        Client client(serverToClient[0], clientToServer[1]);
        client.readString();
        client.sendStringToServer();
        client.recieveDataFromServer();

        break;
    }
    default: // parent
    {
        close(serverToClient[0]);
        close(clientToServer[1]);

        Server server(clientToServer[0], serverToClient[1]);

        server.recieveDataFromClient();
        server.processData();
        server.sendDataToClient();

        break;
    }
    }

    return 0;
}
