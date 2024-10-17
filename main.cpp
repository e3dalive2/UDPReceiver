#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define ENET_IMPLEMENTATION
#include <enet.h>
#include <stdio.h>

#include <spdlog/spdlog.h>

const int kMaxClients = 4000;//enet limitations

void enet_process()
{
	ENetAddress address;
	ENetHost* server;

	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */

	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 27010;

	server = enet_host_create(&address /* the address to bind the server host to */,
		kMaxClients      /* allow up to 32 clients and/or outgoing connections */,
		1      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		spdlog::error("An error occurred while trying to create an ENet server host");
		exit(EXIT_FAILURE);
	}
}

int main()
{
	spdlog::info("starting");

	if (enet_initialize() != 0) {
		spdlog::error("An error occurred while initializing ENet");
		return 1;
	}

	enet_process();

	spdlog::info("closing");
	enet_deinitialize();

	return 8;
}