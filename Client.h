#pragma once

#include <unistd.h>
#include <sys/wait.h>

#include <thread>
#include <iostream>

#include "ServerSettings.h"

// TODO: Client recieve input data from the user and after this sends it the server
// waiting till it proceeds our request and returns data to the use
// firstly we can implement it by command line parameters
class Client
{
public:

    Client()
    {
        while (-1 == (m_fifoHanlde = open(FIFO_FILENAME.c_str(), O_RDWR)))
        {
            std::this_thread::sleep_for(TIMEOUT);
            std::cout << "client waits for server " << TIMEOUT.count() << std::endl;
        }

        printf("client pid: %d\t parentpid: %d\n", getpid(), getppid());
    }

    ~Client()
    {
        close(m_fifoHanlde);
    }

    void readString()
    {
        std::cout<<"Enter data: ";
        getline(std::cin, m_data);
    }

    void sendStringToServer()
    {
        std::cout<<"sendStringToServer" << std::endl;
        write(m_fifoHanlde, m_data.data(), m_data.size());
        std::cout<<"sendStringToServer done" << std::endl;
    }

    void recieveDataFromServer()
    {
        read(m_fifoHanlde, m_data.data(), m_data.size());
        std::cout<<"Data from the Server: " << m_data << std::endl;
    }

private:
    int m_fifoHanlde;
    std::string m_data;
};
