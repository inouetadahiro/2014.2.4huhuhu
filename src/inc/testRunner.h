
#ifndef _TEST_RUNNER_H_
#define _TEST_RUNNER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

//�e�X�g�֐���0�Ő����A��0�Ŏ��s
//TEST_ASSERT(!func)
//TEST_ASSERT�̈����͔�0�Ő����A0�Ŏ��s

/** Assert single */
#define TEST_ASSERT(_a) \
    {\
        int assertErrorA = (_a);\
        if (! assertErrorA) {\
            printf("%s:%d: error: TestCaseError(0x%x)\n",\
                __FILE__,__LINE__,(assertErrorA));\
            return 0xffffu;\
        } else {\
            doTestCountUp();\
        }\
    }

/** Assert equals */
#define TEST_ASSERT_EQUALS(_a,_b) \
    {\
        int assertErrorA = (_a);\
        int assertErrorB = (_b);\
        if ((assertErrorA) != (assertErrorB)) {\
            printf("%s:%d: error: TestCaseError<0x%x><0x%x>\n",\
                __FILE__,__LINE__,(assertErrorA),(assertErrorB));\
            return 0xffffu;\
        } else {\
            doTestCountUp();\
        }\
    }

/** Assert equals */
#define TEST_ASSERT_NOT_EQUALS(_a,_b) \
    {\
        int assertErrorA = (_a);\
        int assertErrorB = (_b);\
        if ((assertErrorA) == (assertErrorB)) {\
            printf("%s:%d: error: TestCaseError<0x%x><0x%x>\n",\
                __FILE__,__LINE__,(assertErrorA),(assertErrorB));\
            return 0xffffu;\
        } else {\
            doTestCountUp();\
        }\
    }

/** File pointer define */
typedef unsigned int (*TEST_FUNCTION)(void);

/** Test runner. */
unsigned int testRunner(TEST_FUNCTION testFunction);

/** Test counter */
void doTestCountUp(void);

#ifdef __cplusplus
}
#endif
#endif 
