#pragma once

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>

// TODO: Client recieve input data from the user and after this sends it the server
// waiting till it proceeds our request and returns data to the use
// firstly we can implement it by command line parameters
class Client
{
public:

    Client(int inputHandle, int outputHandle)
        : m_inputHandle(inputHandle)
        , m_outputHandle(outputHandle)
    {
        printf("client pid: %d\t parentpid: %d\n", getpid(), getppid());
    }

    void readString()
    {
        std::cout<<"Enter data: ";
        getline(std::cin, m_data);
    }

    void sendStringToServer()
    {
        write(m_outputHandle, m_data.data(), m_data.size());
        wait(NULL);
    }

    void recieveDataFromServer()
    {
        read(m_inputHandle, m_data.data(), m_data.size());
        std::cout<<"Data from the Server: " << m_data << std::endl;
    }

private:
    int m_inputHandle;
    int m_outputHandle;
    std::string m_data;
};
