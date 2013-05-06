/*  DJs embedded toolkit test running

    Copyright (C) 2008 Dushara Jayasinghe.

    CUnitWin32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CUnitWin32 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CUnitWin32.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <libgen.h>

#include <test_runner.h>

#define LIB_VERSION     "1.0.0"
#define LIB_NAME	    "test_runner"

/* If the user hasn't defined a setup macro, make our own */
#ifndef TEST_SETUP
#define TEST_SETUP()
#endif

/* If the user hasn't defined a teardown macro, make our own */
#ifndef TEST_TEARDOWN
#define TEST_TEARDOWN()
#endif

/* Everything we need to know about a test case */
typedef struct {
	void (*function)(void);
	const char * const name;
}test_info_t;

/* Generate a list of all the unit tests */
#define TEST_INFO_ITEM(name)	{name, #name },
static const test_info_t test_list[] = {
    TEST_LIST(TEST_INFO_ITEM)
};

static bool last_test_failed;

/* List of command line options */
static const struct option opts[] = {
	{ "help",       0,                  0,  'h' },
	{ "test",       required_argument,  0,  't' },
	{ "xml",        0,                  0,  'x' },
	{ "version",    0,                  0,  'v' },
	{ 0, 0, 0, 0 }
};

static void help(const char *myself)
{
	fprintf(stderr,
		"Usage: %s [OPTION]...\n\n"
		"Execute the tests " LIB_NAME " was compiled with.\n\n"
		" -h, --help           Print this help, then exit\n"
		" -v, --version        Print version number, then exit\n"
		" -t, --test TEST_NAME Run only TEST_NAME\n"
		" -x, --xml            Output results in XML format\n\n"
		"For bug reports contact the person you got this utility from :-)\n", myself);
}

static void version(const char *myself)
{
	printf(
		"%s using " LIB_NAME " version " LIB_VERSION "\n"
		"Copyright (C) 2008 Dushara Jayasinghe\n"
		"This is free software; see the source for copying conditions.\n"
		"There is NO warranty; not even for MERCHANTABILITY or\n"
		"FITNESS FOR A PARTICULAR PURPOSE.\n\n", myself);
}

static void print_header(const char *suite_name, bool xml_mode)
{
	if(xml_mode)
	{
		printf(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
			"<embedded-test-results>"
			"<test-suite name=\"%s\">"
			"<results>\n", suite_name);
			fflush(stdout);
	}
}

static void print_test_name(const test_info_t* test_info,
                            bool use_xml_tags)
{
    /* Check the validity of arguments */
    if( test_info )
    {
        /* Is it in XML mode? */
        if(use_xml_tags)
        {
            printf(
                "<test-case name=\"%s\" assertion=\"",
                test_info->name
                );
        }
        else
        {
            printf("%s\n", test_info->name);
        }
        /* Flush the output (of the child process) so that things don't
         * appear out of sequence. */
        fflush(stdout);
    }
}

static void print_results(bool success, bool xml_mode)
{
	if(xml_mode)
	{
		printf(
			"\" success=\"%s\">"
			"\n</test-case>\n",
			(success)?"True":"False"
			);
		fflush(stdout);
	}
}

static void print_summary(int total,
                          int failure_count,
                          bool xml_mode)
{
	if(xml_mode)
	{
		printf(
			"</results>\n"
			"<summary executed=\"%d\" failed=\"%d\">"
			"</summary>"
			"</test-suite>"
			"</embedded-test-results>", total, failure_count);
	}
	else
	{
		printf("\n\ntotal = %d, failed = %d\n", total, failure_count);
	}
}

static bool is_find_test(const char* test_name,
                         const test_info_t** specified_test)
{
    bool found = false;
    
    /* check argument validity */
    if( test_name && specified_test )
    {
        unsigned int i;
        /* Assign output arg to a value indicating not found. */
        *specified_test = 0;
        /* Find the test name in our list */
        for( i=0; i<sizeof(test_list)/sizeof(test_list[0]); i++)
        {
            /* Is this it? */
            if((strcmp(test_name, test_list[i].name)) == 0)
            {
                /* Yes. Assign the return values. */
                *specified_test = &test_list[i];
                found = true;
            }
        }
    }
    return found;
}

static unsigned int run_specified_test(const test_info_t* specified_test,
                                       bool xml_mode)
{
    /* Assume it's going to fail */
    int failed = 1;
    
    if( specified_test )
    {
        print_test_name(specified_test, xml_mode);
        TEST_SETUP();
        specified_test->function();
        print_results(!last_test_failed, xml_mode);
    }
    
    failed = (last_test_failed)?1:0;
    
    return failed;
}

static unsigned int run_all_tests(const char* myself,
                                  bool xml_mode)
{
    unsigned int failure_count = 0;
    unsigned int i;
    print_header(myself, xml_mode);

    for(i=0;i<sizeof(test_list)/sizeof(test_list[0]);i++)
    {
        pid_t test_case_pid = fork();
        /* Check that the fork succeed */
        if( test_case_pid >= 0 )
        {
            /* Successfull fork. Are we the parent? */
            if( test_case_pid > 0 )
            {
                int status = 0;
                pid_t rval = 0;
                
                /* Parent waits until the child completes. */
                rval = wait(&status);
                /* Look at the childs return code to see if the
                 * test passed */
                if( status != 0 )
                {
                    /* Test failed. Increment the fail counter */
                    failure_count++;
                }
                else
                {
                    /* Test passed. Keep going. */
                }
            }
            else
            {
                /* This is the child process. Run the test */
                const test_info_t* test_info = &test_list[i];
                unsigned int failed = run_specified_test(test_info, xml_mode);
                
                /* Return with the fail count */
                exit(failed);
            }
        }
        else
        {
             /* Critical error */
        }   
    }
    print_summary(i, failure_count, xml_mode);
    
    return failure_count;
}

void notify_failure(void)
{
    last_test_failed = true;
}

int main(int argc, char* argv[])
{
    bool xml_mode = false;
	bool skip_test = false;
    bool still_parsing = true;
    unsigned int parse_guard = 0;
    unsigned int failure_count = 0;
    
    const char *test_to_run = 0;
    const char* myself = basename(argv[0]);
    
    /* Parse all the items in the command line */
    for( parse_guard=0;
         still_parsing && (parse_guard<sizeof(opts)/sizeof(opts[0]));
         parse_guard++)
    {
        /* Extract the next parsed option */
		int c = 0;
        int option_index = 0;
		c = getopt_long(argc, argv, "ht:x", opts, &option_index);
        /* Is the parsing complete? */
        if(c == -1)
        {
            /* Yes. */
            still_parsing = false;
        }
        else
        {
            /* No what's the next option? */
            switch (c) {
            case 'h':
                help(myself);
                skip_test = true;
                break;
            case 'v':
                version(myself);
                skip_test = true;
                break;
            case 't':
                test_to_run = optarg;
                break;
            case 'x':
                xml_mode = true;
                break;
            default:
                help(myself);
                skip_test = true;
            }
        }
    }
    
	if(!skip_test)
	{
        const test_info_t* specified_test = 0;
        
        /* Is the specified test in our registry? */
        if( is_find_test(test_to_run, &specified_test) )
        {
            /* Yes. Run that then. */
            print_header(myself, xml_mode);
            failure_count = run_specified_test(specified_test, xml_mode);
            print_summary(1, failure_count, xml_mode);
        }
        else
        {
            /* No. Run all the test cases */
            failure_count = run_all_tests(myself, xml_mode);
        }
	}

	return (failure_count)?-1:0;
}
