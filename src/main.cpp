#include "client.h"
#include "commandline_argument.h"
#include <cstdio>


int main(int argc, char **argv) {
	ArgumentRequests a;
	if (process_argument(argc, argv, a) != 0) {
		return 1;
	}
	Client client;
	client.initialize({});
	client.main_loop();
	return 0;
}
