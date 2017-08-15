#ifndef FASTCGI_SERVER_H
#define FASTCGI_SERVER_H

namespace fastcgi {
    class responder;
    class server {
    private:
        int sockfd;

        responder * handler;

        void newConnection(int fd);

    public:
        server(responder * res, int port = 8080);

        void accept();
    };
}

#endif //FASTCGI_SERVER_H
