/*
 * test_main.c
 *
 *  Created on: 2 sep 2012
 *      Author: cjw
 */

#include <glib.h>

#include "test_main.h"

extern tests_t tests_conf[];

tests_t* all_tests[] = {
		tests_conf,
		NULL
};

int main(int argc, char** argv) {

	int itr_test;
	int itr_suite;
	tests_t* this_suite;

	g_test_init(&argc, &argv, NULL);

	for (itr_suite = 0; (this_suite = all_tests[itr_suite]); itr_suite ++) {
		for (itr_test = 0; this_suite[itr_test].path; itr_test ++) {
			g_test_add_func(this_suite[itr_test].path, this_suite[itr_test].test_func);
		}
	}

	g_test_run();

	return 0;

}
