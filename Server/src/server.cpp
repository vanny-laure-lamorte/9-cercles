#include "LPTF_Server.h"
#include <iostream>
using namespace std;

int main() {
    try {
        LPTF_Server server("127.0.0.1", 12345);
        server.run();
    } catch (const exception& e) {
        cerr << "Error : " << e.what() << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}