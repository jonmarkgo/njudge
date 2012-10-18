/*
 * conf.c
 *
 *  Created on: 2 sep 2012
 *
 *      Author: cjw
 */

#include <glib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "conf.h"

#define DIP_CONF_ERROR		dip_conf_error_quark()

typedef struct {

	gchar* key;
	gchar* val;

} cfgval_t;

#ifndef UNITTEST
  static
#endif
GHashTable* conf_table = NULL;

#ifndef UNITTEST
  static
#endif
cfgval_t def_vals[] = {

		{"auto_master"			, "no"},
		{"bail_forward"			, "~/bail_forward"},
		{"bailout_msg"			, "bailout.msg"},
		{"bailout_plan"			, "~/bail_plan"},
		{"bitnet_gateway1"		, ""},
		{"bitnet_gateway2"		, ""},
		{"block_file"			, ""},			/* Use block file? Put path here */
		{"create_disabled"		, "no"},
		{"create_dedication"	, "-10"},
		{"db_name"				, ""},
		{"db_pass"				, ""},
		{"db_server"			, "localhost"},
		{"db_server_port"		, "0"},
		{"db_user"				, ""},
		{"deny_string"			, "judge@"},
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
		{"log_file"				, "dip.log"},
		{"mail_spooler"			, "mail_spooler"},
		{"master_db"			, "dip.master"},
		{"master_db_tmp"		, "dip.tmast"},
		{"max_games"			, "-1"},
		{"min_control"			, "43200"},
		{"nobody"				, "nobody@localhost"},
		{"plan"					, ""},
		{"points_abandon"		, "-49"},
		{"points_cd"			, "-50"},
		{"points_late"			, "-1"},
		{"points_ontime"		, "3"},
		{"special_pw"			, ""},
		{"time_tolerance"		, "0"},
		{"warp_file"			, "./dip.warp"}, /* temporary warp-message file */
		{"xforward"				, ""},
		/* Default commands */
		{"cmd_lenlimit"			, "./lenlimit"},
		{"cmd_atrun"			, "./atrun"},
		{"cmd_dip"				, "./dip"},
		{"cmd_smail"			, "./smail"},
		{"cmd_summary"			, "./summary"},
		/* Game defaults */
		{"gd_access"			, "1"},
		{"gd_level"				, "1"},
		{"gd_flags"				, "268566531"},
		{"gd_flags_xtra1"		, "0"},
		{"gd_flags_xtra2"		, "0"},
		{"gd_flags_xtra3"		, "0"},
		{"gd_dedicate"			, "-10000"},
		{"gd_move_clock"		, "1410"},
		{"gd_move_mint"			, "12.0"},
		{"gd_move_next"			, "71.0"},
		{"gd_move_grace"		, "167.0"},
		{"gd_move_delay"		, "0.5"},
		{"gd_move_days"			, "-MTWTF-"},
		{"gd_retreat_clock"		, "-1"},
		{"gd_retreat_mint"		, "12.0"},
		{"gd_retreat_next"		, "71.0"},
		{"gd_retreat_grace"		, "167.0"},
		{"gd_retreat_delay"		, "0.5"},
		{"gd_retreat_days"		, "-mTWTF-"},
		{"gd_build_clock"		, "-1"},
		{"gd_build_mint"		, "12.0"},
		{"gd_build_next"		, "71.0"},
		{"gd_build_grace"		, "167.0"},
		{"gd_build_delay"		, "0.5"},
		{"gd_build_days"		, "-mTWTF-"},
		{"gd_max_absence_delay"	, "15"},
		/* Variant specific variables? */
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
		{"bn_custodian"			, "nobody"},
		{"mn_custodian"			, "nobody"},
		{"EP_CUSTODIAN"			, "nobody"},
		/* Default custodians for sailho (other variants will default to nothing */
		/* {"CUSTODIAN_sailho"		, "tarzan_monkeyman@bigfoot.com"},
		{"CUSTODIAN_sailho_crowded", "tarzan_monkeyman@bigfoot.com"}, */
		/* Default custodians for 1900? */
		/* {"CUSTODIAN_1900_1"		, "VonPowell@aol.com"},
		{"CUSTODIAN_1900_2"		, "VonPowell@aol.com"},
		{"CUSTODIAN_1900_3"		, "VonPowell@aol.com"}, */
		{NULL					, NULL}

};

