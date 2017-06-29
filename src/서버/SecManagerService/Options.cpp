#include "StdAfx.h"
#include "Options.h"
#include "SecWinService.h"
#include "SecServer.h"

#include "service/CubeNTService.h"
#include "service/CubeNTServiceManager.h"

//#include <string.h>

#define DEFAULT_SERVICE_INIT_STARTUP SERVICE_DEMAND_START

int     opterr = 1,             /* if error message should be printed */
		optind = 1,             /* index into parent argv vector */
		optopt,                 /* character checked for validity */
		optreset;               /* reset getopt */
char    *optarg;                /* argument associated with option */

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

/*
	*	getopt --
	*      Parse argc/argv argument vector.
*/
int  getopt(int nargc, char * const nargv[], const char *ostr)
{
  static char *place = EMSG;              /* option letter processing */
  const char *oli;                        /* option letter list index */

  if (optreset || !*place) {              /* update scanning pointer */
    optreset = 0;
    if (optind >= nargc || *(place = nargv[optind]) != '-') {
      place = EMSG;
      return (-1);
    }
    if (place[1] && *++place == '-') {      /* found "--" */
      ++optind;
      place = EMSG;
      return (-1);
    }
  }                                       /* option letter okay? */
  if ((optopt = (int)*place++) == (int)':' ||
    !(oli = strchr(ostr, optopt))) {
      /*
      * if the user didn't specify '-' as an option,
      * assume it means -1.
      */
      if (optopt == (int)'-')
        return (-1);
      if (!*place)
        ++optind;
      if (opterr && *ostr != ':')
        (void)printf("illegal option -- %c\n", optopt);
      return (BADCH);
  }
  if (*++oli != ':') {                    /* don't need argument */
    optarg = NULL;
    if (!*place)
      ++optind;
  }
  else {                                  /* need an argument */
    if (*place)                     /* no white space */
      optarg = place;
    else if (nargc <= ++optind) {   /* no arg */
      place = EMSG;
      if (*ostr == ':')
        return (BADARG);
      if (opterr)
        (void)printf("option requires an argument -- %c\n", optopt);
      return (BADCH);
    }
    else                            /* white space */
      optarg = nargv[optind];
    place = EMSG;
    ++optind;
  }
  return (optopt);                        /* dump back option letter */
}



Options::Options(void)
	: opt_install (0),
		opt_remove (0),
		opt_start (0),
		opt_kill (0),
		opt_pause (0),
		opt_debug (0),
		opt_restart(0),
		opt_startup (0)
{
}


Options::~Options(void)
{
}

Options*	Options::GetInstance()
{
	static Options _Instance;
	return &_Instance;
}

int		Options::run (int argc, char *argv[])
{
	parse_args(argc, argv);
	if(opt_install && !opt_remove)
	{
		// 서비스 등록
 		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
 		sm.Install();
		return 0;
	}

	if(opt_remove && !opt_install)
	{
		// 서비스 제거
 		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
 		sm.Remove();		
		return 0;
	}


	if(opt_start)
	{
		printf("[SVC] OPT SERVICE START\n");
		// 서비스 시작
		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
		sm.Start();
		return 0;
	}

	if(opt_kill)
	{
		printf("[SVC] OPT SERVICE KILL\n");
		// 서비스 중지
		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
		sm.Stop();
		return 0;

	}

	if(opt_pause)
	{
		printf("[SVC] OPT SERVICE PAUSE\n");
		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
		sm.Pause();
		return 0;
	}

	if(opt_debug)
	{
		printf("[SVC] SEC Server Program Start\n");
 		int iRet;
 		if((iRet = SECSERVER->ServiceInit(0, "")) == 0)
 			SECSERVER->ServiceRun();
 		else
 		{
 			printf("[SVC] SEC Server Program Init Fail(ret:%d)\n", iRet);
 			Sleep(30);
 		}
 
 		SECSERVER->ServiceDestroy();
 		printf("[SVC] DSV Server Program Stop\n");
		return 0;
	}

	if(opt_restart)
	{
		printf("[SVC] DSV Server Program ReStart\n");
		CubeNTServiceManager sm(SERVICE_NAME, SERVICE_SHOW_NAME);
		sm.Stop();
		printf("stop succ");
		Sleep(5000);
		sm.Start();
		printf("start succ");
	}

	
	return -1;
}
void	Options::parse_args (int argc,	char *argv[])
{
	int c = getopt(argc, argv, "irskpdn");

	while (c != -1)
	{
		switch (c)
		{
			case 'i':
				opt_install = 1;
				break;
			case 'r':
				opt_remove = 1;
				break;
			case 's':
				opt_start = 1;
				break;
			case 'k':
				opt_kill = 1;
				break;
			case 't':
				opt_pause = 1;
				break;
			case 'd':
				opt_debug = 1;
				break;
			case 'n':
				opt_restart = 1;
				break;
			default:
				// -i can also be given without a value - if so, it defaults
				// to defined value.
				this->print_usage_and_die ();
				break;
		}
		c = getopt(argc, argv, "irskpdn");
	}
}

void	Options::print_usage_and_die (void)
{
		printf("Usage:");
		printf(" -i -r -s -k -t -d\n");
		printf("  -i: Install this program as an NT service, with specified startup\n");
		printf("  -r: Remove this program from the Service Manager\n");
		printf("  -s: Start the service\n");
		printf("  -k: Kill the service\n");
		printf("  -p: Set startup for an existing service\n");
		printf("  -d: Debug; run as a regular application\n");
		exit(1);
}