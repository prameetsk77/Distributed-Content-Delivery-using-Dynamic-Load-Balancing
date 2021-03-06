#ifndef NFV_H
#define NFV_H

#include <string>
#include <list>
#include <thread>
#include <Sockets/tcp_socket.h>
#include <Sockets/udp_socket.h>
#include <NFV/load_distributer.h>

using namespace std;

const int no_of_servers = 3;

class NFV{

public: 
	static const string NFV_IP;//  = "192.168.1.7";
	static const string PortToServer; // = "10000";
	static const string PortToClient; // = "11000";
	static const string server_ip[3]; // = {"10.10.2.2","10.10.3.1","10.10.4.2"};
	static const string server_port; //="10000";

	static const string Client_IP;//  = "192.168.1.7";
	static const string Client_NFV_IP;
	static const string ClientPort;

	/*NFV(string server_ip_array[3], string server_port){
		for(int i = 0; i < 3; i++){
			server_ip[i] = server_ip_array[i];
		}
		dest_port = server_port;
	}*/

	bool receiveFromClient(TCP_Socket *client_socket, char *packet, int &bytes);

	bool broadcastToServer(UDP_Socket &socket, int packet_len, int& bytes, char *packet);

	bool receiveLoadFromServer(UDP_Socket &udp_socket, Server servers[no_of_servers]);

	void getContentRequest(Server s[no_of_servers], char *url, int urlLength, list<thread> &threadList, TCP_Socket* client, int filesize);

private:
	//void NFV_Server_TCPSend(TCP_Socket *soc, content_packet cpack);

};

const string NFV::NFV_IP = "10.10.2.1";
const string NFV::PortToServer = "10000";
const string NFV::server_ip[3] = {"10.10.2.2","10.10.3.1","10.10.4.2"};
const string NFV::server_port="10000";

const string NFV::Client_NFV_IP = "10.10.1.1";
const string NFV::PortToClient = "11000";
const string NFV::Client_IP = "10.10.1.2";
const string NFV::ClientPort = "11000";

#endif