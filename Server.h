#pragma once

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

// TODO: server should have process request method and return std::future with related data
// for this we have to maintain a map with names and methods
// as parameter we recieve a name of method and additional parameters as string
// after this we parse method name and search for it the map
// we doing the same for the method parameters and matching it with our procedure
// after this we proceed it async and return the result
class Server
{
public:

    Server(int inputHandle, int outputHandle)
        : m_inputHandle(inputHandle)
        , m_outputHandle(outputHandle)
    {
        printf("Server pid: %d\t parentpid: %d\n", getpid(), getppid());
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
        read(m_inputHandle, m_inputBuff.data(), m_inputBuff.size());
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
        write(m_outputHandle, m_inputBuff.data(), m_inputBuff.size());
        wait(NULL);
    }


private:
    int m_inputHandle;
    int m_outputHandle;

    static constexpr int BUFFER_SIZE = 1024;
    std::array<char, BUFFER_SIZE> m_inputBuff;
};
