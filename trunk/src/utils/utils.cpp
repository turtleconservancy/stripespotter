#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#ifdef __WXGTK__
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#ifdef __WXMAC__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#endif

/* Returns total sys memory in kb */
int getSystemMemory()	{
#ifdef __WXGTK__
	FILE *fp;
	char buf[100];
	char *p; 

	if((fp = fopen("/proc/meminfo", "r")))	{
		while(fgets(buf, 99, fp))	{
			if(strncmp(buf, "MemTotal:", 9) == 0)	{
				p = strchr(buf, ':')+1;
				while(*p == ' ' || *p == '\t')
					p++;
                return atoi(p);
			}
		}
		fclose(fp);
	} else {
		fprintf(stderr, "ERROR cannot get memory statistics\n");
		exit(255);
	}
#elif defined(__WXMAC__)
	int64_t physical_memory;
	int mib[] = { CTL_HW,  HW_MEMSIZE };
	size_t length = sizeof(int64_t);
	sysctl(mib, 2, &physical_memory, &length, NULL, 0);
	return physical_memory;
#else
#	warning No code for getting memory stats from OS
#endif
    return 0;
}

/* Returns free sys memory in kb */
int getSystemFreeMemory()	{
#ifdef __WXGTK__
	FILE *fp;
	char buf[100];
	char *p;

	if((fp = fopen("/proc/meminfo", "r")))	{
		while(fgets(buf, 99, fp))	{
			if(strncmp(buf, "MemFree:", 8) == 0)	{
				p = strchr(buf, ':')+1;
				while(*p == ' ' || *p == '\t')
					p++;
                return atoi(p);
			}
		}
		fclose(fp);
	} else {
		fprintf(stderr, "ERROR cannot get memory statistics\n");
		exit(255);
	} 
#elif defined(__WXMAC__)
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics_data_t vm_stats;

	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
	    KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
	                                    (host_info_t)&vm_stats, &count))
	{
	    return (int64_t)vm_stats.free_count * (int64_t)page_size;
	} else {
		fprintf(stderr, "ERROR cannot get memory statistics.");
		exit(255);
	}
#else
#	warning No code for getting memory stats from OS
#endif
    return 0;
}


int getMemoryPeak()	{
	int musage = 0;
#ifdef __WXGTK__
	FILE *fp;
	char buf[100];
	char *p;

	sprintf(buf, "/proc/%d/status", getpid());
	if((fp = fopen(buf, "r")))	{
		while(fgets(buf, 99, fp))	{
			if(strncmp(buf, "VmPeak:", 7) == 0)	{
				p = strchr(buf, ':')+1;
				while(*p == ' ' || *p == '\t')
					p++;
				musage = atoi(p);
				break;
			}
		}
		fclose(fp);
	}
#endif
	return musage;
}

int getMemoryCurrent()	{
	int musage = 0;

#ifdef __WXGTK__
	FILE *fp;
	char buf[100];
	char *p;

	sprintf(buf, "/proc/%d/status", getpid());
	if((fp = fopen(buf, "r")))	{
		while(fgets(buf, 99, fp))	{
			if(strncmp(buf, "VmSize:", 7) == 0)	{
				p = strchr(buf, ':')+1;
				while(*p == ' ' || *p == '\t')
					p++;
				musage = atoi(p);
				break;
			}
		}
		fclose(fp);
	}
#endif
	return musage;
}


/*
 * USERSPACE CPU USAGE (LINUX ONLY)
 */
static double user_clock_start = 0;
static double kernel_clock_start = 0;

void startUserClock()	{
#ifdef __WXGTK__
	struct rusage rbf;
	getrusage(RUSAGE_SELF, &rbf);
	user_clock_start = ((double)rbf.ru_utime.tv_sec * 1e6) +  rbf.ru_utime.tv_usec;
#endif
}
double stopUserClock()	{
#ifdef __WXGTK__
	struct rusage rbf;
	double clock_stop;
	getrusage(RUSAGE_SELF, &rbf);
	clock_stop = ((double)rbf.ru_utime.tv_sec*1e6) + rbf.ru_utime.tv_usec;
	return  (clock_stop-user_clock_start)/1e6;
#else
	return 0;
#endif
}
void startKernelClock()	{
#ifdef __WXGTK__
	struct rusage rbf;
	getrusage(RUSAGE_SELF, &rbf);
	kernel_clock_start = ((double)rbf.ru_stime.tv_sec * 1e6) +  rbf.ru_stime.tv_usec;
#endif
}
double stopKernelClock()	{
#ifdef __WXGTK__
	struct rusage rbf;
	double clock_stop;
	getrusage(RUSAGE_SELF, &rbf);
	clock_stop = ((double)rbf.ru_stime.tv_sec*1e6) + rbf.ru_stime.tv_usec;
	return  (clock_stop-kernel_clock_start)/1e6;
#else
	return 0;
#endif
}
void startClocks()	{
	startKernelClock();
	startUserClock();
}
double stopClocks()	{
	return stopUserClock() + stopKernelClock();
}

