/*
 * $Log$
 * Revision 1.29  2004/07/25 16:13:43  millis
 * Bug fixes for Bug 91 (Duplex powers), Bug 233 (Abandoned power cannot
 * return in duplex) and Bug 206 (allow takeover of unknown abandoned
 * countries)
 *
 * Revision 1.28  2004/07/04 03:25:24  millis
 * Fix bug 328, implement Portage variant
 *
 * Revision 1.27  2004/06/27 01:50:21  millis
 * Futher Intimate fixes (Bug 297) specifically to allow phased orders
 * and correct turns not processing, plus more information printed.
 *
 * Revision 1.26  2004/06/16 07:54:50  nzmb
 * A couple of changes to improve the interaction between rdip and bailout, and
 * make rdip a bit more fault tolerant:
 * (1) rdip now handles SIGPIPE and bails the judge out gracefully should it
 *     occur (previously, things died in mysterious and unpleasant ways).
 * (2) Fixed segfaults that occured when rdip calls bailout.
 *
 * Revision 1.25  2004/05/22 10:31:33  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.23  2004/01/04 11:34:35  millis
 * Implement Bug #262 (ExtraCentres for 1900 Steamroller)
 *
 * Revision 1.22  2003/09/14 08:25:13  millis
 * Fix bug 225
 *
 * Revision 1.21  2003/07/15 22:47:06  millis
 * Fix Bug 185 (call smail for each email individually)
 *
 * Revision 1.20  2003/06/29 21:37:42  nzmb
 * Made EOG draw entries broadcasted at the end of the game.
 *
 * Revision 1.19  2003/06/22 04:10:19  nzmb
 * Added code to allow users to record diary entries, read them, and delete them
 * if they make a mistake. The diaries will be broadcast when the games end (this
 * part is not as of now, yet written).
 *
 * Revision 1.18  2003/05/03 23:09:47  millis
 * added missing prototype for ExtraCentres()
 *
 * Revision 1.17  2003/05/02 21:30:03  millis
 * Added an artilery support funciton
 *
 * Revision 1.16  2003/02/17 15:39:20  millis
 * bug 10, prevent overlapping absences
 * Also improve text output for absences.
 *
 * Revision 1.15  2003/01/13 22:29:48  millis
 * Merged from ustv
 *
 * Revision 1.14  2002/12/28 00:02:54  millis
 * Fixed bug 77, adding wrap_char() function
 *
 * Revision 1.13  2002/11/08 21:51:19  millis
 * Resolve bug 36
 *
 * Revision 1.12  2002/07/16 18:14:21  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.11  2002/05/04 02:06:17  nzmb
 * Added code to display the time left until the deadline and grace at the
 * bottom of their reply whenever a player signs on.
 *
 * Revision 1.10  2002/04/18 04:44:31  greg
 * Added the following commands:
 * - unstart
 * - set secret
 * - set [prflist|prfrand|prfboth]
 *
 * Fixed Set Absence so that "to" is not case sensitive
 *
 * Fixed Quiet games so that new players are announced
 * before the game starts
 *
 * Fixed ascii_to_ded.c so thatit no longer generates an
 * error when compiled
 *
 * Revision 1.9  2002/04/15 12:55:42  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.8  2002/02/25 11:51:50  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.7  2001/10/20 12:11:11  miller
 * Merged in changes from DEMA and USTV
 *
 * Revision 1.6.2.2  2001/10/20 00:52:05  dedo
 * Add missing prototypes
 *
 * Revision 1.6.2.1  2001/10/19 23:31:42  dema
 * Added new Mach fundtion prototype + CVS header
 *
 *
 */    

#include <stdio.h>
#include <time.h>

/* Not included by stdlib.h for some reason */
void  exit(int);

/* needed for sequence typedef */
#include "dip.h"

/* a comprehensive list of function prototypes */

/* defined in mail.c */
int mail(void);
void msg_header(FILE * fp);
void open_press(void);
void mail_reply(int err);
void send_press(void);

/* defined in ml_short.c */
int mail_shorthand(char *text);

