#include "request.h"
#include "responder.h"
#include <unistd.h>

using namespace fastcgi;

request::request(int sock, unsigned char t, unsigned int reqID, responder * res)
        : sockfd(sock), type(t), requestId(reqID), handler(res)
{

}

int request::write(unsigned char *buffer, unsigned int bufferSize)
{
    int bytesWrite = 0, result = 0;
    while ((result = ::write(sockfd, buffer + bytesWrite, bufferSize - bytesWrite)) > 0) {
        bytesWrite += result;
    }
    return 0;
}

int request::read(unsigned char * buffer, unsigned int bufferSize)
{
    int bytesRead = 0, result = 0;
    while ((result = ::read(sockfd, buffer + bytesRead, bufferSize - bytesRead)) > 0) {
        bytesRead += result;
    }
    return 0;
}

int request::control(unsigned char *buffer, unsigned int bufferSize)
{
    contentData = buffer;
    contentLength = bufferSize;
    switch (type) {
        case 1:
            beginRequest();
            break;
        case 2:
            abortRequest();
            break;
        case 4:
            params();
            break;
        case 5:
            stdIn();
            break;
        case 8:
            data();
            break;
    }
    if (shouldClose)
        return -1; // in case of the need for closing connection!
    else
        return 0;
}

void request::beginRequest()
{
    role = (contentData[0] << 8) + contentData[1];
    flags = contentData[2];
    delete [] contentData;
}

void request::abortRequest()
{

    delete [] contentData;
}

void request::params()
{

    delete [] contentData;
}

void request::stdIn()
{
    shouldClose = true;
    delete [] contentData;
}

void request::data()
{

    delete [] contentData;
}

unsigned int request::getRequestId()
{
    return requestId;
}

void request::setType(unsigned char t)
{
    type = t;
}

