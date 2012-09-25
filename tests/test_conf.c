/*
 * test_conf.c
 *
 *  Created on: 2 sep 2012
 *      Author: cjw
 */

#include <stdio.h>
#include <glib.h>

#include "test_main.h"

#include "../conf.h"

typedef struct {
	gchar* key;
	gchar* val;
} cfgval_t;

extern cfgval_t def_vals[];

#define DIP_CONF_ERROR		dip_conf_error_quark()
static GQuark dip_conf_error_quark(void) {

	return g_quark_from_static_string("dip-conf-error-quark");

}

void test_conf_set(void) {

	GError* err = NULL;

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	g_assert(!conf_set("test_key", "test_value", 0, &err));
	g_assert(g_error_matches(err, DIP_CONF_ERROR, DIP_CONF_ERROR_VOID_VALUE));
	g_clear_error(&err);

	g_assert(conf_set("test_key", "test_value", 1, &err));
	g_assert(err == NULL);

	g_assert_cmpstr(g_hash_table_lookup(conf_table, "test_key"), ==, "test_value");

	g_hash_table_unref(conf_table);

}
void test_conf_get(void) {

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

	g_hash_table_insert(conf_table, "test_key", "test_value");

	// Should ignore case of keyname
	g_assert_cmpstr(conf_get("TEST_KEY"), ==, "test_value");
	g_assert(conf_get("NOT_FOUND") == NULL);

	g_hash_table_unref(conf_table);

}
void test_conf_read_file(void) {

	char *dir  = "./test_data";
	char *fn1  = "fail_parse.cfg";
	char *fn2  = "test_ok.cfg";
	GError* err = NULL;

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	// Create variables so they are known when reading file.
	g_hash_table_insert(conf_table, g_strdup("judge_code"), g_strdup(""));
	g_hash_table_insert(conf_table, g_strdup("judge_tz"), g_strdup(""));

	g_assert(!conf_read_file("./", "notfound", &err));
	g_assert(g_error_matches(err, DIP_CONF_ERROR, DIP_CONF_ERROR_FILE));
	g_clear_error(&err);

	g_assert(!conf_read_file(dir, fn1, &err));
	g_assert(g_error_matches(err, DIP_CONF_ERROR, DIP_CONF_ERROR_PARSE));
	g_clear_error(&err);

	g_assert(conf_read_file(dir, fn2, &err));

	g_assert_cmpstr(conf_get("judge_code"), ==, "TEST");
	g_assert_cmpstr(conf_get("judge_tz"), ==, "CET");

	g_hash_table_unref(conf_table);

}
void test_conf_init(void) {

	gint idx;

	conf_init();

	for (idx = 0; def_vals[idx].key; idx ++) {
		//printf("%s\n", cfg_vals[idx].key);
		g_assert_cmpstr(conf_get(def_vals[idx].key), ==, def_vals[idx].val);
	}

	conf_destroy();

}
void test_conf_get_int(void) {

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	g_hash_table_insert(conf_table, g_strdup("test_key_0"), g_strdup("0"));
	g_hash_table_insert(conf_table, g_strdup("test_key_1"), g_strdup("501"));
	g_hash_table_insert(conf_table, g_strdup("test_key_false"), g_strdup("false"));

	g_assert(!conf_get_bool("test_key_0"));
	g_assert_cmpint(conf_get_bool("test_key_1"), ==, 501);
	g_assert(!conf_get_bool("test_key_false"));

	g_hash_table_unref(conf_table);

}
void test_conf_get_bool(void) {

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	g_hash_table_insert(conf_table, g_strdup("test_key_0"), g_strdup("0"));
	g_hash_table_insert(conf_table, g_strdup("test_key_1"), g_strdup("1"));
	g_hash_table_insert(conf_table, g_strdup("test_key_false"), g_strdup("false"));
	g_hash_table_insert(conf_table, g_strdup("test_key_on"), g_strdup("on"));
	g_hash_table_insert(conf_table, g_strdup("test_key_true"), g_strdup("true"));
	g_hash_table_insert(conf_table, g_strdup("test_key_yes"), g_strdup("yes"));

	g_assert(!conf_get_bool("test_key_0"));
	g_assert(conf_get_bool("test_key_1"));
	g_assert(!conf_get_bool("test_key_false"));
	g_assert(conf_get_bool("test_key_on"));
	g_assert(conf_get_bool("test_key_true"));
	g_assert(conf_get_bool("test_key_yes"));

	g_hash_table_unref(conf_table);

}

tests_t tests_conf[] = {
		{"/conf/conf_set",		test_conf_set},
		{"/conf/conf_get",		test_conf_get},
		{"/conf/conf_readfile",	test_conf_read_file},
		{"/conf/conf_init",		test_conf_init},
		{"/conf/conf_get_int",	test_conf_get_int},
		{"/conf/conf_get_bool",	test_conf_get_bool},
		{NULL}
};
