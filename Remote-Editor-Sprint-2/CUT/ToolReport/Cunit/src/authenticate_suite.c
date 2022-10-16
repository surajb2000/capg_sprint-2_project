#include "../include/authenticate.h"
#include "CUnit/Basic.h"

int init_suite_authenticate(void) {
    return 0;
}

int clean_suite_authenticate(void) {
    return 0;
}

void testSunnyCases1(void) {
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "nikita", "nikita15"), 2);
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "suraj", "suraj25"), 2);
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "dinesh", "dinesh16"), 2);
}
void testRainyCases1(void) {
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "anony", "anony"), 1);
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "author", "author23"), 1);
    CU_ASSERT_EQUAL(authentication("data/usercu.txt", "ruby", "ruby7"), 1);
}
