#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include "constants.h"
#include "server.h"
#include "request.h"

using namespace fastcgi;

server::server(responder * res, int port /* = 8080 */) : handler(res)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockadd;
    memset(&sockadd, 0, sizeof(&sockadd));
    sockadd.sin_family = AF_INET;
    sockadd.sin_port = htons(port);
    sockadd.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *) &sockadd, sizeof(sockadd));
}

void server::accept()
{
    listen(sockfd, FCGI_MAX_CONNS);
    while (true) {
        int reqfd = ::accept(sockfd, NULL, NULL);
        std::thread (&server::connection, this, reqfd).detach();
    }
}

void server::connection(int fd)
{
    request * temp;
    unsigned char version, type, paddingLength;
    unsigned int requestId, contentLength;
    unsigned char fcgi_header[FCGI_HEADER_LEN];
    unsigned char * fcgi_content_data;
    unsigned char * paddingData;

    while (true) {
        read(fd, fcgi_header, FCGI_HEADER_LEN);
        version = fcgi_header[0];
        type = fcgi_header[1];
        requestId = (fcgi_header[2] << 8) + fcgi_header[3];
        contentLength = (fcgi_header[4] << 8) + fcgi_header[5];
        paddingLength = fcgi_header[6];
        fcgi_content_data = new unsigned char[contentLength];
        read(fd, fcgi_content_data, contentLength);
        paddingData = new unsigned char[paddingLength];
        read(fd, paddingData, paddingLength);
        delete paddingData;

        if (reqList == nullptr || reqList->getRequestId() != requestId) {
            temp = new request(fd, type, requestId, handler);
        } else {
            temp = reqList;
            temp->setType(type);
        }
        if (temp->control(fcgi_content_data, contentLength))
            break;
        //should I delete fcgi_content_data or not?
    }
    delete temp;
    shutdown(fd, SHUT_RDWR);
    ::close(fd);
}

int server::read(int fd, unsigned char * buffer, int bufferSize)
{
    int bytesRead = 0, result = 0;
    while ((result = ::read(fd, buffer + bytesRead, bufferSize - bytesRead)) > 0) {
        bytesRead += result;
    }
    return 0;
}