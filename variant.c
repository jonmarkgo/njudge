#include <stdlib.h>

#include "dip.h"
#include "variant.h"

static int max_country_name_len[NVARIANT];
static int first_pass = 1;

/*
 * This function returns the largest name for a particular variant
 * Only looking at powers active for the variant in question
 */
int GetMaxCountryStrlen()
{
    int n, i = 0;
    if (first_pass)
    {
	first_pass = 0;
	for (n = 0; n > NVARIANT; n++)
	    max_country_name_len[n] = 0;
    }

    if (max_country_name_len[dipent.variant] !=0 ) return max_country_name_len[dipent.variant];

    while( strcmp(powers[++i], "Master"))
    {
        if (pletter[dipent.variant][i] != 'x')
	    if (strlen(powers[i]) > max_country_name_len[dipent.variant])
	        max_country_name_len[dipent.variant] =  strlen(powers[i]);
    }
    max_country_name_len[dipent.variant]++;
    if (max_country_name_len[dipent.variant] < 10)
	max_country_name_len[dipent.variant] = 10;
    return max_country_name_len[dipent.variant];
}

char *variants[] =
{
	NULL,			/* Don't use 0 as a variant number */
	"standard",		/* Standard variant                */
	"youngstown",		/* Youngstown variant              */
	"loeb9",		/* Loeb 9 player variant           */
	"pure",			/* Pure variant                    */
	"chaos",		/* 34 player nonsense              */
	"britain",		/* Great Britain variant           */
	"1898",			/* 1898 variant                    */
	"crowded",		/* Eleven player variant           */
	"machiavelli",		/* Machiavelli 1454                */
	"fleet_rome",		/* Standard with a fleet in rome   */
	"shift_right",		/* Shift right variant             */
	"shift_left",		/* Shift left variant              */
	"rootz",		/* Root Z Diplomacy                */
	"aberration",		/* The way things might have been  */
	"wraparound",		/* Doughnut shaped nonsense        */
	"chromatic",		/* 5-Italy version with colours    */
	"milan",		/* Milan Diplomacy                 */
	"asia",			/* Asian Diplomacy                 */
	"modern",		/* Modern Diplomacy                */
	"colonial",		/* Colonial Diplomacy              */
	"h31",			/* Hundred Years War Diplomacy     */
	"h32",			/* Hundred Years War Diplomacy     */
	"classical",		/* Classical Medit. Diplomacy      */
	"empire",		/* Empire			   */
	"african2",		/* African 2			   */
	"wraparound2",		/* wraparound2			   */
	"shift_around",		/* Shift around			   */
	"layered",		/* Layered			   */
	"sailho_crowded",	/* Sailhoi, crowded		   */
	"sailho",		/* Sailho, 			   */
	/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	"gunboat",		/* Gunboat (anonymous) option      */
	"blind",		/* Blind movement option           */
        "shorthand",		/* Shorthand press option          */
	"wings" /* , */		/* Wings (for modern)		   */
	/* "a/f" */		/* AirForces option		   */
};

int vvalue[] =
{
	NO_VARIANT,		/* Don't use 0 as a variant number */
	V_STANDARD,		/* Standard variant                */
	V_youngstown,		/* Youngstown variant              */
	V_loeb9,		/* Loeb 9 player variant           */
	V_pure,			/* Pure variant                    */
	V_chaos,		/* 34 player nonsense              */
	V_britain,		/* Great Britain variant           */
	V_1898,			/* 1898 variant                    */
	V_crowded,		/* Eleven player variant           */
	V_machiavelli,		/* Machiavelli 1454                */
	V_fleet_rome,		/* Standard with a fleet in rome   */
	V_shift_right,		/* Shift right variant             */
	V_shift_left,		/* Shift left variant              */
	V_rootz,		/* Root Z Diplomacy                */
	V_aberration,		/* The way things might have been  */
	V_wraparound,		/* Doughnut shaped nonsense        */
	V_chromatic,		/* 5-Italy version with colours    */
	V_milan,		/* Milan Diplomacy                 */
	V_asia,			/* Asian Diplomacy                 */
	V_modern,		/* Modern Diplomacy                */
	V_colonial,		/* Colonial Diplomacy              */
	V_h31,			/* Hundred Years War Diplomacy     */
	V_h32,			/* Hundred Years War Diplomacy     */
	V_classical,		/* Classical Medit. Diplomacy      */
        V_empire,               /* 24 Empire version                 */
        V_african2,             /* 25 African2                       */
        V_wraparound2,          /* 26 2nd version of wraparound      */
        V_shift_around,         /* 27 Shift around verison           */
        V_layered,              /* 28 Layered                        */
        V_sailho_crowded,       /* 29 SailHo crowded                 */
        V_sailho,               /* 30 SailHo                         */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	F_GUNBOAT,		/* Gunboat (anonymous) option      */
	F_BLIND,		/* Blind movement option           */
        F_SHORTHAND,		/* Shorthand press option          */
	F_WINGS /* , */		/* Wings (for modern) variant      */
	/* F_AFRULES  */        /* Army/Fleet Rules variant        */
};

