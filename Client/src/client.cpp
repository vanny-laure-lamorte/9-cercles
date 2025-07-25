#include "LPTF_Client.h"
using namespace std;

int main() {
    string ip = "127.0.0.1";
    int port = 12345;

    try {
        LPTF_Client client(ip, port);
        client.run();
    } catch (const exception& e) {
        cerr << "Error : " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