/* 
 * COMMAND LINE PARSING
 */
static void showUsage(CMDLINE_PARAMETERS *parameters, char *argv0)	{
	CMDLINE_PARAMETERS *p;

	printf("Usage:\n  %s <options>\n\nOptions:\n", argv0);
	for(p = parameters; p->option; p++)	{
		printf(" %s ", p->option);
		switch(p->arg_type) {
			case CMDLINE_INTEGER: printf("<integer in [%d,%d]>", p->min, p->max); break;
			case CMDLINE_DOUBLE:  printf("<real>              "); break;
			case CMDLINE_STRING:  printf("<string>            "); break;
			case CMDLINE_HELP:
			case CMDLINE_BINARY:  printf("                    "); break;
		}
		printf("(required: %s) %s\n", p->required?"yes":"no", p->description);
	}
}

int ParseCommandLine(int argc, char **argv, CMDLINE_PARAMETERS *parameters)	{
	int a, t;
	double d;
	CMDLINE_PARAMETERS *p;

	for(a=1; a<argc; a++)	{
		/* find argv[a] in parameters */
		for(p=parameters; p->option; p++)
			if(strcasecmp(p->option, argv[a])==0)
				break;
		if(!p->option)	{
			fprintf(stderr, "%s: Unknown parameter \"%s\".\n", argv[0], argv[a]);
			return 0;
		}

		/* type CMDLINE_BINARY has no argument */
		if(p->arg_type == CMDLINE_BINARY || p->arg_type == CMDLINE_HELP)	{
			if(p->arg_type == CMDLINE_BINARY)
				*((int *)p->where_to) = 1;
			else	{
				showUsage(parameters, argv[0]);
				return 0;
			}
		}
		else {
			/* the others do */
			if(++a == argc)	{
				fprintf(stderr, "%s: Parameter \"%s\" requires an argument.\n", argv[0], argv[--a]);
				return 0;
			}
			switch(p->arg_type)	{
				case CMDLINE_INTEGER:
					t = atoi(argv[a]);
					if(t < p->min || (t > p->max && p->max >= p->min))	{
						fprintf(stderr, "%s: Parameter \"%s\" is out of range.\n", argv[0], argv[--a]);
						return 0;
					}
					*((int *)p->where_to) = t;
					break;

				case CMDLINE_STRING:
					if((int)strlen(argv[a]) < p->min || (int)strlen(argv[a]) > p->max)	{
						fprintf(stderr, "%s: Argument for parameter \"%s\" is either too long or too short.\n", argv[0], argv[--a]);
						return 0;
					}
					strncpy((char *)p->where_to, argv[a], strlen(argv[a]));
					*((char *)p->where_to+strlen(argv[a])) = 0;
					break;

				case CMDLINE_DOUBLE:
					d = strtod(argv[a], NULL);
					if(d < (double)p->min || d > (double)p->max)	{
						fprintf(stderr, "%s: Argument for parameter \"%s\" is out of range.\n", argv[0], argv[--a]);
						return 0;
					}
					*((double *)p->where_to) = d;
					break;

				default:
					fprintf(stderr, "%s: Program error.\n", argv[0]);
					break;
			}

			/* made it through this option successfully */
			if(p->required)
				p->required = 0;
		}		
	}

	for(p=parameters; p->option; p++)	
		if(p->required)	{
			fprintf(stderr, "%s: Option \"%s\" not specified: %s\n", argv[0], p->option, p->description);
			return 0;
		}

	return 1;
}



/*
 * TEXT PROCESSING
 */
inline void chomp(char *s)	{
	s--;
	while(*(++s))
		if(*s=='\n')	{
			*s = 0;
			return;
		}
}