/* defined in lib.c */
char *NowString(void);
void archive(char *file, char *subject);
int execute(char *command);
void ferrck(FILE * fp, int n);
int power(char c);
void wrap(FILE * fp, char *buf, int pos, int indent);
void wrap_char(FILE * fp, char *buf, int pos, int indent, char w_char);
char *autype(char c);
char *utype(char c);
char *Stype(char c);
char *astype(char c, char cu);
char *bstype(char c);
void die_rolls(int seed);
int die(int n, int s);
char *strcap();
char *Utype();
char *mov_type(int prov_index, int unit_index);
int FindPower(int);
int GetOneWord(char *text);
int despace(char *intext);
void display_absence(int pnum, FILE *fptr);
void AddCoastToOrder(char *text, int c1);
void AddUnitToOrder(char *text, int unit);
void AddPlaceToOrder(char *text, int place);
void AddProvinceToOrder(char *text, int place);
void AddUnitProvinceToOrder(char *text, int unit, int place);
void AddPowerToOrder(char *text, int power);
void AddOrderToOrder(char *text, int order);
char *lookforv(char *l, char *w[], int len, int *n, int exact_word);
void InformCustodians( char *game_name, char *text, int variant, int is_gunboat);
int ValidGatewayProvince(int igw, int p);
int ValidRailwayProvince(int irw, int p, int *pr_index);
int PermittedRailwayPower(int irw, char power_letter);
int IsGatewayOrder(int u, int *igw);
int IsGatewayOrdered(int igw, int u, int result[]);
char HasUnit(int province_index);
int AllowedGatewayRetreat(int u, int p);
int GetNativeIndex(void);
int IsMultiProvince(int p);
int CheckForMultiUnitProvinces(void);
int GetUnitIndex(int p, int power);
void MailOut(char *out_line, char *address);
char *get_coast(char * coast_string, int *coast);
void PrintTwoColTable( char * title, char *power_col, char *other_col );
int PowerControlledBy( int p);
char *SomeoneText( int index);
int RealPlayerIndex(int index);
int IsPlayerDead(int index);
void PrintTreasury(int pt, int power_bid_total[], int processing, int predict);
int LandLocked(int p);	

/* defined in bailout.c */
void real_bailout(int, char *, int, int);
#define bailout(btype) real_bailout(btype, __FILE__, __LINE__, 1)
#define rdip_bailout(btype) real_bailout(btype, __FILE__, __LINE__, 0)

/* void bailout(int); */

/* defined in dip.c */
void master(void);
void savemail(void);
int process(void);
int deadline(sequence * seq, int new);

/* defined in porder.c */
int porder(char c, int player, int listflg);
void income(int mindie);

/* defined in dipent.c */
int getdipent(FILE * fp);
void putdipent(FILE * fp, int dopw);
void testdipent(int seq, int variant);
void newdipent(char *name, int variant);
int getseq(FILE * fp, char *line, sequence * seq);
void putseq(FILE * fp, char *s, sequence * seq);
int countgames(void);

/* defined in jm.c */
int jm(char *p, long *t);
char *ptime(time_t * t);
char *abs_time(time_t * t);
char *timeleft(time_t *deadline);

/* defined in po_errmsg.c */
void errmsg(char *fmt,...);
void errputc(char c, FILE * fp);
void junk(char **stream);
int preprocess(char **s, int *p);
int canpreprocess(char *s);

/* defined in st_porder.c */
int process_output(int pt, char phase);
void process_input(int pt, char phase, int player);
int ownership(int new_type, int *int_status);

/* defined in st_bid.c */
int bidin(char **s, int p, int syntaxcheck);
void bidout(int pt);
int bid_syntaxcheck(char *s, int precheck, char *out_string);


/* defined in st_build.c */
int CountCentres( int p );
int buildin_td(char **s, int p);
int buildin(char **s, int p);
void init_build(void);
void buildout_td(int pt);
void buildout(int pt);
int build_syntaxcheck(char *s, int precheck, char *out_string);
int ExtraCentres( int power );

/* defined in st_setup.c */
int setupin(char **s, int p);
void setupout(int pt);

/* defined in st_retreat.c */
int retreatin(char **s, int pt);
void retreatout(int pt);
int retreat_syntaxcheck(char *s, int precheck, char *out_string);

/* defined in st_movement.c */
int movein(char **s, int p);
int moveout(int pt);
int move_syntaxcheck(char *s, int precheck, char *out_string);

