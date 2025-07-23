#include "LPTF_Client.h"
using namespace std;

int main() {
    string ip;
    int port = 12345;

    cout << endl << "Enter IP address : ";
    getline(cin, ip);
    try {
        LPTF_Client client(ip, port);
        client.run();
    } catch (const exception& e) {
        cerr << "Error : " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
