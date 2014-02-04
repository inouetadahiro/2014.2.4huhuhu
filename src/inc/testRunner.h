
#ifndef _TEST_RUNNER_H_
#define _TEST_RUNNER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

//テスト関数は0で成功、非0で失敗
//TEST_ASSERT(!func)
//TEST_ASSERTの引数は非0で成功、0で失敗

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
