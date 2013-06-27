/*
 * user.h
 *
 *  Created on: Feb 13, 2013
 *      Author: cjw
 */

#ifndef USER_H_
#define USER_H_

typedef struct userstat_s {

	guint  player_id;
	gint   dedication;
	time_t last_logon;
	guint  total;
    guint  ontime;
    guint  started;
    guint  resigned;
    guint  tookover;

} userstat_t;
typedef struct user_s {

	int    id;
	int	   level;
	time_t birthdate;
	int    sex;
	char*  name;
	char*  phone;
	char*  site;
	char*  address;
	char*  country;
	char*  timezone;
	userstat_t* stat;
	GSList*     mail;
	GSList*     extra_fields;

} user_t;

typedef enum _user_errors_e {

	MAIL_EMPTY = 1,
	MAIL_INVALID

} user_errors_t;

#define USER_MODULE		_user_quark()

int    user_add_mail(user_t* usr, char* mail, GError** gerr);
void   user_free(user_t* usr);
GQuark _user_quark(void);
int    user_remove_mail(user_t* usr, char* mail);

#endif /* USER_H_ */
