/*
 * $Log$
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 *
 */

/*  porder.c

 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 *
 * Switch to different processing for Machiavelli and other variants
 * A. Verweij, 1994.
 */

#include "dip.h"
#include "porder.h"
#include "functions.h"

int porder(char c, int player, int listflg)
{
	int power;

	if (po_init() || gamein()) {
		fprintf(stderr, "%d errors during initialization.\n", err);
		fprintf(rfp, "%d errors during initialization.\n", err);
		return E_FATAL;
	}
	power = player != -1 ? dipent.players[player].power : listflg ? OBSERVER : 0;

	if (listflg)
		status(power);

	if (power < NPOWER + 1 || power == MASTER) {

		sprintf(line, "%s%s/%c%s", GAME_DIR, dipent.name, c, dipent.seq);
		if (!(ifp = fopen(line, "r")))
			ifp = fopen("/dev/null", "r");

		if (!ifp) {
			perror(line);
			fprintf(rfp, "Cannot open input or output files in porder.\n");
			return E_FATAL;
		}
		processing = player == -1;
		more_orders = 0;

		if (dipent.flags & F_MACH) {
			ma_process_input(power, dipent.phase[5]);
			ma_process_output(power, dipent.phase[5]);
		} else {
			process_input(power, dipent.phase[5]);
			process_output(power, dipent.phase[5]);
		}

		fclose(ifp);
	}
	if (!err && !power && !listflg)
		err = gameout();
	return err;
}
