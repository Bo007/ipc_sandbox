#pragma once

#include <unistd.h>
#include <sys/wait.h>

#include <thread>
#include <iostream>

#include "ServerSettings.h"

class Client
{
public:

    Client()
    {
        m_pid = getpid();
        printf("client pid: %d\t parentpid: %d\n", m_pid, getppid());
    }

    ~Client()
    {
        close(m_fifoHanlde);
    }

    bool connectToServer()
    {
        std::cout << "connectToServer" << std::endl;
        if (-1 == m_fifoHanlde)
        {
            m_fifoHanlde = open(FIFO_FILENAME.c_str(), O_RDWR);
        }

        if (-1 != m_fifoHanlde)
        {
            const std::string msg = std::string("[") + kMessageMap.at(MESSAGE::connect) + "][" + std::to_string(m_pid) + "]";
            writeData(msg);

            read(m_fifoHanlde, m_buffer.data(), m_buffer.size());

            const auto& [msgCode, pId] = getMessage(m_buffer.data());
            if (msgCode != MESSAGE::connect)
            {
                return false;
            }
            if (pId <= 0)
            {
                return false;
            }

            m_serverId = pId;
            return true;
        }

        return false;
    }

    void writeData(const std::string& data)
    {
        write(m_fifoHanlde, data.data(), data.size());
//        waitid(m_serverId);
    }

    void callMethod()
    {
        std::cout << "callMethod" << std::endl;
        const std::string msg = std::string("[") + kMessageMap.at(MESSAGE::call_method) + "][" + std::to_string(m_pid) + "]";
        writeData(msg);
        std::cout << "~callMethod" << std::endl;

        std::cout<<"sendStringToServer" << std::endl;
        writeData(m_data);
        std::cout<<"~sendStringToServer" << std::endl;

        read(m_fifoHanlde, m_data.data(), m_data.size());
        std::cout<<"Data from the Server: " << m_data << std::endl;
    }

    void readString()
    {
        std::cout<<"Enter data: ";
        getline(std::cin, m_data);
    }

private:
    int m_serverId{-1};
    int m_pid;
    int m_fifoHanlde{-1};
    std::string m_data;
    std::array<char, 1024> m_buffer;
};
