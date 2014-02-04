
#include "testRunner.h"

static unsigned int counter;

unsigned int testRunner(TEST_FUNCTION testFunction) {
    int err;
    counter = 0;
    err = (testFunction)();
    if (err) {
        printf("NG (now %d ok...)\n",counter);
        return 0xffffu;
    }
    printf("OK (%d tests)\n",counter);
    return 0;
}

void doTestCountUp(void) {
    counter++;
}
