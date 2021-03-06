#include "SecureSFML/SecureNetwork.hpp"
#include <iostream>

using namespace std;
using namespace ssf;

int main(int argc, char* argv[]) {

    SecureTcpSocket s;

    s.connect("127.0.0.1", 1234);

    SecurePacket data = s.getNewSecurePacket();

    while(1) {
      string toSend;
      cin >> toSend;
      SecurePacket data = s.getNewSecurePacket();
      data << toSend;
      s.send(data);
    }

    return 0;
}
