#ifndef FASTCGI_SERVER_H
#define FASTCGI_SERVER_H

namespace fastcgi {
    class responder;
    class request;
    class connection;

    class server {
    private:
        int sockfd;

        request *reqList;

        responder * handler;

        struct pollfd * fds;

        connection ** conns;

        void manage(connection * io);

        void newConn(int fd);

    public:
        server(responder * res, int port = 8080);

        void accept();
    };
}

#endif //FASTCGI_SERVER_H
