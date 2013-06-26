/*
 * user.c
 *
 *  Created on: Feb 13, 2013
 *      Author: cjw
 */

#include <glib.h>
#include <stdio.h>

#include "user.h"

GQuark user_quark(void) {

	return g_quark_from_static_string(__FILE__);

}

int user_add_mail(user_t* usr, char* mail, GError** gerr) {

	int RESULT = 0;

	g_assert((usr != NULL) && (mail != NULL));

	if (*mail == '\0') {
		g_set_error(gerr, USER_MODULE, MAIL_EMPTY, "mail is an empty string");
		goto exit_user_add_mail;
	}

	usr->mail = g_slist_append(usr->mail, g_strdup(mail));

	RESULT = 1;

exit_user_add_mail:

	return RESULT;

}
int user_free(user_t* usr) {

	int RESULT = 0;

	return RESULT;

}
