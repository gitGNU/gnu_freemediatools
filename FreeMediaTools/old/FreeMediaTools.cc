/* 
   FreeMediaTools - Free/Libre Software for Video Processing

   Copyright (C) 2008 James Michael DuPont

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

*/

#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include "system.h"

#define EXIT_FAILURE 1

extern "C" {
  char *xstrdup (char *p);
}

static void usage (int status);

/* The name the program was run with, stripped of any leading path. */
char *program_name;

/* Option flags and variables */

char *oname = "stdout";		/* --output */
FILE *ofile;
char *new_directory;		/* --cd */
char *desired_directory;	/* --directory */
int want_interactive;		/* --interactive */
int want_quiet;			/* --quiet, --silent */
int want_brief;			/* --brief */
int want_dry_run;		/* --dry-run */
int want_no_warn;		/* --no-warn */

static struct option const long_options[] =
{
  {"interactive", no_argument, 0, 'i'},
  {"output", required_argument, 0, 'o'},
  {"quiet", no_argument, 0, 'q'},
  {"silent", no_argument, 0, 'q'},
  {"verbose", no_argument, 0, 'v'},
  {"dry-run", no_argument, 0, 18},
  {"no-warn", no_argument, 0, 19},
  {"brief", no_argument, 0, 20},
  {"cd", required_argument, 0, 21},
  {"directory", required_argument, 0, 22},
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'V'},
  {NULL, 0, NULL, 0}
};

static int decode_switches (int argc, char **argv);

int
main (int argc, char **argv)
{
  int i;

  program_name = argv[0];

  i = decode_switches (argc, argv);

  /* do the work */

  exit (0);
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

static int
decode_switches (int argc, char **argv)
{
  int c;

  ofile = stdout;

  while ((c = getopt_long (argc, argv, 
			   "i"	/* interactive */
			   "q"	/* quiet or silent */
			   "v"	/* verbose */
			   "o:"	/* output */
			   "h"	/* help */
			   "V",	/* version */
			   long_options, (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 'i':		/* --interactive */
	  want_interactive = 1;
	  break;
	case 'o':		/* --output */
	  oname = xstrdup(optarg);
	  ofile = fopen(oname, "w");
	  if (!ofile)
	    {
	      fprintf(stderr, "cannot open %s for writing", optarg);
	      exit(1);
	    }
	  break;
	case 'q':		/* --quiet, --silent */
	  want_quiet = 1;
	  break;
	case 18:		/* --dry-run */
	  want_dry_run = 1;
	  break;
	case 19:		/* --no-warn */
	  want_no_warn = 1;
	  break;
	case 20:		/* --brief */
	  want_brief = 1;
	  break;
	case 21:		/* --cd */
	  new_directory = xstrdup(optarg);
	  break;
	case 22:		/* --directory */
	  desired_directory = xstrdup(optarg);
	  break;
	case 'V':
	  printf ("FreeMediaTools %s\n", VERSION);
	  exit (0);

	case 'h':
	  usage (0);

	default:
	  usage (EXIT_FAILURE);
	}
    }

  return optind;
}


static void
usage (int status)
{
  printf (_("%s - Free/Libre Software for Video Processing\n"), program_name);
  printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
  printf (_("Options:\n"
"  -o, --output NAME          send output to NAME instead of standard output\n"
"  -i, --interactive          prompt for confirmation\n"
"  --dry-run                  take no real actions\n"
"  --no-warn                  disable warnings\n"
"  -q, --quiet, --silent      inhibit usual output\n"
"  --brief                    shorten output\n"
"  --verbose                  print more information\n"
"  --directory NAME           use specified directory \n"
"  --cd NAME                  change to specified directory before proceeding\n"
"  -h, --help                 display this help and exit\n"
"  -V, --version              output version information and exit\n"
));
  exit (status);
}
