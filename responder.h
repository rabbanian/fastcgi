#ifndef FASTCGI_RESPONDER_H
#define FASTCGI_RESPONDER_H


namespace fastcgi {
    class responder {
    protected:
        void writeHeader(unsigned char * buffer, unsigned int bufferSize);
        void writeBody(unsigned char * buffer, unsigned int bufferSize);
    public:
        virtual void respond() = 0;
    };
}

#endif //FASTCGI_RESPONDER_H
