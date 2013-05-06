#include <test_runner.h>

void pass(void)
{
    TEST_ASSERT(get_incremented_value() == 1);
}

void fail(void)
{
    // This is to prove that unit tests don't affect each other. If they do,
    //this test will pass.
    TEST_ASSERT(get_incremented_value() == 2);
}

void exception(void)
{
}
