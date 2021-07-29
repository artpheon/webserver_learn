#include "Webserver.hpp"

int main(int argc, char* argv[], char* envp[]) {
    {
        Webserver ws(3);
        ws.addServer("8080");
        ws.addServer("3490");
        ws.describe();
        ws.invokeServer();
    }
    return 0;
}