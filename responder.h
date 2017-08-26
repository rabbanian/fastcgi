#ifndef FASTCGI_RESPONDER_H
#define FASTCGI_RESPONDER_H


namespace fastcgi {
    class responder {
    private:
        unsigned char *params = nullptr;
        unsigned int paramsLength = 0;
        unsigned char *stdIn = nullptr;
        unsigned int stdInLength = 0;
        unsigned char *data = nullptr;
        unsigned int dataLength = 0;
        unsigned char *headerBuffer = nullptr;
        unsigned int headerLength = 0;
        unsigned char *bodyBuffer = nullptr;
        unsigned int bodyLength = 0;
    protected:
        void writeHeader(const void * buffer, unsigned int bufferSize);
        void writeBody(const void * buffer, unsigned int bufferSize);
    public:
        void setParams(unsigned char *buff, unsigned int size);
        void setStdIn(unsigned char *buff, unsigned int size);
        void setData(unsigned char *buff, unsigned int size);
        unsigned char * getBuffer(unsigned int &size);
        virtual void respond() = 0;
    };
}

#endif //FASTCGI_RESPONDER_H
