#pragma once

#include <string>
#include <chrono>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
