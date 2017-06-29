#pragma once


class Options
{
public:
	Options(void);
	~Options(void);

	static Options*	GetInstance();
	int run (int argc, char *argv[]);

private:
	void parse_args (int argc,	char *argv[]);
	void print_usage_and_die (void);

private:
	char progname[128];

	int opt_install;
	int opt_remove;
	int opt_start;
	int opt_kill;
	int opt_pause;
	int opt_debug;
	int opt_restart;

	int opt_startup;
};

