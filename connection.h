#ifndef FASTCGI_CONNECTION_H
#define FASTCGI_CONNECTION_H

namespace fastcgi {
    class responder;
    class request;
    class connection {
    private:
        int sockfd;
        request *reqList;
        responder *handler;

    public:
        connection(int fd, responder *res);

        ~connection();

        void manage();

        int write(const unsigned char *buffer, int bufferSize);

        int read(unsigned char *buffer, int bufferSize);
    };
}

#endif //FASTCGI_CONNECTION_H
