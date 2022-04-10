#include "commandline_argument.h"

#include "argument_utils.h"
#include <cstdlib>


int process_argument(int argc, char **argv, ArgumentRequests &output) {
	output.disable_mainmenu = false;

	ArgumentProcessor ap;

	Argument help, disable_mainmenu;

	help.add_name("-h").add_name("--help").add_name("--usage");
	help.set_argc(0);
	help.set_description("Display this help and exit.");
	help.set_act_func([&ap](char **argv) {
		ap.output_help({argv[0], ": Flappybird.\n"});

		exit(0);
	});

	disable_mainmenu.add_name("--disable-mainmenu");
	disable_mainmenu.set_argc(0);
	disable_mainmenu.set_called_limit(1);
	disable_mainmenu.set_description("Disable mainmenu. Exit the game after the game overs.");
	disable_mainmenu.set_act_func([&output](char **) {
		output.disable_mainmenu = true;
	});

	ap.register_argument(help);
	ap.register_argument(disable_mainmenu);

	return ap.process(argc, argv) ? 0 : 1;
}
