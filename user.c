/*
 * user.c
 *
 *  Created on: Feb 13, 2013
 *      Author: cjw
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "user.h"

GQuark _user_quark(void) {

	return g_quark_from_static_string(__FILE__);

}

int user_add_mail(user_t* usr, char* mail, GError** gerr) {

	int RESULT = 0;

	g_assert((usr != NULL) && (mail != NULL));

	if (*mail == '\0') {
		g_set_error(gerr, USER_MODULE, MAIL_EMPTY, "mail is an empty string");
		goto exit_user_add_mail;
	}
	if (!g_regex_match_simple("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$",
			mail, G_REGEX_CASELESS, 0)) {
		g_set_error(gerr, USER_MODULE, MAIL_INVALID, "mail is invalid");
		goto exit_user_add_mail;
	}

	usr->mail = g_slist_append(usr->mail, g_strdup(mail));

	RESULT = 1;

exit_user_add_mail:

	return RESULT;

}
int user_remove_mail(user_t* usr, char* mail) {

	size_t   len;
	GSList** this;
	GSList*  ptr = NULL;

	g_assert((usr != NULL) && (mail != NULL));

	this = &usr->mail;
	do {
		len = strlen((*this)->data);
		if (!strncasecmp((*this)->data, mail, len)) {
			ptr = *this;
			*this = (*this)->next;
			g_free(ptr->data);
			g_slist_free_1(ptr);
			break;
		}
		this = &(*this)->next;
	} while (*this);

	// if ptr == NULL, the mail address didn't match anyone of this user
	return (ptr != NULL);

}
void user_free(user_t* usr) {

	GSList* ptr;

	g_assert(usr != NULL);

	if (usr->mail) {
		ptr = usr->mail;
		do g_free(ptr->data);
		while ((ptr = ptr->next));
		g_slist_free(usr->mail);
	}

	if (usr->extra_fields) {
		ptr = usr->extra_fields;
		do g_free(ptr->data);
		while ((ptr = ptr->next));
		g_slist_free(usr->extra_fields);
	}

	g_free(usr);

}
