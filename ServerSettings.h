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
