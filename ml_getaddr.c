/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  mail_getaddr.c -- process an email return address
   **
   **  Copyright 1987, Lowe.
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the 
   **  above notices are preserved and that due credit is given to Mr.
   **  Lowe.
   **
 */

#include "dip.h"
#include "mail.h"
#include "functions.h"

#include <string.h>

/***************************************************************************/

int mail_getaddr(char *s, char *addr)
{

/*
 *  Extract a mailing address off of the input line.  The address can be
 *  in either of the following formats:
 *
 *       name <id@domain>      -or-   id@domain (name)
 *
 *  In either case, the name is ignored.
 */

	char c, *t, buf[100], line[150];
	char *percent, *at, *bang;
	int period, period1, quote;


	period1 = period = quote = 0;
	at = bang = percent = NULL;
	t = buf;
	while (*s && *s != '\n') {

		if (t >= buf + sizeof(buf))
			return E_WARN;

		switch (c = *s++) {
		case ' ':
		case ';':
		case '*':
		case '?':
			break;

		case '"':	/* Ignore garbage between quote marks */
			if (quote)
				quote = 0;
			else {
				char *r;
				for (r = s; *s && *s != '"'; s++) {
					if (*s == '!')
						quote = 1;	/* "host!user (name)"@domain */
				}
				if (*s != '"' || (*++s == '@' && quote))
					s = r;
				else
					quote = 0;
			}
			break;

		case '<':	/* name <id@domain> */
			at = bang = percent = 0;
			t = buf;
			break;

		case '>':	/* name <id@domain> */
			*t++ = '\0';
			break;

		case '(':	/* id@domain (name) */
			c = 1;
			while (*s && c) {
				if (*s == ')')
					c--;
				else if (*s == '(')
					c++;
				s++;
			}
			break;

		default:
			if (c == '@') {
				at = t;
				period = period1;
				period1 = 0;
			} else if (c == '%') {
				percent = t;
				period1 = 0;
			} else if (c == '!') {	/* UUCP host!user       */
				bang = t;
			} else if (!*addr && c == ':') {	/* DECNET host::user    */
				bang = t;
			}
			if (c == '.')
				period1++;
			*t++ = c;
		}
		*t = '\0';
	}

	/*
	 * Validate the address.  It must have nonzero length and it can't be to
	 * ourselves or any of the other known mailers.  If we've already got an
	 * address that contains an '@' or a '!' and this one doesn't, we use the
	 * previous one.
	 */

	if (t == buf || (!at && !bang && *addr)) {
		moreaddr++;
		return E_WARN;
	}
	t = bang ? bang + 1 : buf;
	if (!strncasecmp(t, "mailer", 6) ||
	    !strncasecmp(t, "mail-daemon", 11) ||
	    !strncasecmp(t, "root", 4) ||
	    !strncasecmp(t, "uucp", 4) ||
	    !strncasecmp(t, "mmdf", 4) ||
	    !strncasecmp(t, "smtp", 4) ||
	    !strncasecmp(t, "postma", 6) ||
	    !strncasecmp(t, "portal_mail", 11) ||
	    !strncasecmp(t, "tnc_mailer", 10) ||
	    !strncasecmp(t, "udb@cs.utex", 11) ||
	    !strncasecmp(t, "-MaiSer-", 8) ||
	    !strncasecmp(t, OURSELVES, strlen(OURSELVES))) {

		*addr = '*';	/* Ensure that we don't reply */

		fprintf(log_fp, "Ignoring mail from funny address.\n");

		bang = 0;
		while (fgets(line, sizeof(line), stdin)) {
			fputs(line, log_fp);
			fputs(line, ifp);
			if (!strcmp(line, "Subject: Returned mail: Return receipt\n"))
				bang++;
		}

		if (!bang) {
			if ((tfp = fopen("dip.ignore", "a"))) {
				fflush(ifp);
				rewind(ifp);
				while (fgets(line, sizeof(line), ifp))
					fputs(line, tfp);
				fputs("\n", tfp);
			} else {
				perror("dip.ignore");
				fprintf(log_fp, "Unable to open ignore file.\n");
				bailout(1);
			}
		}
		return E_WARN;

	}
	/*
	 * Pretty up the address a bit.  We can get to everywhere that our
	 * BITNET gateway can get to, so remove the extra garbage that it
	 * throws on to the end of addresses.
	 */

	if (percent && at && (!strcmp(at, BITNET_GATEWAY1) ||
			      !strcmp(at, BITNET_GATEWAY2) ||
			      !strncasecmp(at - 7, ".bitnet", 7))) {
		*percent = '@';
		if (!period)
			strcpy(at, ".BITNET");
		else
			*at = '\0';
		at = percent;
	}
#if 0
	/*
	 * If there are no periods in the address there's a pretty good chance
	 * that this is a bitnet address and needs a little help.  This assumes
	 * that all the local hosts fully qualify their addresses in the headers.
	 */

	for (t = at; t && *t; t++)
		if (*t == '.' || *t == '!')
			break;

	if (t && !*t) {
		strcpy(t, ".BITNET");
	}
#endif

	/*
	 * Fix up those British people who drive on the wrong side of the road.
	 */

	if (at && !strncasecmp(at, "@uk.ac", 6)) {
		*at++ = '\0';
		strcpy(addr, buf);
		strcat(addr, "@");
		while ((t = strrchr(at, '.'))) {
			strcat(addr, t + 1);
			strcat(addr, ".");
			*t = '\0';
		}
		strcat(addr, at);
	}
	/*
	 * Fix up those French people who can't keep their mailers operational.
	 */

	else if (at && (t = strchr(at, '.')) && !strncasecmp(t, ".greco-prog.fr", 14)) {
		*at = '\0';
		sprintf(addr, "%s@geocub.greco-prog.fr", buf);
	}
	/*
	 * Otherwise we simply copy the address across.
	 */

	else {
		strcpy(addr, buf);
	}
	return 0;

}


/***************************************************************************/
