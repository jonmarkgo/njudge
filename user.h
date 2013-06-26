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
	char*  name;
	int	   level;
	time_t birthdate;
	int    sex;
	char*  phone;
	char*  site;
	char*  address;
	char*  country;
	char*  timezone;
	userstat_t* stat;
	GSList*     mail;
	GSList*     extra_fields;

} user_t;

typedef enum _error_e {

	MAIL_EMPTY = 1

} user_errors_t;

#define USER_MODULE		user_quark()

GQuark user_quark(void);
int user_add_mail(user_t* usr, char* mail, GError** gerr);

#endif /* USER_H_ */
