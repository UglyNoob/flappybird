#ifndef __FLA_COMMANDLINE_ARGUMENT_H__
#define __FLA_COMMANDLINE_ARGUMENT_H__

struct ArgumentRequests {
	bool disable_mainmenu;
};

/*
 * Return: 0 for success, a non-zero integer for errors.
 */
int process_argument(int argc, char **argv, ArgumentRequests &);

#endif
