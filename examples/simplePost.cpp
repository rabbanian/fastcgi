#include "../server.h"
#include "../responder.h"
#include <string.h>

class helloWorld : public fastcgi::responder
{
public:
    void respond()
    {
        const char *header =
                "Content-Type: text/html\n"
                "\n";
        writeBody(header, 25);
        const char *postBody =
                "<!DOCTYPE html>\n"
                        "<html>\n"
                        "\t<head>\n"
                        "\t\t<title>Wellcome!</title>\n"
                        "\t</head>\n"
                        "\t<body>\n"
                        "\t\t<h1>You Are POSTing!</h1>\n"
                        "\t\t<p>your POST data:</p>\n"
                        "\t\t<p>";
        const char *body =
                "<!DOCTYPE html>\n"
                        "<html>\n"
                        "\t<head>\n"
                        "\t\t<title>Oh No!</title>\n"
                        "\t</head>\n"
                        "\t<body>\n"
                        "\t\t<p>Please try to POST some data!</p>\n"
                        "\t</body>\n"
                        "</html>";
        unsigned int length;
        const unsigned char *method = getHeader("REQUEST_METHOD", length);
        if (method != nullptr && strncmp((const char *)method, "POST", length) == 0) {
            writeBody(postBody, strlen(postBody));
            unsigned int postLength;
            unsigned char * postData = getStdIn(postLength);
            writeBody(postData, postLength);
            writeBody("\t\t</p>\t</body>\n</html>", 22);
        } else {
            writeBody(body, strlen(body));
        }
    };
};

int main(int argc, char * argv[])
{
    fastcgi::server app(new helloWorld);
    app.accept();
    return 0;
}
