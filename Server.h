#pragma once

#include <unistd.h>
#include <sys/wait.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "ServerSettings.h"

// TODO: server should have process request method and return std::future with related data
// for this we have to maintain a map with names and methods
// as parameter we recieve a name of method and additional parameters as string
// after this we parse method name and search for it the map
// we doing the same for the method parameters and matching it with our procedure
// after this we proceed it async and return the result
class Server
{
public:

    Server()
    {
        unlink(FIFO_FILENAME.c_str());

        createFifo(FIFO_FILENAME.c_str());
        m_fifoHanlde = openFifo(FIFO_FILENAME.c_str(), O_RDWR);

        printf("Server pid: %d\t parentpid: %d\n", getpid(), getppid());
    }

    ~Server()
    {
        unlink(FIFO_FILENAME.c_str());
    }

    template<class Type>
    Type processArraySum(const std::vector<Type>& data)
    {
        return !data.empty() ? std::accumulate(std::begin(data), std::end(data)) : Type();
    }

    std::string toUpperCase(std::string& str)
    {
        std::string result(str);
        std::transform(str.begin(), str.end(), result.begin(), ::toupper);
        return result;
    }

    void recieveDataFromClient()
    {
        std::cout<<"recieveDataFromClient waiting " << std::endl;
        read(m_fifoHanlde, m_inputBuff.data(), m_inputBuff.size());
        std::cout<<"Data from the Client: " << m_inputBuff.data() << std::endl;
    }

    void processData()
    {
        std::string str(std::begin(m_inputBuff), std::end(m_inputBuff));
        std::string buff = toUpperCase(str);
        std::memcpy(m_inputBuff.data(),  buff.data(), buff.size());
    }


    void sendDataToClient()
    {
        std::cout<<"Data to the Client: " << m_inputBuff.data() << std::endl;
        write(m_fifoHanlde, m_inputBuff.data(), m_inputBuff.size());
        wait(NULL);
    }

    void reopenFifo()
    {
        std::cout << "reopenFifo" << std::endl;
        close(m_fifoHanlde);
        m_fifoHanlde = openFifo(FIFO_FILENAME.c_str(), O_RDWR);
        std::memset(m_inputBuff.data(), 0, m_inputBuff.size());
    }
private:
    void createFifo(const char* filename)
    {
        int error = mkfifo(filename, 0666);
        if (-1 == error)
        {
            fprintf(stderr, "Can't create fifo\n");
            exit(0);
        }
    }

private:
    int m_fifoHanlde;

    static constexpr int BUFFER_SIZE = 1024;
    std::array<char, BUFFER_SIZE> m_inputBuff;
};
