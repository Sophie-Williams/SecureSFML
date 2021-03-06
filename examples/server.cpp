#include "SecureSFML/SecureNetwork.hpp"
#include <iostream>
#include <list>

using namespace std;
using namespace sf;
using namespace ssf;

int main(int argc, char* argv[]) {
    SecureTcpListener listener;

    SocketSelector selector;
    listener.listen(1234);
    selector.add(listener);
    // Create a list to store the future clients
    list<SecureTcpSocket*> clients;

    // Endless loop that waits for new connections
    while (true)
      {
        // Make the selector wait for data on any socket
        if (selector.wait())
          {
            // Test the listener
            if (selector.isReady(listener))
              {
                // The listener is ready: there is a pending connection
                // Use an AES-CBC-256 cipher for encrypting the data
                SecureTcpSocket* client = new SecureTcpSocket(new AESCipher(32));
                if (listener.accept(*client) == Socket::Done)
                  {
                    // Add the new client to the clients list
                    clients.push_back(client);

                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);
                  }
              }
            else
              {
                // The listener socket is not ready, test all other sockets (the clients)
                for (list<SecureTcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                  {
                    SecureTcpSocket& client = **it;
                    if (selector.isReady(client))
                      {
                        // The client has sent some data, we can receive it
                        SecurePacket packet = client.getNewSecurePacket();
                        if (client.receive(packet) == Socket::Done)
                          {
                            string s;
                            packet >> s;
                            cout << s << endl;
                          }
                      }
                  }
              }
          }
      }

    return 0;
}
