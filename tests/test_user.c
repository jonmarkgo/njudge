/*
 * test_datetime.c
 *
 *  Created on: Nov 18, 2012
 *      Author: cjw
 */

#include <stdio.h>
#include <glib.h>

#include "test_main.h"

#include "../user.h"

char* _str_test_to_lower(char*);

void test__str_to_lower(void) {

	char test[] = "TeSt";

	g_assert(!strcmp(_str_to_lower(test), "test"));

}

void test_user_add_mail(void) {

	user_t* usr;
	GError* gerr = NULL;

	usr = g_new0(user_t, 1);

	usr->name = g_strdup("Test User");

	g_assert(user_add_mail(usr, "mail1@domain.com", &gerr));
	g_assert(gerr == NULL);
	g_assert_cmpstr(usr->mail->data, ==, "mail1@domain.com");
	g_assert(user_add_mail(usr, "mail2@domain.com", &gerr));
	g_assert(gerr == NULL);
	g_assert_cmpstr(usr->mail->next->data, ==, "mail2@domain.com");

	g_assert(user_add_mail(usr, "mail2@domain.com", &gerr));
	g_assert(2 == g_slist_length(usr->mail));

	g_assert(!user_add_mail(usr, "", &gerr));
	g_assert(gerr != NULL);
	g_assert(gerr->domain == USER_MODULE);
	g_assert(gerr->code   == MAIL_EMPTY);

	g_error_free(gerr); gerr = NULL;

	g_assert(!user_add_mail(usr, "mail3@domain", &gerr));
	g_assert(gerr != NULL);
	g_assert(gerr->domain == USER_MODULE);
	g_assert(gerr->code   == MAIL_INVALID);

	user_free(usr);

}
void test_user_remove_mail(void) {

	user_t* usr;

	usr = g_new0(user_t, 1);

	user_add_mail(usr, "mail1@domain.com", NULL);
	user_add_mail(usr, "mail2@domain.com", NULL);
	user_add_mail(usr, "mail3@domain.com", NULL);

	g_assert(!user_remove_mail(usr, "test"));
	g_assert(3 == g_slist_length(usr->mail));

	g_assert(user_remove_mail(usr, "mail2@domain.com"));
	g_assert(2 == g_slist_length(usr->mail));
	g_assert_cmpstr(usr->mail->data, ==, "mail1@domain.com");
	g_assert_cmpstr(usr->mail->next->data, ==, "mail3@domain.com");

	user_free(usr);

}

tests_t tests_user[] = {
		{"/user/_str_to_lower",			test__str_to_lower},
		{"/user/user_add_mail",			test_user_add_mail},
		{"/user/user_remove_mail",		test_user_remove_mail},
		{NULL}
};
