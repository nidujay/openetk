#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <test_def.h>

/**/
#define TEST_ASSERT(expression)		do {										\
    if(!(expression)) {															\
        printf("%s @ %s in line %d failed\n", #expression, __FILE__, __LINE__);	\
        fflush(stdout);															\
        notify_failure();                                                       \
    }																			\
}while(0)

extern void notify_failure(void);

/* Generate the function prototypes for the test cases */
#define FUNCTION_PROTOTYPE(test_name)   \
    extern void test_name(void);
TEST_LIST(FUNCTION_PROTOTYPE)

#endif
