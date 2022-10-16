#include <CUnit/Basic.h>

#include "../include/authenticate.h"
#include "../include/authenticate_suite.h"
#include "../include/load.h"
#include "../include/load_suite.h"
int main() {
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite 1 to the registry */
    CU_pSuite pSuite1 = NULL;
    pSuite1 = CU_add_suite("Suite Authenticate file", init_suite_authenticate, clean_suite_authenticate);
    if (NULL == pSuite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite1, "test of authenticate() in Sunny cases", testSunnyCases1)) || (NULL == CU_add_test(pSuite1, "test of authenticate() in Rainy cases", testRainyCases1))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite pSuite2 = NULL;
    pSuite2 = CU_add_suite("Suite Load file", init_suite_load, clean_suite_load);
    if (NULL == pSuite2) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite 
    if ((NULL == CU_add_test(pSuite2, "test of load() in Sunny cases", testSunnyCases1)) || (NULL == CU_add_test(pSuite1, "test of load() in Rainy cases", testRainyCases1))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
