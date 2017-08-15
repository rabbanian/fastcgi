#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include "constants.h"
#include "server.h"
#include "connection.h"

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
        std::thread (&server::newConnection, this, reqfd).detach();
    }
}

void server::newConnection(int fd)
{
    connection * temp = new connection(fd, handler);
    temp->manage();
    delete temp;
}