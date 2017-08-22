#ifndef FASTCGI_REQUEST_H
#define FASTCGI_REQUEST_H

namespace fastcgi {
    class responder;
    class connection;
    class request {
    private:
        connection * io;
        unsigned char type, flags;
        unsigned int requestId, role;
        unsigned char *contentData;
        unsigned int contentLength;
        bool shouldClose = false;

        responder * handler;

        void beginRequest();

        void abortRequest();

        void params();

        void stdIn();

        void data();

    public:
        request(connection * writer, unsigned char t, unsigned int reqId, responder * res);

        int control(unsigned char *buffer, unsigned int bufferSize);

        unsigned int getRequestId();

        void setType(unsigned char t);

    };
}

#endif //FASTCGI_REQUEST_H
