#include <unistd.h>
#include <sys/socket.h>

#include "connection.h"
#include "constants.h"
#include "request.h"

using namespace fastcgi;

connection::connection(int fd, responder * res) : sockfd(fd), handler(res)
{
    reqList = nullptr;
}

void connection::manage()
{
    request * temp;
    unsigned char version, type, paddingLength;
    unsigned int requestId, contentLength;
    unsigned char fcgi_header[FCGI_HEADER_LEN];
    unsigned char * fcgi_content_data;
    unsigned char * paddingData;

    while (true) {
        read(fcgi_header, FCGI_HEADER_LEN);
        version = fcgi_header[0];
        type = fcgi_header[1];
        requestId = (fcgi_header[2] << 8) + fcgi_header[3];
        contentLength = (fcgi_header[4] << 8) + fcgi_header[5];
        paddingLength = fcgi_header[6];
        fcgi_content_data = new unsigned char[contentLength];
        read(fcgi_content_data, contentLength);
        paddingData = new unsigned char[paddingLength];
        read(paddingData, paddingLength);
        delete paddingData;

        if (reqList == nullptr || reqList->getRequestId() != requestId) {
            temp = new request(sockfd, type, requestId, handler);
        } else {
            temp = reqList;
            temp->setType(type);
        }
        if (temp->control(fcgi_content_data, contentLength))
            break;
        //should I delete fcgi_content_data or not?
    }
    delete temp;
}

int connection::write(const unsigned char * buffer, int bufferSize)
{
    int bytesWrite = 0, result = 0;
    while ((result = ::write(sockfd, buffer + bytesWrite, bufferSize - bytesWrite)) > 0) {
        bytesWrite += result;
    }
    return 0;
}

int connection::read(unsigned char * buffer, int bufferSize)
{
    int bytesRead = 0, result = 0;
    while ((result = ::read(sockfd, buffer + bytesRead, bufferSize - bytesRead)) > 0) {
        bytesRead += result;
    }
    return 0;
}

connection::~connection()
{
    shutdown(sockfd, SHUT_RDWR);
    ::close(sockfd);
}
