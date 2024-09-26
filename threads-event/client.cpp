#include <iostream>
#include <cstring>      // For memset
#include <unistd.h>     // For close
#include <arpa/inet.h>  // For socket, bind, listen, accept, inet_ntoa
#include <ctime>        // For time, localtime, strftime

using namespace std;

int main() {
  /* 1. Set up client socket */
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    cerr << "bad client socket" << endl;
    return 1;
  }

  /* 2. Configure server address */
  sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);

  // Accept connections from any IP address
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // Only allow the server to accept connections from localhost
  // inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

  /* 3. Connect to the server */
  int rc = connect(clientSocket, (sockaddr*) &serverAddress, sizeof(serverAddress));
  if (rc == -1) {
    cerr << "Failed to connect to server socket" << endl;
    return 1;
  }

  /* 4. Instatiate buffer, try to read from server */
  char buffer[100];
  memset(buffer, 0, sizeof(buffer));

  ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

  if (bytesReceived > 0) {
    cout << "Server time: " << buffer << endl;
  } else {
    cerr << "No data from server" << endl;
  }
  close(clientSocket);


  return 0;
}
