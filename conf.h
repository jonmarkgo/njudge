/*
 * $Log$
 * Revision 1.8  2003/01/13 22:29:48  millis
 * Merged from ustv
 *
 * Revision 1.7  2002/12/22 02:00:34  millis
 * Added DIE_STORMS setting for new random seed for storms
 *
 * Revision 1.6  2002/04/15 12:55:41  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.5  2001/07/17 09:55:31  greg
 * added GAME_DIR
 *
 * Revision 1.4  2001/07/08 22:57:07  miller
 * New defines
 *
 * Revision 1.3  2001/01/06 18:48:43  davidn
 * Changes to allow game creator to be automatically made master
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added new dip.conf variable. Plus allowed dip.conf path to be passed in as a parameter
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.2  1996/11/05 22:49:23  rpaar
 * NW's changes
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#define JUDGE_PATH      config("JUDGE_PATH")
#define SMAIL_CMD	config("SMAIL_CMD")
#define ATRUN_CMD	config("ATRUN_CMD")
#define SUMMARY_CMD	config("SUMMARY_CMD")
#define RUNLISTMAP_CMD	config("RUNLISTMAP_CMD")
#define	RUNDIPMAP_CMD 	config("RUNDIPMAP_CMD")
#define DIP_CMD		config("DIP_CMD")
#define GAMES_MASTER    config("GAMES_MASTER")
#define GAMES_OPENER	config("GAMES_OPENER")
#define HALL_KEEPER     config("HALL_KEEPER")
#define BN_CUSTODIAN    config("BN_CUSTODIAN")
#define MN_CUSTODIAN    config("MN_CUSTODIAN")
#define EP_CUSTODIAN    config("EP_CUSTODIAN")
#define OURSELVES       config("OURSELVES")
#define BITNET_GATEWAY1 config("BITNET_GATEWAY1")
#define BITNET_GATEWAY2 config("BITNET_GATEWAY2")
#define FORWARD         config("FORWARD")
#define XFORWARD        config("XFORWARD")
#define YFORWARD        config("YFORWARD")
#define KEEPOUT         config("KEEPOUT")
#define SAVE_FILE       config("SAVE_FILE")
#define PLAN		    config("PLAN")
#define BAILOUT_PLAN    config("BAILOUT_PLAN")
#define BAILOUT_MSG     config("BAILOUT_MSG")
#define RGD_GATEWAY     config("RGD_GATEWAY")
#define MIN_CONTROL     atoi(config("MIN_CONTROL"))
#define LOCK_TIMEOUT    atoi(config("LOCK_TIMEOUT"))
#define JUDGE_CODE      config("JUDGE_CODE")
#define SPECIAL_PW	config("SPECIAL_PW")
#define D_ONTIME        atoi(config("D_ONTIME"))
#define D_LATE          atoi(config("D_LATE"))
#define D_ABANDON       atoi(config("D_ABANDON"))
#define D_CD            atoi(config("D_CD"))
#define MAP_FILE        config("MAP_FILE")
#define MASTER_FILE     config("MASTER_FILE")
#define TMASTER_FILE	config("TMASTER_FILE")
#define NO_CREATE       config("NO_CREATE")
#define AUTO_MASTER     config("AUTO_MASTER")
#define LOG_FILE        config("LOG_FILE")
#define CUTOFF_LENGTH   atoi(config("CUTOFF_LENGTH"))
#define DIE_ASSASSIN    atoi(config("DIE_ASSASSIN"))
#define DIE_EXPENSE     atoi(config("DIE_EXPENSE"))
#define DIE_FAMPLAG     atoi(config("DIE_FAMPLAG"))
#define DIE_STORMS	atoi(config("DIE_STORMS"))
#define DIE_INCOME      atoi(config("DIE_INCOME"))
#define MAXGAMES	atoi(config("MAXGAMES"))
#define SYSLOG_FLAG	atoi(config("SYSLOG_FLAG"))
#define TIME_TOLERANCE  atoi(config("TIME_TOLERANCE"))
#define WARP_FILE	config("WARP_FILE")
#define GAME_DIR	config("GAME_DIR")
#define BLIND_CMD       config("BLIND_CMD")
#define NOBODY		config("NOBODY")
#define BLOCK_FILE	config("BLOCK_FILE");
#define JUDGE_TZ	config("JUDGE_TZ");

/* this is the location of the judge configuration file,
 * it should probably be a full path name, as i don't think
 * that the initial working directory is known, and the function
 * that uses this may be called before the working directory is
 * set to a sane value.
 */
extern char * CONFIG_FILE;
/* Now defined as a command line opton '-C' */
extern char * CONFIG_DIR;

void conf_usage(void);
void conf_cmdline(int argc, char **argv);
void conf_readfile(char *directory, char *filename);
void conf_print(FILE * where);
int conf_init(void);

int conf_set(char *var, char *val);
char *config(char *var);
extern struct pd_ht conf_table;