/*  Power names.  */

char *powers[] =
{
	"Unowned",
	/* POWER LIST BEGINS HERE */
	"Austria","Balkan","Britain","Burgundy","Byzantium","China",
	"Egypt","Eire","England","Florence","France","Germany",
	"Hungary","India","Indonesia","Israel","Italy","Japan",
	"Lowland","Milan","Naples","Norway","Papacy","Persia",
	"Poland","Russia","Siberia","Sicily","Spain","Turkey",
	"Ukraine","Venice","Holland",
	"Austria1","Austria2","England1","England2","France1","France2",
	"Germany1","Germany2","Italy1","Italy2","Russia1","Russia2",
	"Turkey1","Turkey2",
	"Yellow","Dark","Light","Red","Blue",
	"A-Ank","B-Bel","C-Ber","D-Bre","E-Bud","F-Bul",
	"G-Con","H-Den","I-Edi","J-Gre","K-Hol","L-Kie",
	"N-Lvp","P-Lon","Q-Mar","R-Mos","S-Mun","T-Nap",
	"U-Nor","V-Par","W-Por","X-Rom","Y-Rum","Z-Ser",
	"0-Sev","1-Smy","2-Spa","3-StP","4-Swe","5-Tri",
	"6-Tun","7-Ven","8-Vie","9-War",
	"BC","California","Mexico","Florida","Heartland","NewYork",
	"Peru","Quebec","Texas","Cuba",
	"Ethiopia","Ivory-Coast","Libya","Saudi-Arabia","South-Africa","Zaire",
	"North","South","East","West","Cyprus","Dissentos",
	"Syria","Carthage","Macedon","Rome",
	//** ADD NEW POWERS ABOVE THIS LINE! **/
	"a power","Autonomous","Observer","Master"};

/*  Owner names (i.e., possessive form of power names).  */

char *owners[] =
{
	"unowned",
	"Austrian","Balkan","British","Burgundian","Byzantine","Chinese",
	"Egyptian","Irish","English","Florentine","French","German",
	"Hungarian","Indian","Indonesian","Israelite","Italian","Japanese",
	"Lowland","Milanese","Naplese","Norwegian","Papal","Persian",
	"Polish","Russian","Siberian","Sicilian","Spanish","Turkish",
	"Ukranian","Venetian","Dutch",
	"1Austrian","2Austrian","1English","2English","1French","2French",
	"1German","2German","1Italian","2Italian","1Russian","2Russian",
	"1Turkish","2Turkish",
	"Yellow","Dark","Light","Red","Blue",
	"A-Ank","B-Bel","C-Ber","D-Bre","E-Bud","F-Bul",
	"G-Con","H-Den","I-Edi","J-Gre","K-Hol","L-Kie",
	"N-Lvp","P-Lon","Q-Mar","R-Mos","S-Mun","T-Nap",
	"U-Nor","V-Par","W-Por","X-Rom","Y-Rum","Z-Ser",
	"0-Sev","1-Smy","2-Spa","3-StP","4-Swe","5-Tri",
	"6-Tun","7-Ven","8-Vie","9-War",
	"British Columbian","Californian","Mexican","Floridian","Heartlander","New Yorker",
	"Peruvian","Quebecois","Texan","Cuban",
	"Ethiopian","Ivory-Coastish","Libyan","Saudi-Arabian","South-African","Zairean",
	"Northern","Southern","Eastern","Western","Cyprus","Dissentos",
	"Syrian","Carthaginian","Macedonian","Roman",
	/** ADD NEW POWERS ABOVE THIS LINE! **/
	"??????","Autonomous","Observer","Master"};

