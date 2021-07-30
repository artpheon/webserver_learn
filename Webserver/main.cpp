#include "Webserver.hpp"

void test() {
    Webserver ws;
    ws.run();
}

int main(int argc, char* argv[], char* envp[]) {
    test();
    getchar();
    return 0;
}