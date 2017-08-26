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
