#include "commandline_argument.h"
#include <cstdio>


int main(int argc, char **argv) {
	ArgumentSpecifying a;
	if (process_argument(argc, argv, a) != 0) {
		return 1;
	}
	
	printf("%d\n", (int)a.disable_mainmenu);
	return 0;
}
