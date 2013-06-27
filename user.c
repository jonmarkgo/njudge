/*
 * user.c
 *
 * Documentation:
 *
 *   A new user should be created with g_new0(user_t, 1) or similar.
 *
 * ---------------------------------------------------------------------------
 *   int user_add_mail(user_t* usr, char* mail, GError** gerr)
 *
 *     Adds an e-mail address to the users list.
 *
 *     [in]      user : User
 *     [in]      mail : Mail address to add to users list
 *     [opt/out] gerr : An optional GError pointer that receives an eventual
 *                      error
 *
 *     Returns non zero on success.
 *
 *     If the e-mail address already exists, the addition is ignored and
 *     success is returned.
 *
 *     Errors:
 *
 *       - MAIL_EMPTY
 *           The e-mail address is an empty string.
 *       - MAIL_INVALID
 *           The e-mail address doesn't qualify as an valid e-mail address
 *
 * ---------------------------------------------------------------------------
 *   void user_remove_mail(user_t* usr, char* mail)
 *
 *     Removes an e-mail address from the users list.
 *
 *     [in]		usr	 : User
 *     [in]		mail : Mail address to be removed
 *
 *     This function never fails.
 *
 * ---------------------------------------------------------------------------
 *   void user_free(user_t* usr)
 *
 *     Frees a user object.
 *
 *     [in]		usr  : User object to be freed
 *
 *     This function never fails.
 *
 */

#include <glib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "user.h"

GQuark _user_quark(void) {

	return g_quark_from_static_string(__FILE__);

}
#ifndef UNITTEST
static
#endif
char* _str_to_lower(char* str) {

	char* ptr;

	for (ptr = str; *ptr; ptr ++) *ptr = tolower(*ptr);

	return str;

}
int user_add_mail(user_t* usr, char* mail, GError** gerr) {

	int     len;
	int     RESULT = 0;
	GSList* ptr;

	g_assert((usr != NULL) && (mail != NULL));

	if (*mail == '\0') {
		g_set_error(gerr, USER_MODULE, MAIL_EMPTY, "mail is an empty string");
		goto exit_user_add_mail;
	}
	if (!g_regex_match_simple("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$",
			mail, G_REGEX_CASELESS, 0)) {
		g_set_error(gerr, USER_MODULE, MAIL_INVALID, "mail address is invalid");
		goto exit_user_add_mail;
	}

	/* check if the mail address already exists, ignore if that's the case */
	for (ptr = usr->mail; ptr; ptr = ptr->next) {
		len = strlen(ptr->data);
		if (!strncasecmp(ptr->data, mail, len)) break;
	}

	if (!ptr) usr->mail = g_slist_append(usr->mail, _str_to_lower(g_strdup(mail)));

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

	/* if ptr == NULL, the mail address didn't match anyone of this user */
	return (ptr != NULL);

}
void user_free(user_t* usr) {

	GSList* ptr;

	g_assert(usr != NULL);

	if (usr->address)  g_free(usr->address);
	if (usr->country)  g_free(usr->country);
	if (usr->name)     g_free(usr->name);
	if (usr->phone)    g_free(usr->phone);
	if (usr->site)     g_free(usr->site);
	if (usr->timezone) g_free(usr->timezone);

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
