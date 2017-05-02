#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

//#include <sys/resource.h>

#include "DB.h"
#include "Config.h"
#include "DAQ.h"

// Return pid of locking process or 0 if no other DAQ is running
pid_t create_lock()
{
  FILE* lckf;
  pid_t pid;

  // Check if lock file exists and return its pid
  if ( access(Config->lock_file,F_OK) != -1 ) {
    lckf = fopen(Config->lock_file,"r");
    if ( fscanf(lckf,"%d",&pid) == 0 ) {
      printf("WARNING - Lock file '%s' found but could not read the PID\n",Config->lock_file);
      pid = -1;
    }
    fclose(lckf);
    return pid;
  }

  // Create lock file and write own pid in it
  lckf = fopen(Config->lock_file,"w");
  fprintf(lckf,"%d",getpid());
  fclose(lckf);
  printf("- create_lock - Lock file '%s' created for PID %d\n",Config->lock_file,getpid());
  return 0;

}

// Remove lock file (called just before exiting)
void remove_lock()
{
  if ( unlink(Config->lock_file) ) {
    printf("WARNING - Problem while removing lock file '%s'\n",Config->lock_file);
  } else {
    printf("- remove_lock - Lock file '%s' removed\n",Config->lock_file);
  }
  return;
}

// Handle failure in initialization phase
void init_fail()
{
  FILE* iff;
  if ( access(Config->initfail_file,F_OK) == -1 ) {
    iff = fopen(Config->initfail_file,"w");
    fclose(iff);
    printf("- init_fail - InitFail file '%s' created\n",Config->initfail_file);
  } else {
    printf("- init_fail - InitFail file '%s' already exists (?)\n",Config->initfail_file);
  }
  remove_lock();
  exit(1);
}

