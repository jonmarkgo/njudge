/*
 * test_main.h
 *
 *  Created on: 2 sep 2012
 *      Author: cjw
 */

#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_

typedef struct tests_s {

	char* path;
	void (*test_func)(void);

} tests_t;

#endif /* TEST_MAIN_H_ */
