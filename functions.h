#include <stdio.h>
#include <time.h>

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

/* defined in bailout.c */
void real_bailout(int, char *, int);
#define bailout(btype) real_bailout(btype, __FILE__, __LINE__ )

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

/* defined in po_errmsg.c */
void errmsg(char *fmt,...);
void errputc(char c, FILE * fp);
void junk(char **stream);
int preprocess(char **s, int *p);
int canpreprocess(char *s);

/* defined in st_porder.c */
int process_output(int pt, char phase);
void process_input(int pt, char phase);
int ownership(void);

/* defined in st_build.c */
int buildin(char **s, int p);
void init_build(void);
void buildout(int pt);
int build_syntaxcheck(char *s, int precheck, char *out_string);

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

/* defined in po_mastrpt.c */
void mast_rpt(int current_power, int line_up);

/* defined in ma_stats.h */
void balance(int pt, int next, int listflg);
void ma_ownership(void);

/* defined in function mfprintf.c */
void mfprintf(FILE * the_file, char *fmt,...);

/* defined in ml_date.c */
int mail_date(char **p, long *date, int past, FILE * rfp);

/* defined in machlib.c */
int has_fortress(int prov);
int has_fortcity(int prov);
void set_rebellion(int prov);

/* defined in ma_expenses.c */
int testword(char **stream, char *word);
int expin(char **s, int p);
void expout(int pt);

/* defined in ma_porder.c */
int victory(void);
void ma_process_input(int pt, char phase);
int ma_process_output(int pt, char phase);

/* defined in ma_famplag.c */
void fam_plag(int t);

/* defined in ma_build.c */
int ma_buildin(char **s, int p);
void ma_buildout(int pt);

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
int check_can_vote(int i);
int chkdraw(char *to_check);
int process_draw(void);

/* defined in assign.c */
void assignment(int a[][], int n, int m, int x[]);

/* defined in strcasecmp.c */
#if 0
int strcasecmp(char *s1, char *s2);
int strncasecmp(char *s1, char *s2, register int n);
#endif
