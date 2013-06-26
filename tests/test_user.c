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

void test_user_add_mail(void) {

	user_t* usr;
	GError* gerr = NULL;

	usr = g_new0(user_t, 1);

	g_assert(user_add_mail(usr, "mail1@domain.com", &gerr));
	g_assert(gerr == NULL);
	g_assert_cmpstr(usr->mail->data, ==, "mail1@domain.com");
	g_assert(user_add_mail(usr, "mail2@domain.com", &gerr));
	g_assert(gerr == NULL);
	g_assert_cmpstr(usr->mail->next->data, ==, "mail2@domain.com");

	g_assert(!user_add_mail(usr, "", &gerr));
	g_assert(gerr != NULL);
	g_assert(gerr->domain == USER_MODULE);
	g_assert(gerr->code   == MAIL_EMPTY);

	g_free(usr->mail->next->data);
	g_free(usr->mail->data);
	g_slist_free(usr->mail);
	g_free(usr);

}

tests_t tests_user[] = {
		{"/user/user_add_mail",			test_user_add_mail},
		{NULL}
};
