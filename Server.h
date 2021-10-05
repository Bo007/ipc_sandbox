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

        m_pid = getpid();
        printf("Server pid: %d\t parentpid: %d\n", m_pid, getppid());
    }

    ~Server()
    {
        unlink(FIFO_FILENAME.c_str());
    }

    void processMessage()
    {
        int byteNum = read(m_fifoHanlde, m_messageBuffer.data(), m_messageBuffer.size());
        if (byteNum == -1)
        {
            std::cout << "read was interrupted due to a signal.\n";
            return;
        }

        std::cout<<"processMessage " << m_messageBuffer.data() << std::endl;
        auto result = getMessage(m_messageBuffer.data());

        switch (std::get<0>(result))
        {
        case MESSAGE::connect:
        {
            const std::string msg = std::string("[") + kMessageMap.at(MESSAGE::connect) + "][" + std::to_string(m_pid) + "]";
            writeData(msg.data(), msg.size(), std::get<1>(result));
            break;
        }
        case MESSAGE::call_method:
        {
            convertStringToUpperCase(std::get<1>(result));
            break;
        }
        case MESSAGE::waiting:
        {
            break;
        }
        case MESSAGE::not_found:
        {
            break;
        }
        case MESSAGE::unknown_client:
        {
            break;
        }
        case MESSAGE::unknown_error:
        {
            break;
        }
        default:
            break;
        }

        std::memset(m_messageBuffer.data(), 0, m_messageBuffer.size());
    }

private:
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

    void convertStringToUpperCase(int processId)
    {
        std::memset(m_messageBuffer.data(), 0, m_messageBuffer.size());
        std::cout<< "waiting data from client" << std::endl;
        int byteNum = read(m_fifoHanlde, m_messageBuffer.data(), m_messageBuffer.size());
        if (byteNum == -1)
        {
            std::cout << "read was interrupted due to a signal.\n";
            return;
        }
        std::cout<<"Data from the Client: " << m_messageBuffer.data() << std::endl;

        std::string str(std::begin(m_messageBuffer), std::begin(m_messageBuffer) + byteNum);
        std::memset(m_messageBuffer.data(), 0, m_messageBuffer.size());

        std::string buff = toUpperCase(str);
        std::memcpy(m_messageBuffer.data(), buff.data(), buff.size());

        std::cout<<"Data to the Client: " << m_messageBuffer.data() << std::endl;
        writeData(m_messageBuffer.data(), m_messageBuffer.size(), processId);
    }

    void reopenFifo()
    {
        std::cout << "reopenFifo" << std::endl;
        close(m_fifoHanlde);
        m_fifoHanlde = openFifo(FIFO_FILENAME.c_str(), O_RDWR);
        std::memset(m_messageBuffer.data(), 0, m_messageBuffer.size());
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

    void writeData(const char* data, size_t size, int processId)
    {
        write(m_fifoHanlde, data, size);
        waitid(processId);
    }

private:
    int m_pid;
    int m_fifoHanlde;
    std::array<char, BUFFER_SIZE> m_messageBuffer;
};
