#include "incl.hpp"


void test() {
    Webserver ws;
    ws.addServer("8080");
    ws.addServer("8081", "new server");
    ws.setPFD();
    ws.serverForever();
}

int main(int argc, char* argv[], char* envp[]) {
    test();
    getchar();
    return 0;
}