static GQuark dip_conf_error_quark(void) {

	return g_quark_from_static_string("dip-conf-error-quark");

}
void conf_destroy(void) {

	g_assert(conf_table != NULL);

	g_hash_table_unref(conf_table);

}
gchar* conf_get(gchar *var) {

	gchar* res;
	gchar* lower;

	g_assert((var != NULL) && (*var != 0));

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
gfloat conf_get_float(gchar* key) {

	gfloat rtn;
	gchar* val;

	if (NULL == (val = conf_get(key))) {
		return 0;
	}

	sscanf(val, "%f", &rtn);

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

	/* Set default (and allowed) values */
	for (itr = 0; def_vals[itr].key; itr ++) {
		conf_set(def_vals[itr].key, def_vals[itr].val, 1, NULL);
	}

	return 1;

}
gint conf_read_file(gchar *dir, gchar *bname, GError** err) {

	gint    rtn = 1;
	guint   lc = 0;
	gchar*  fn;
	gchar   line[256];
	FILE*   fp = NULL;

	g_assert(err == NULL || *err == NULL);
	g_assert(((dir != NULL) && (*dir != 0)) && ((bname != NULL) && (*bname != 0)));

	fn = g_strdup_printf("%s/%s", dir, bname);

	if (NULL == (fp = fopen(fn, "r"))) {
		g_set_error(err, DIP_CONF_ERROR, DIP_CONF_ERROR_FILE,
				"failed to open config file - %s: %s", bname, g_strerror(errno));
		rtn = 0;
		goto exit_conf_read_file;
	}

	while ((NULL != fgets(line, 256, fp))) {
		lc ++;
		if (!conf_textual_set(line, err)) {
			if (err == NULL || *err == NULL) continue;
			g_prefix_error(err, "error at line %u: ", lc);
			rtn = 0;
			goto exit_conf_read_file;
		}
	}

exit_conf_read_file:

	if (fp) fclose(fp);
	g_free(fn);

	return rtn;

}
gint conf_set(gchar *var, gchar *val, gint init, GError** err) {

	g_assert(err == NULL || *err == NULL);
	g_assert((var != NULL) && (val != NULL));

	if (!init && !g_hash_table_lookup(conf_table, var)) {
		g_set_error(err, DIP_CONF_ERROR, DIP_CONF_ERROR_VOID_VALUE, "void value - %s", var);
		return 0;
	}

	g_hash_table_insert(conf_table, g_ascii_strdown(var, -1), g_strdup(val));

	return 1;

}
gint conf_vset(GPtrArray* arr, GError** err) {

	gint   idx;
	gint   rtn = 1;
	gchar* tcptr;

	g_assert(arr != NULL && (err == NULL || *err == NULL));

	for (idx = 0; idx < arr->len; idx ++) {
		tcptr = g_ptr_array_index(arr, idx);
		if (!conf_textual_set(tcptr, err)) {
			if (!err) continue;
			g_prefix_error(err, "config option error, ");
			rtn = 0;
			break;
		}
	}

	return rtn;

}
gint conf_textual_set(gchar* line, GError** err) {

	gint		rtn = 1;
	gchar*		tmpl;
	gchar*		key       = NULL;
	gchar*		val       = NULL;
	GRegex*     rex       = NULL;
	GMatchInfo* rex_match = NULL;

	tmpl = g_strstrip(g_strdup(line));

	if (!(*tmpl) || strchr("#;", *tmpl)) {
		rtn = 0;
		goto exit_conf_textual_set;
	}

	rex = g_regex_new("^\\s*(\\w+)\\s*=\\s*(.*?)\\s*$", 0, 0, NULL);

	if (!g_regex_match(rex, line, 0, &rex_match)) {
		g_set_error(err, DIP_CONF_ERROR, DIP_CONF_ERROR_PARSE, "parse error - %s", line);
		rtn = 0;
		goto exit_conf_textual_set;
	}

	key = g_match_info_fetch(rex_match, 1);
	val = g_match_info_fetch(rex_match, 2);

	if (!conf_set(key, val, 0, err)) {
		rtn = 0;
	}

exit_conf_textual_set:

	g_free(tmpl);

	if (key) {
		g_free(key);
		g_free(val);
	}
	if (rex) g_regex_unref(rex);
	if (rex_match) g_match_info_free(rex_match);

	return rtn;

}
