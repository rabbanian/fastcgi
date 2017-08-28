#ifndef FASTCGI_REQUEST_H
#define FASTCGI_REQUEST_H

namespace fastcgi {
    class responder;
    class connection;
    class request {
    private:
        connection * io = nullptr;
        unsigned char type, flags;
        unsigned int requestId, role;
        unsigned char *contentData = nullptr;
        unsigned int contentLength;
        unsigned char *paramsData = nullptr;
        unsigned int paramsLength = 0;
        unsigned char *stdInData = nullptr;
        unsigned int stdInLength = 0;
        unsigned char *dataData = nullptr;
        unsigned int dataLength = 0;
        bool shouldClose = false;
        bool paramsOver = false;
        bool stdInOver = false;
        bool dataOver = false;

        responder * handler = nullptr;

        void beginRequest();

        void abortRequest();

        void streamRecord(bool &isOver, unsigned char **data, unsigned int &length);

    public:
        request(connection * writer, unsigned char t, unsigned int reqId, responder * res);

        ~request();

        int control(unsigned char *buffer, unsigned int bufferSize);

        unsigned int getRequestId();

        void setType(unsigned char t);

    };
}

#endif //FASTCGI_REQUEST_H
