#include <unistd.h>
#include <string.h>
#include <mutex>
#include <condition_variable>
#include "connection.h"
#include "constants.h"

#include <iostream>

using namespace fastcgi;
using namespace std;

connection::connection(pollfd *fd) : fd(fd)
{
    inputBuffSize = FCGI_BUFFER_SIZE;
    outputBuffSize = FCGI_BUFFER_SIZE;
    inputBuffer = new unsigned char[FCGI_BUFFER_SIZE];
    outputBuffer = new unsigned char[FCGI_BUFFER_SIZE];
    inputBytes = 0;
    outputBytes = 0;
}

connection::~connection()
{
    unique_lock<mutex> lock(writeLock);
    while (!writeDone)
        writeCv.wait(lock);
    std::cout << "calling ~connection!" << std::endl;
    ::close(fd->fd);
    fd->fd = -1;
    fd->events = 0;
    delete [] outputBuffer, inputBuffer;
}

void connection::readFd()
{
    unique_lock<mutex> lock(readLock);
    if (inputBuffSize == inputBytes)
        extendBuffer(&inputBuffer, inputBuffSize);
    long bytesRead = ::read(fd->fd, inputBuffer + inputBytes, inputBuffSize - inputBytes);
    if (bytesRead > 0)
        inputBytes += bytesRead;
    if (bytesRead == 0) {
        fd->fd = -1;
        fd->events = 0;
        shouldClose = true;
    }
    std::cout << "###readFd: reading done! bytesRead: " << bytesRead << " inputBytes: " << inputBytes << std::endl;
    lock.unlock();
    cv.notify_one();
}

void connection::writeFd()
{
    if (!outputBytes) return;
    unique_lock<mutex> lock(writeLock);
    long bytesWrite= ::write(fd->fd, outputBuffer, outputBytes);
    std::cout << "writeFd: bytesWrite: " << bytesWrite << std::endl;
    if (bytesWrite > 0) {
        if (bytesWrite < outputBytes)
            ::memmove(outputBuffer, outputBuffer + bytesWrite, outputBytes - bytesWrite);
        outputBytes -= bytesWrite;
    }
    if (!outputBytes) {
        writeDone = true;
        lock.unlock();
        writeCv.notify_one();
    }
    std::cout << "writeFd: writing done!" << std::endl;
}

void connection::extendBuffer(unsigned char ** buffer, unsigned int &bufferSize)
{
    unsigned char * temp = new unsigned char[bufferSize * 2];
    ::memcpy(temp, *buffer, bufferSize);
    delete [] *buffer;
    bufferSize *= 2;
    *buffer = temp;
}

int connection::read(unsigned char * buffer, unsigned int bufferSize)
{
    if (!bufferSize) return 0;
    unique_lock<mutex> lock(readLock);
    if (shouldClose) return -1;
    std::cout << "read: bufferSize: " << bufferSize << " inputBytes : " << inputBytes << std::endl;
    while (bufferSize > inputBytes) {
        std::cout << "read: waiting for data to be ready ..." << std::endl;
        cv.wait(lock);
    }
    ::memcpy(buffer, inputBuffer, bufferSize);
    ::memmove(inputBuffer ,inputBuffer + bufferSize, inputBytes - bufferSize);
    inputBytes -= bufferSize;
    std::cout << "read: done! inputBytes: " << inputBytes << std::endl;
    return 0;
}

int connection::write(unsigned char *buffer, unsigned int bufferSize)
{
    unique_lock<mutex> lock(writeLock);
    if (bufferSize > (outputBuffSize - outputBytes))
        extendBuffer(&outputBuffer, outputBuffSize);
    ::memcpy(outputBuffer + outputBytes, buffer, bufferSize);
    outputBytes += bufferSize;
    writeDone = false;
    return 0;
}
