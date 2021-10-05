#pragma once

#include <string>
#include <chrono>
#include <map>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static constexpr int BUFFER_SIZE = 1024;
std::string FIFO_FILENAME = "/tmp/ipc_test_provider.1";

auto TIMEOUT = std::chrono::milliseconds(10);

int openFifo(const char* filename, int flags)
{
    int error = open(filename, flags);
    if (-1 == error)
    {
        fprintf(stderr, "Can't open fifo\n");
        exit(0);
    }
    return error;
}

#define ENUM_START(NAME) const char *##NAME##_to_string(NAME val) { switch (val) {
#define ENUM_ADD(NAME) case NAME: return #NAME;
#define ENUM_END default: return "Invalid value"; } }

enum class MESSAGE
{
    not_found = -3,
    unknown_error = -2,
    unknown_client = -1,

    // this messages are only allowed to send from client
    connect = 0,
    waiting = 1,
    call_method = 2
};

// TODO: generate the map
const std::map<MESSAGE, const char*> kMessageMap = {
    {MESSAGE::not_found, "not_found"},
    {MESSAGE::unknown_error, "unknown_error"},
    {MESSAGE::unknown_client, "unknown_client"},
    {MESSAGE::connect, "connect"},
    {MESSAGE::waiting, "waiting"},
    {MESSAGE::call_method, "call_method"},
};

int findId(const std::string& msg)
{
    auto result = -1;

    auto firstIndex = msg.find("[");
    if (firstIndex == std::string::npos)
    {
        return result;
    }

    auto secondIndex = msg.find("]");
    if (secondIndex == std::string::npos)
    {
        return result;
    }

    return atoi(msg.substr(firstIndex + 1, secondIndex - 1).data());
}

std::tuple<MESSAGE, int> getMessage(const std::string& msg)
{
    std::tuple<MESSAGE, int> wrongResult = {MESSAGE::not_found, -1};

    auto firstIndex = msg.find("[");
    if (firstIndex == std::string::npos)
    {
        return wrongResult;
    }

    auto secondIndex = msg.find("]");
    if (secondIndex == std::string::npos)
    {
        return wrongResult;
    }

    auto msgString = msg.substr(firstIndex + 1, secondIndex - 1);
    for (auto& [key, str]: kMessageMap)
    {
        if (msgString == str)
        {
            return {key, findId(msg.substr(secondIndex + 1))};
        }
    }

    return wrongResult;
}

// TODO:
// we have to implement communication protocol between client and server
// client has to not just open file but sent message "connect_request" to server
// unless server sends it back message "connected"
// after this client can send data to the server with message "proceed data"
// server revieves the data and sends message "processing" and manipulate with data
// after this, server sents "result"
// and client sends "disconneted"
// each message must have client id(process id will be ok for now)
// server should process data in separate thread
// server should disconect client after some delay if the client is idle
// server has to send message "unknown_client" if client isn't connected
// client has to be able reconnect after "unknown_client" from the server
