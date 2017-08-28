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

unsigned char * responder::getHeader(const char * header, unsigned int &size)
{
    unsigned int i = 0;
    size_t headerLength = strlen(header);
    unsigned int nameLength = 0;
    unsigned int valueLength = 0;
    while(i < paramsLength) {
        if (params[i] >> 7) {
            nameLength = ((params[i] & 0x7f) << 24) + (params[i + 1] << 16) + (params[i + 2] << 8) + params[i + 3];
            i += 4;
        } else {
            nameLength = params[i];
            i += 1;
        }
        if (params[i] >> 7) {
            valueLength = ((params[i] & 0x7f) << 24) + (params[i + 1] << 16) + (params[i + 2] << 8) + params[i + 3];
            i += 4;
        } else {
            valueLength = params[i];
            i += 1;
        }
        if (nameLength != headerLength) {
            i += nameLength + valueLength;
            continue;
        }
        if (strncmp((const char *)&params[i], header, nameLength) == 0) {
            size = valueLength;
            return &params[i+ nameLength];
        }

    }
    return nullptr;
}

unsigned char * responder::getStdIn(unsigned int &length)
{
    length = stdInLength;
    return stdIn;
}