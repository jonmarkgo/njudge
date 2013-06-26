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

#endif /* USER_H_ */
