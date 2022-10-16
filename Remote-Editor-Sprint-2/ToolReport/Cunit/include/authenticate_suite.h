#ifndef AUTHENTICATE_SUITE_H_
#define AUTHENTICATE_SUITE_H_

#include <CUnit/Basic.h>
#include "authenticate.h"

int init_suite_authenticate(void);
int clean_suite_authenticate(void);
void testSunnyCases1(void);
void testRainyCases1(void);

#endif  /* AUTHENTICATE_SUITE_H_ */