#ifndef FASTCGI_CONNECTION_H
#define FASTCGI_CONNECTION_H
#include <condition_variable>
#include <mutex>
#include <poll.h>
namespace fastcgi {
    class connection {
    private:
        pollfd * fd;
        bool shouldClose = false;
        std::condition_variable cv;
        bool writeDone = false;
        std::condition_variable writeCv;
        unsigned char * inputBuffer = nullptr;
        unsigned int inputBuffSize;
        unsigned int inputBytes;
        std::mutex readLock;
        unsigned char * outputBuffer = nullptr;
        unsigned int outputBuffSize;
        unsigned int outputBytes;
        std::mutex writeLock;

        void extendBuffer(unsigned char ** buffer, unsigned int &bufferSize);

    public:
        connection(pollfd *fd);
        ~connection();
        void readFd();
        void writeFd();
        int read(unsigned char * buffer, unsigned int bufferSize);
        int write(unsigned char * buffer, unsigned int bufferSize);
    };
}


#endif //FASTCGI_CONNECTION_H
