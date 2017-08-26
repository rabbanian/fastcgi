#include "responder.h"
#include <string.h>

using namespace fastcgi;

void responder::writeHeader(const void *buffer, unsigned int bufferSize)
{
    unsigned char *temp = new unsigned char[headerLength + bufferSize];
    ::memcpy(temp, headerBuffer, headerLength);
    ::memcpy(temp + headerLength, buffer, bufferSize);
    delete [] headerBuffer;
    headerBuffer = temp;
    headerLength += bufferSize;
}

void responder::writeBody(const void *buffer, unsigned int bufferSize)
{
    unsigned char *temp = new unsigned char[bodyLength + bufferSize];
    ::memcpy(temp, bodyBuffer, bodyLength);
    ::memcpy(temp + bodyLength, buffer, bufferSize);
    delete [] bodyBuffer;
    bodyBuffer = temp;
    bodyLength += bufferSize;
}

void responder::setParams(unsigned char *buff, unsigned int size)
{
    params = buff;
    paramsLength = size;
}

void responder::setStdIn(unsigned char *buff, unsigned int size)
{
    stdIn = buff;
    stdInLength = size;
}

void responder::setData(unsigned char *buff, unsigned int size)
{
    data = buff;
    dataLength = size;
}

unsigned char * responder::getBuffer(unsigned int &size)
{
    //maybe buffer is nullptr! --> what happens?
    size = headerLength + bodyLength;
    unsigned char * temp = new unsigned char[size];
    ::memcpy(temp, headerBuffer, headerLength);
    ::memcpy(temp + headerLength, bodyBuffer, bodyLength);
    delete [] headerBuffer, bodyBuffer;
    headerLength = bodyLength = 0;
    return temp;
}
