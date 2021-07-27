#include "Webserver.hpp"

int main(int argc, char* argv[], char* envp[]) {
    {
        Webserver ws;
        ws.describe();
    }
    {
        Webserver ws(3);
        ws.describe();
        ws.addServer("8080");
        ws.addServer("3490");
        ws.describe();
    }
    return 0;
}