/*  Power abbreviations based on variant ('x' means corresponding power isn't
   in this variant).  The last five characters should always be '?xOM', except
   for Machiavelli where it's '?&OM'.  Likewise, 'O' and 'M' should not be
   used for anything other than Observer and Master, respecitively.  */

char *pletter[] =
{
    NULL,                               /* Don't use 0 as a variant number */
/** ADD NEW POWERS TO EACH LINE! **/
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Standard variant              */
    ".AxxxxCxxExFGxNxxIJxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Youngstown variant            */
    ".AxxxxxxxExFGxxxxIxxxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Loeb 9 player variant         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Pure variant                  */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxABCDEFGHIJKLNPQRSTUVWXYZ0123456789xxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* 34 player nonsense            */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Great Britain variant         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* 1898 variant                  */
    ".ABxxxxxxExFGxxxxIxLxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Eleven player variant         */
    ".AxxxxxxxxLFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?&OM",                          /* Machiavelli 1454              */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Standard with a fleet in rome */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Shift right variant           */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Shift left variant            */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxA1E2F3G4I5R6T7xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Root Z Diplomacy              */
    ".xxxBZxxExxxxHxxIxxxxxxxxPxxCSxUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* The way things might have been*/
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Doughnut shaped nonsense      */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxYDLRBxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* 5-Italy version with colours  */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Milan Diplomacy               */
    ".xxxxxCxxxxxxxNIxxJxxxxxPxRSxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Asian Diplomacy               */
    ".xxBxxxExxxFGxxxxIxxxxxxxPRxxSTUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Modern Diplomacy              */
    ".xxBxxCxxxxFxxxxxxJxxxxxxxRxxxTxxHxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Colonial Diplomacy            */
    ".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Hundred Years War Diplomacy   */
    ".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* H31 Diplomacy                 */
    ".xxxxxxExxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxSCAR?xOM",                          /* Classical Diplomacy           */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxBCXFHNPQTUxxxxxxxxxxxxxxxx?xOM",                          /* Empire4 Diplomacy             */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxEILSAZxxxxxxxxxx?xOM",                          /* African2 Diplomacy            */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Wraparound2 Diplomacy         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Shift_Around Diplomacy        */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",                          /* Layered Diplomacy             */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxNSEWCDxxxx?xOM",                          /* Sailho2_Crowded Diplomacy     */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxNSEWxxxxxx?xOM",                          /* SailHo2 Diplomacy             */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
};

/*  Starting phase.  */

char *sphase[] =
{
	NULL,			/* Don't use 0 as a variant number */
	"S1901M",		/* Standard variant                */
	"S1901M",		/* Youngstown variant              */
	"S1901M",		/* Loeb 9 player variant           */
	"S1901M",		/* Pure variant                    */
	"F1900B",		/* 34 player nonsense              */
	"S1901M",		/* Great Britain variant           */
	"S1899M",		/* 1898 variant                    */
	"S1901M",		/* Eleven player variant           */
	"F1453A",		/* Machiavelli 1454                */
	"S1901M",		/* Standard with a fleet in rome   */
	"S1901M",		/* Shift right variant             */
	"S1901M",		/* Shift left variant              */
	"S1901M",		/* Root Z Diplomacy                */
	"S1901M",		/* The way things might have been  */
	"S1901M",		/* Doughnut shaped nonsense        */
	"S1901M",		/* 5-Italy version with colours    */
	"S1901M",		/* Milan Diplomacy                 */
	"S1901M",		/* Asian Diplomacy                 */
	"S1995M",		/* Modern Diplomacy                */
	"S1870M",		/* Colonial Diplomacy              */
	"S1425M",		/* 100 Year War Diplomacy          */
	"S1425M",		/* 100 Year War Diplomacy          */
	"S1050M",		/* Classical Med.  Diplomacy       */
	"S1999M",               /* Empire Diplomacy                */
	"S1901M",               /* African2 Diplomacy              */
	"S1901M",               /* Wraparound2 Diplomacy           */
	"S1901M",               /* Shift_Around Diplomacy          */
	"S1901M",               /* Layered Diplomacy               */
	"S1901M",               /* SailHo2 Diplomacy               */
	"S1901M",               /* Sailho2_Crowded Diplomacy       */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
};
