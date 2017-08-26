#include "request.h"
#include "responder.h"
#include "connection.h"
#include "constants.h"
#include <iostream>
#include <string.h>

using namespace fastcgi;

request::request(connection * writer, unsigned char t, unsigned int reqID, responder * res)
        : io(writer), type(t), requestId(reqID), handler(res)
{

}

request::~request()
{
    delete [] paramsData, stdInData, dataData;
}

int request::control(unsigned char *buffer, unsigned int bufferSize)
{
    contentData = buffer;
    contentLength = bufferSize;
    switch (type) {
        case FCGI_BEGIN_REQUEST:
            std::cout << "beginRequest();" << std::endl;
            beginRequest();
            break;
        case FCGI_ABORT_REQUEST:
            std::cout << "abortRequest();" << std::endl;
            abortRequest();
            break;
        case FCGI_PARAMS:
            std::cout << "params();" << std::endl;
            streamRecord(paramsOver, &paramsData, paramsLength);
            handler->setParams(paramsData, paramsLength);
            break;
        case FCGI_STDIN:
            std::cout << "stdIn();" << std::endl;
            streamRecord(stdInOver, &stdInData, stdInLength);
            handler->setStdIn(stdInData, stdInLength);
            break;
        case FCGI_DATA:
            std::cout << "data();" << std::endl;
            streamRecord(dataOver, &dataData, dataLength);
            handler->setData(dataData, dataLength);
            break;
    }
    std::cout << "Control: paramsOver: " << paramsOver << " stdInOver: " << stdInOver  << " dataOver: " << dataOver << std::endl;
    if (paramsOver && stdInOver ) {
        handler->respond();
        unsigned int size = 0;
        unsigned char * temp = handler->getBuffer(size);
        io->write(temp, size);
        delete [] temp;
        shouldClose = true;
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

void request::streamRecord(bool &isOver, unsigned char **data, unsigned int &length)
{
    if (!contentLength) {
        isOver = true;
        delete [] contentData;
        return;
    }
    unsigned char *temp = new unsigned char[contentLength + length];
    ::memcpy(temp, *data, length);
    ::memcpy(temp + length, contentData, contentLength);
    delete [] *data, contentData;
    *data = temp;
    length += contentLength;
}

unsigned int request::getRequestId()
{
    return requestId;
}

void request::setType(unsigned char t)
{
    type = t;
}