// Start of main program
int main(int argc, char*argv[])
{

  pid_t pid;
  int c;
  //int runnr;
  int rc;

  /*
  // Show current size of stack and set it to high value
  int rlrc;
  struct rlimit stack_rlim;
  rlrc = getrlimit(RLIMIT_STACK,&stack_rlim);
  if (rlrc==0) {
    printf("Initial stack values: current %lld max %lld\n",(long long)stack_rlim.rlim_cur,(long long)stack_rlim.rlim_max);
  } else {
    printf("Initial getrlimit returned error %d\n",rlrc);
  }
  stack_rlim.rlim_cur = 100000000;
  rlrc = setrlimit(RLIMIT_STACK,&stack_rlim);
  if (rlrc!=0) {
    printf("setrlimit returned error %d\n",rlrc);
  }
  rlrc = getrlimit(RLIMIT_STACK,&stack_rlim);
  if (rlrc==0) {
    printf("Final stack values: current %lld max %lld\n",(long long)stack_rlim.rlim_cur,(long long)stack_rlim.rlim_max);
  } else {
    printf("Final getrlimit returned error %d\n",rlrc);
  }
  */

  // Make sure local data types are correct for us
  //printf("char: %d\nshort: %d\nint: %d\nlong: %d\nlong long: %d\n",
  //	 sizeof(char),sizeof(short),sizeof(int),sizeof(long),sizeof(long long));
  //printf("float: %d\ndouble: %d\nlong double: %d\n",sizeof(float),sizeof(double),sizeof(long double));
  if (sizeof(int)<4) {
    printf("*** On this system sizeof(int) is %lu bytes while we need at least 4 bytes. Aborting ***\n",sizeof(int));
    exit(1);
  }
  if (sizeof(long)<8) {
    printf("*** On this system sizeof(long) is %lu bytes while we need at least 8 bytes. Aborting ***\n",sizeof(long));
    exit(1);
  }

  // Use line buffering for stdout
  setlinebuf(stdout);

  // Show welcome message
  printf("=======================================\n");
  printf("=== Welcome to the PADME DAQ system ===\n");
  printf("=======================================\n");

  // Initialize run configuration
  if ( init_config() ) {
    printf("*** ERROR *** Problem initializing run configuration.\n");
    exit(1);
  }

  // Parse options
  //while ((c = getopt (argc, argv, "c:r:h")) != -1)
  while ((c = getopt (argc, argv, "c:h")) != -1)
    switch (c)
      {
      case 'c':
	// Check if another configuration file was specified
	if ( strcmp(Config->config_file,"")!=0 ) {
	  printf("*** ERROR *** Multiple configuration files specified: '%s' and '%s'.\n",Config->config_file,optarg);
	  exit(1);
	}
	// Read configuration parameters from configuration file
	if ( read_config(optarg) ) {
	  printf("*** ERROR *** Problem while reading configuration file '%s'.\n",optarg);
	  exit(1);
	}
        break;
      /*
      case 'r':
	if ( Config->run_number != 0 ) {
          fprintf (stderr, "Error: multiple definitions of run number.\n");
	  exit(1);
	}
        if ( sscanf(optarg,"%d",&runnr) != 1 ) {
          fprintf (stderr, "Error while processing option '-r'. Wrong parameter '%s'.\n", optarg);
	  exit(1);
	}
	if (runnr < 0) {
	  fprintf (stderr, "Error while processing option '-r'. Invalid run number %d.\n", runnr);
	  exit(1);
	}
	Config->run_number = runnr;
        break;
      */
      case 'h':
	fprintf(stdout,"\nPadmeDAQ [-c cfg_file] [-r run_nr] [-R] [-m comment] [-h]\n\n");
	fprintf(stdout,"  -c: use file 'cfg_file' to set configuration parameters for this process\n");
	fprintf(stdout,"     If no file is specified, use default settings\n");
	//fprintf(stdout,"  -r: set run number to 'run_nr'\n");
	//fprintf(stdout,"     If no run number is specified, the run is dummy: no data are saved to DB\n");
	fprintf(stdout,"  -h: show this help message and exit\n\n");
	exit(0);
      case '?':
        if (optopt == 'c' || optopt == 'r')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option '-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character '\\x%x'.\n",optopt);
        exit(1);
      default:
        abort ();
      }

  // Run number verification: if run number is not 0, it must exist in the DB
  if ( Config->run_number ) {

    // Connect to DB
    //if ( db_init(Config->db_file) != DB_OK ) exit(1);
    //if ( db_init(Config->db_file) != DB_OK ) init_fail();
    if ( db_init() != DB_OK ) init_fail();

    // Verify if run number is valid
    rc = db_run_check(Config->run_number);
    if ( rc == -1 ) {
      printf("ERROR: DB check for run number %d returned ad error\n",Config->run_number);
      init_fail();
    } else if ( rc == 0 ) {
      printf("ERROR: run number %d does not exist in the DB\n",Config->run_number);
      init_fail();
    }

  }

  // Show some startup message
  if (Config->run_number == 0) {
    printf("\n=== Starting PadmeDAQ acquisition for dummy run ===\n");
    printf("- WARNING: data will not be registered in the DB!\n");
  } else {
    printf("\n=== Starting PadmeDAQ acquisition for run %d ===\n",Config->run_number);
  }
  printf("- Run type: '%s'\n",Config->run_type);

  // Show configuration
  print_config();

  // Check if another DAQ is running
  printf("\n=== Verifying that no other DAQ instances are running ===\n");
  if ( (pid = create_lock()) ) {
    printf("*** ERROR *** Another DAQ is running with PID %d. Exiting.\n",pid);
    init_fail();
  }

  // Connect to digitizer
  printf("\n=== Connect to digitizer ===\n");
  if ( DAQ_connect() ) {
    printf("*** ERROR *** Problem while connecting to V1742 digitizer. Exiting.\n");
    init_fail();
  }

  // Initialize and configure digitizer
  printf("\n=== Initialize digitizer ===\n");
  if ( DAQ_init() ) {
    printf("*** ERROR *** Problem while initializing V1742 digitizer. Exiting.\n");
    init_fail();
  }

  // Handle data acquisition
  printf("\n=== Starting data acquisition ===\n");
  rc = DAQ_readdata();
  if ( rc == 1 ) {
    printf("*** ERROR *** Problem while initializing DAQ process. Exiting.\n");
    init_fail();
  } else if ( rc == 2 ) {
    printf("*** ERROR *** Data acquistion ended with an error. Please check log file for details. Exiting.\n");
    remove_lock();
    exit(1);
  } else if ( rc == 3 ) {
    printf("=== Run aborted before starting DAQ ===\n");
  }

  // Final reset of the digitizer
  printf("\n=== Reset digitizer and close connection ===\n");
  if ( DAQ_close() ) {
    printf("*** ERROR *** Final reset of digitizer ended with an error. Exiting.\n");
    remove_lock();
    exit(1);
  }

  // Remove lock file
  remove_lock();

  // Clean up configuration
  end_config();

  // All is done: exit
  printf("\n=== May the force be with you. Bye! ===\n");
  exit(0);

}
