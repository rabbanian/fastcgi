#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <thread>

#include "constants.h"
#include "server.h"
#include "request.h"
#include "connection.h"

#include <iostream>

using namespace fastcgi;

server::server(responder * res, int port /* = 8080 */) : handler(res)
{
    reqList = nullptr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockadd;
    memset(&sockadd, 0, sizeof(&sockadd));
    sockadd.sin_family = AF_INET;
    sockadd.sin_port = htons(port);
    sockadd.sin_addr.s_addr = htonl(INADDR_ANY);

    int test = bind(sockfd, (struct sockaddr *) &sockadd, sizeof(sockadd));
    std::cout << "bind says : " << test << std::endl;

    conns = new connection*[FCGI_MAX_CONNS];
    fds = new pollfd[FCGI_MAX_CONNS + 1];

    for (int i = 0; i < FCGI_MAX_CONNS; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[FCGI_MAX_CONNS].fd = sockfd;
    fds[FCGI_MAX_CONNS].events = POLLIN;
}

void server::accept()
{
    listen(sockfd, FCGI_MAX_CONNS * 2);
    while (poll(fds, FCGI_MAX_CONNS + 1, -1) > 0) {
        if (fds[FCGI_MAX_CONNS].revents & POLLIN) {
            int reqfd = ::accept(sockfd, NULL, NULL);
            fcntl(reqfd, F_SETFL, fcntl(reqfd, F_GETFL) | O_NONBLOCK);
            newConn(reqfd);
        }
        if (fds[FCGI_MAX_CONNS].revents & (POLLHUP & POLLERR & POLLNVAL)) {
            shutdown(sockfd, SHUT_RDWR);
            ::close(sockfd);
            break;
        }
        for (int i = 0; i < FCGI_MAX_CONNS; i++) {
            if (fds[i].revents & (POLLHUP & POLLERR & POLLNVAL)) {
                //We have to kill the thread associated to i
                delete conns[i];
                fds[i].fd = -1;
                fds[i].events = 0;
            }
            if (fds[i].revents & POLLIN) {
                //tell thread i to do read
                conns[i]->readFd();
            }
            if (fds[i].revents & POLLOUT) {
                //tell thread i that data is available
                conns[i]->writeFd();
            }
        }
    }
}

void server::newConn(int fd)
{
    int i;
    for (i = 0; i < FCGI_MAX_CONNS; i++)
        if (fds[i].fd == -1)
            break;
    if (i < FCGI_MAX_CONNS) {
        fds[i].fd = fd;
        fds[i].events = POLLIN | POLLOUT;
        std::cout << "NEW CONNECTION!" << std::endl << "---------------------" << std::endl;
        connection * temp = new connection(&fds[i]);
        conns[i] = temp;
        std::thread t1(&server::manage, this, temp);
        t1.detach();
    } else {
        //Maximum simultaneous connections reached!
        shutdown(fd, SHUT_RDWR);
        ::close(fd);
    }
}

void server::manage(connection * io)
{
    request * temp = nullptr;
    unsigned char version, type, paddingLength;
    unsigned int requestId, contentLength;
    unsigned char * fcgi_header = new unsigned char[FCGI_HEADER_LEN];
    unsigned char * fcgi_content_data = nullptr;
    unsigned char * paddingData = nullptr;

    while (true) {
        io->read(fcgi_header, FCGI_HEADER_LEN);
        version = fcgi_header[0];
        type = fcgi_header[1];
        requestId = (fcgi_header[2] << 8) + fcgi_header[3];
        contentLength = (fcgi_header[4] << 8) + fcgi_header[5];
        paddingLength = fcgi_header[6];
        std::cout << "---version : " << (int) version << std::endl;
        std::cout << "---type : " << (int) type << std::endl;
        std::cout << "---requestId : " << (int) requestId << std::endl;
        std::cout << "---contentLength : " << (int) contentLength << std::endl;
        std::cout << "---paddingLength : " << (int) paddingLength << std::endl;
        fcgi_content_data = new unsigned char[contentLength];
        io->read(fcgi_content_data, contentLength);
        paddingData = new unsigned char[paddingLength];
        io->read(paddingData, paddingLength);
        delete [] paddingData;

        if (reqList == nullptr || reqList->getRequestId() != requestId) {
            temp = new request(io, type, requestId, handler);
        } else {
            temp = reqList;
            temp->setType(type);
        }
        if (temp->control(fcgi_content_data, contentLength)) break;
    }
    //should I delete fcgi_content_data or not?
    delete temp;
    delete [] fcgi_header;
    delete io;
}

/*
int server::read(int fd, unsigned char * buffer, int bufferSize)
{
    int bytesRead = 0, result = 0;
    while ((result = ::read(fd, buffer + bytesRead, bufferSize - bytesRead)) > 0) {
        bytesRead += result;
    }
    return 0;
}
 */