/*
 * conf.c
 *
 *  Created on: 2 sep 2012
 *
 *      Author: cjw
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "conf.h"

typedef struct {

	gchar* key;
	gchar* val;

} cfgval_t;

#ifndef UNITTEST
  static
#endif
GHashTable* conf_table = NULL;

static cfgval_t def_vals[] = {

		{"auto_master"			, "no"},
		{"bail_forward"			, "~/bail_forward"},
		{"bailout_msg"			, "bailout.msg"},
		{"bailout_plan"			, "~/bail_plan"},
		{"bitnet_gateway1"		, ""},
		{"bitnet_gateway2"		, ""},
		{"block_file"			, ""},			// Use block file? Put path here
		{"create_disabled"		, "no"},
		{"enable_syslog"		, "no"},
		{"forward"				, "~/.forward"},
		{"forward_onbail"		, "~/forward_onbail"},
		{"game_dir"				, "./games"},
		{"games_opener"			, ""},
		{"hall_keeper"			, ""},
		{"judge_code"			, "XXXX"},
		{"judge_keeper"			, "judgekeeper@localhost"},
		{"judge_path"			, "./"},
		{"judge_tz"				, ""},
		{"line_wrap"			, "78"},
		{"lock_timeout"			, "3600"},
		{"log_file"				, "njudgex.log"},
		{"mail_spooler"			, "mail_spooler"},
		{"master_db"			, "dip.master"},
		{"master_db_tmp"		, "dip.tmast"},
		{"min_control"			, "43200"},
		{"nobody"				, "nobody@localhost"},
		{"points_abandon"		, "-49"},
		{"points_cd"			, "-100"},
		{"points_late"			, "-1"},
		{"points_ontime"		, "3"},
		{"username"				, "judge"},
		{"warp_file"			, "./njudgex.warp"}, // temporary warp-message file
		{"xforward"				, ""},
		// Default commands
		{"cmd_lenlimit"			, "./lenlimit"},
		{"cmd_atrun"			, "./atrun"},
		{"cmd_dip"				, "./njudgex"},
		{"cmd_smail"			, "./smail"},
		{"cmd_summary"			, "./summary"},
		// Variant specific variables?
		{"DIE_ASSASSIN"			, "382204"},
		{"DIE_EXPENSE"			, "148741"},
		{"DIE_FAMPLAG"			, "995816"},
		{"DIE_STORMS"			, "341291"},
		{"DIE_INCOME"			, "66144"},
		/*
		 * BN_CUSTODIAN is the Boardman Custodian, who receives game starts
		 *              and ends for standard, nongunboat games
		 * MN_CUSTODIAN is the Miller Number Custodian, who receives the game
		 *              starts and ends for other games
		 * Neither of these should be altered, except on test judges.  These
		 * addresses may change from time to time.  Please revise if necessary.
		 */
		{"BN_CUSTODIAN"			, "nobody"},
		{"MN_CUSTODIAN"			, "nobody"},
		{"EP_CUSTODIAN"			, "nobody"},
		// Default custodians for sailho (other variants will default to nothing
		{"CUSTODIAN_sailho"		, "tarzan_monkeyman@bigfoot.com"},
		{"CUSTODIAN_sailho_crowded", "tarzan_monkeyman@bigfoot.com"},
		// Default custodians for 1900?
		{"CUSTODIAN_1900_1"		, "VonPowell@aol.com"},
		{"CUSTODIAN_1900_2"		, "VonPowell@aol.com"},
		{"CUSTODIAN_1900_3"		, "VonPowell@aol.com"},
		{NULL					, NULL}

};
void conf_destroy(void) {

	g_assert(conf_table != NULL);

	g_hash_table_unref(conf_table);

}
gchar* conf_get(gchar *var) {

	g_assert((var != NULL) && (*var != 0));

	gchar* res;
	gchar* lower;

	lower = g_ascii_strdown(var, -1);

	res = g_hash_table_lookup(conf_table, lower);

	g_free(lower);

	return res;

}
gint conf_get_bool(gchar* key) {

	gint   rtn = 0;
	gchar* val;

	if (NULL == (val = conf_get(key))) {
		return 0;
	}

	if (!sscanf(val, "%d", &rtn)) {
		if (!g_ascii_strcasecmp(val, "true") ||
				!g_ascii_strcasecmp(val, "on") ||
				!g_ascii_strcasecmp(val, "yes")) {
			rtn = 1;
		}
	}

	return rtn;

}
gint conf_get_int(gchar* key) {

	gint   rtn;
	gchar* val;

	if (NULL == (val = conf_get(key))) {
		return 0;
	}

	if (!sscanf(val, "%d", &rtn)) {
		rtn = 0;
	}

	return rtn;

}
gint conf_init(void) {

	gint itr;

	conf_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	// Set default (and allowed) values
	for (itr = 0; def_vals[itr].key; itr ++) {
		conf_set(def_vals[itr].key, def_vals[itr].val, 1);
	}

	return 1;

}
gint conf_read_file(gchar *dir, gchar *bname) {

	g_assert(((dir != NULL) && (*dir != 0)) && ((bname != NULL) && (*bname != 0)));

	gint    e;
	guint   lc = 1;
	gchar*  fn;
	gchar   line[256];
	FILE*   fp;
	GError* err	= NULL;

	fn = g_strdup_printf("%s/%s", dir, bname);

	if (NULL == (fp = fopen(fn, "r"))) {
		fprintf(stderr, "e> config file error - %s: %s", err->message);
		goto exit_conf_readfile;
	}

	while ((NULL != fgets(line, 256, fp))) {
		if ((e = conf_textual_set(line)) < 1) {
			if (e == -1) {
				fprintf(stderr, "e> config error, error parsing @ line %u\n", lc);
			} else if (e == -2) {
				fprintf(stderr, "e> config error, unknown config variable @ line %u\n", lc);
			}
		}
		lc ++;
	}

	fclose(fp);

exit_conf_readfile:

	g_free(fn);
	if (err) g_error_free(err);

	return (err == NULL);

}
gint conf_set(gchar *var, gchar *val, gint init) {

	g_assert((var != NULL) && (val != NULL));

	if (!init && !g_hash_table_lookup(conf_table, var)) {
		return 0;
	}

	g_hash_table_insert(conf_table, g_ascii_strdown(var, -1), g_strdup(val));

	return 1;

}
gint conf_textual_set(gchar* line) {

	gint		rtn = 1;
	gchar*		tmpl;
	gchar*		key			= NULL;
	gchar*		val			= NULL;
	GRegex*     rex			= NULL;
	GMatchInfo* rex_match	= NULL;

	tmpl = g_strstrip(g_strdup(line));

	if (!(*tmpl) || strchr("#;", *tmpl)) {
		rtn = 0;
		goto exit_conf_textual_set;
	}

	rex = g_regex_new("^\\s*(\\w+)\\s*=\\s*(.*?)\\s*$", 0, 0, NULL);

	if (!g_regex_match(rex, line, 0, &rex_match)) {
		rtn = -1;
		goto exit_conf_textual_set;
	}

	key = g_match_info_fetch(rex_match, 1);
	val = g_match_info_fetch(rex_match, 2);

	/*if (!g_hash_table_lookup(conf_table, key)) {
		rtn = -2;
		goto exit_conf_textual_set;
	}*/

	if (!conf_set(key, val, 0)) {
		rtn = -2;
	}

exit_conf_textual_set:

	g_free(tmpl);

	if (rtn) {
		g_free(key); g_free(val);
		if (rex) g_regex_unref(rex);
		if (rex_match) g_match_info_free(rex_match);
	}

	return rtn;

}
