/*
 * $Log$
 * Revision 1.2  1996/11/05 22:49:23  rpaar
 * NW's changes
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

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
#define D_ONTIME        atoi(config("D_ONTIME"))
#define D_LATE          atoi(config("D_LATE"))
#define D_ABANDON       atoi(config("D_ABANDON"))
#define D_CD            atoi(config("D_CD"))
#define MAP_FILE        config("MAP_FILE")
#define MASTER_FILE     config("MASTER_FILE")
#define NO_CREATE       config("NO_CREATE")
#define LOG_FILE        config("LOG_FILE")
#define CUTOFF_LENGTH   atoi(config("CUTOFF_LENGTH"))
#define DIE_ASSASSIN    atoi(config("DIE_ASSASSIN"))
#define DIE_EXPENSE     atoi(config("DIE_EXPENSE"))
#define DIE_FAMPLAG     atoi(config("DIE_FAMPLAG"))
#define DIE_INCOME      atoi(config("DIE_INCOME"))

/* this is the location of the judge configuration file,
 * it should probably be a full path name, as i don't think
 * that the initial working directory is known, and the function
 * that uses this may be called before the working directory is
 * set to a sane value.
 */

#define CONFIG_FILE "/home/judge/dip/dip.conf"

void conf_usage(void);
void conf_cmdline(int argc, char **argv);
void conf_readfile(char *filename);
void conf_print(FILE * where);
int conf_init(void);

int conf_set(char *var, char *val);
char *config(char *var);
extern struct pd_ht conf_table;
