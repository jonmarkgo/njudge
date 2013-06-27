/*
 * player.h
 *
 *  Created on: Dec 25, 2012
 *      Author: cjw
 */

#ifndef PLAYER_H_
#define PLAYER_H_

typedef struct plrstat_s {

	guint  player_id;
	gint   dedication;
	time_t last_logon;
	guint  total;
    guint  ontime;
    guint  started;
    guint  resigned;
    guint  tookover;

} plrstat_t;

#endif /* PLAYER_H_ */
