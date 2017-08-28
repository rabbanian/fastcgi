# Fastcgi
A simple C++ implementation of [fastcgi protocol](http://www.mit.edu/%7Eyandros/doc/specs/fcgi-spec.html "FastCGI Specification") 
which enables you to write your web application logic in C++.
# Usage
Implementing a fastcgi application is easy, one should first create
a class that inherits responder, then server class that accepts
incoming requests.
```c++
#include "../server.h"
#include "../responder.h"
#include <string.h>

class helloWorld : public fastcgi::responder
{
public:
    void respond()
    {
        const char *test =
                "Content-Type: text/html\n"
                        "Content-Length: 115\n"
                        "\n"
                        "<!DOCTYPE html>\n"
                        "<html>\n"
                        "\t<head>\n"
                        "\t\t<title>Wellcome!</title>\n"
                        "\t</head>\n"
                        "\t<body>\n"
                        "\t\t<h1>Hello World!</h1>\n"
                        "\t</body>\n"
                        "</html>";
        writeBody(test, strlen(test));
    };
};

int main(int argc, char * argv[])
{
    fastcgi::server app(new helloWorld);
    app.accept();
    return 0;
}
```
You must run the application yourself and point the web sever to which
port server is listening (default is 8080).

Simple Nginx configuration that should work with fastcgi:
```nginx
server {
    listen 80;
    server_name localhost;
    
    location / {
        include fastcgi_params;
        fastcgi_pass localhost:8080;
    }
}
```
# Building 
The build system is CMake and there should be no problem 
building on Linux. MS Windows is not supported yet.

First clone:
```bash
$ git clone https://github.com/rabbanian/fastcgi.git
```
Then make a build directory:
```bash
$ mkdir fastcgi/build/
$ cd fastcgi/build/
```
Run CMake:
```bash
$ cmake ..
```
And Make:
```bash
$ make
```