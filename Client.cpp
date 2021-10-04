#include "Client.h"

int main()
{
    Client client;
    client.readString();
    client.sendStringToServer();
    client.recieveDataFromServer();

    return 0;
}