/* defined in common.c */
void next_year(void);
void init_movement(void);
void init_retreats(void);
int valid_move(int u, int p, int *c, int *b);
int valid_artillery_move(int u, int p, int *c, int *b);

/* defined in po_mastrpt.c */
void mast_rpt(int current_power, int line_up);

/* defined in ma_stats.h */
void balance(int pt, int next, int listflg);
void ma_ownership(void);

/* defined in function mfprintf.c */
void mfprintf(FILE * the_file, char *fmt,...);

/* defined in ml_date.c */
#define DT_ABS_START 1
#define DT_ABS_END 2
int mail_date(char **p, long *date, int past, FILE * rfp, int date_type);

/* defined in machlib.c */
int has_fortress(int prov);
int has_fortcity(int prov);
void set_rebellion(int prov);
int GetInitialMoney(int power, int *money);
int PermittedMachUnit(int power, char type, char stype, int mode);

/* defined in ma_expenses.c */
int testword(char **stream, char *word);
int expin(char **s, int p);
void expout(int pt);

/* defined in ma_porder.c */
int victory(void);
void ma_process_input(int pt, char phase, int player);
int ma_process_output(int pt, char phase);

/* defined in ma_famplag.c */
void fam_plag(int t);

/* defined in ma_build.c */
int ma_buildin(char **s, int p);
void ma_buildout(int pt);

/* defined in ma_build_basic.c */
int MachCheckOwnedOKBasic( char type, int u, int p, int p1, int *c1);
int ma_init_build_basic(void);
int build_syntaxcheck_basic(char *in_text, int precheck, char *out_string);
int ma_buildin_basic(char **s, int p);
void ma_buildout_basic(int pt);

/* defined in ma_movement.c */
int ma_movein(char **s, int p);
int ma_moveout(int pt);

/* defined in ma_retreat.c */
int ma_retreatin(char **s, int p);
void ma_retreatout(int pt);

/* defined in params.c */
void params(FILE * fp);
char *printlevel(int level);

/* defined in users.c */
int cmpaddr(char *addr, char *list);
int setsite(char *s);
void send_package(char *addr);
void whois(char *t);
int new_checklist(FILE * fp, char *addr);
int is_allowed(int type_flag);
int is_disallowed(int type_flag);
int send_dedication(char *raddr);
int newuser(char *addr, FILE * fp);
int iamalso(char *addr, char *oldaddr);
void whogame(int f);
void add_player(char *player_name, char *file, char addflag);
int getuser(char *addr, int *userid, int *siteid, int *level);

/* defined in po_init.c */
int po_init(void);
int gamein(void);
int gameout(void);
void po_chkmov(void);
void UpdateBlockades(void);

/* defined in st_status.c */
void status(int pt);

/* defined in po_condition.c */
int condition(char **s);

/* defined in ml_getaddr.c */
int mail_getaddr(char *s, char *addr);

/* defined in ml_signon.c */
int mail_signon(char *s);
void mail_igame(void);
int chkpref(char *s, int wp[], int wv[]);
void SetupIntimateTreasury(void);

/* defined in ml_list.c */
void mail_listit(void);
void mail_listbrief(void);

/* defined in version.c */
void version(FILE * rfp);

/* defined in history.c */
int history(char *line, int power_type);

/* defined in ml_set.c */
void ShowPreferences(FILE *);
void mail_setp(char *s);
void process_allowdeny(char **info, char *basename);

/* defined in ml_press.c */
void mail_press(char *s, int need_opts);
int parse_powers(char **s, char *list, size_t size, int *count, int put_output);
void list_powers(char *line, char *list);

/* defined in po_get.c */
char *get_power(char *l, int *o);

/* defined in phase.c */
int phase(char *s);
int phase_syntax(int phase, char *s);

/* defined in draw.c */
int check_can_vote(int i, int flag); /* Change Aug 28, 2001 */
int chkdraw(char *to_check);
int process_draw(void);
/* Tim Miller's concession stuff */
int chkconc(char *to_check);
int process_conc(void);

/* defined in assign.c */
void assignment(int a[][], int n, int m, int x[]);

/* defined in strcasecmp.c */
#if 0
int strcasecmp(char *s1, char *s2);
int strncasecmp(char *s1, char *s2, register int n);
#endif

/* defined in tm_xpress.c */
int process_ppress(void);
void process_diary(char *cmd);
void send_diary(void);
