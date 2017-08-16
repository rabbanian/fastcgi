#ifndef FASTCGI_SERVER_H
#define FASTCGI_SERVER_H

namespace fastcgi {
    class responder;
    class request;
    class server {
    private:
        int sockfd;

        request *reqList;

        responder * handler;

        void connection(int fd);

        int read(int fd, unsigned char *buffer, int bufferSize);

    public:
        server(responder * res, int port = 8080);

        void accept();
    };
}

#endif //FASTCGI_SERVER_H
