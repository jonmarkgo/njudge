/*
 * $Log$
 * Revision 1.19  2004/08/09 20:15:10  machmstr
 * While I am at it: Lowland had Lowland as adjective, corrected to 'Lowlander'.
 *
 * Revision 1.18  2004/08/07 17:05:58  machmstr
 * Correction of adjectives: it is portugal portuguese and cyprus cypriotic (or cyprean).
 * WARNING there are two entries for Norway Norwegian !!!  (I have not touched in case there is some reason to it.)
 *
 * Revision 1.17  2004/08/07 16:05:42  machmstr
 * All AH Mach variants added and then some
 *
 * Revision 1.17  2004/08/07 01:35:00  Sergio Lidsell
 * All AH Mach variants added and then some

 * Revision 1.16  2004/07/30 22:01:27  millis
 * Updated Minorpower variant to have real power names
 *
 * Revision 1.15  2004/03/16 07:52:39  nzmb
 * dded the minorpowers variant.
 *
 * Revision 1.14  2003/05/03 16:16:35  millis
 * Missed a Neutral definition.
 *
 * Revision 1.13  2003/05/02 23:15:19  millis
 * Added in definitions for Neutral player
 *
 * Revision 1.12  2002/12/28 01:08:09  millis
 * Bug 57, Add colonial96 variant
 *
 * Revision 1.11  2002/09/26 21:53:10  millis
 * Added Machiavelli2 variant
 *
 * Revision 1.10  2002/06/01 01:02:35  millis
 * Undo last change, caused blind games to be rejected at signon stage
 *
 * Revision 1.9  2002/05/16 09:40:05  miller
 * Blind game also sets quiet flag
 *
 * Revision 1.8  2001/10/26 23:20:34  nzmb
 * put the crowded_1898 variant in (Tim Miller)
 *
 * Revision 1.7  2001/10/20 12:11:16  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.6.2.2  2001/10/20 00:53:08  dedo
 * Removed compile warnings
 *
 *
 */       

#include <stdlib.h>
#include <string.h>

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
	"machiavelli",		/* Machiavelli 1454 Balance of Power */
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
	"ancient_med",		/* Ancient medieteraenean	   */
	"crowded_1898",		/* crowded powers, 1898 style      */
        "machiavelli2",         /* Mach2 1454 Balance of Power     */
	"colonial96",           /* Colonial 1996 Diplomacy         */
	"minorpower",		/* Minor powers Diplomacy	   */
	"mach_eos",		/* Mach1 Expansion of States	   */
	"mach_este",		/* Mach1 Expansion of States Este  */
	"mach_sfd1",		/* Mach1 Struggle for Dominance part 1 */
	"mach_sfd2",		/* Mach1 Struggle for Dominance part 2 */
	"mach_span",		/* Mach1 Spanish Preponderance	   */
	"mach_aoc",		/* Mach1 Age of Communes	   */
	"dipmach",		/* Mach played on the Standard map */
	"machfive",		/* Machfive-Sort of five italies mach */
	"mach2_sfd1",		/* Mach2 Struggle for D. part 1	   */
	"mach2_sfd2",		/* Mach2 Struggle for D. part 2	   */
	"mach2_span",		/* Mach2 Spanish Preponderance	   */
	"mach2_fotl",		/* Mach2 Fall of the Lombards	   */
	"mach2_eai",		/* Mach2 Empire and Invaders	   */
	"mach2_pamt",		/* Mach2 Papal Ascendancy and Mongol Threat */
	"mach2_pa",		/* Mach2 4 player pamt	   */
	"mach2_ttr",		/* Mach2 To The Renaissance	   */
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
	V_ancient_med,		/* 31 Ancient_Med		     */
	V_crowded_1898,		/* 32 crowded_1898		     */
        V_machiavelli2,         /* 33 Machiavelli2                   */
	V_colonial96,		/* 34 Colonial 1996 variant	     */
	V_minorpower,		/* 35 Minor powers Diplomacy	     */
	V_mach_eos,		/* Mach1 Expansion of States	   */
	V_mach_este,		/* Mach1 Expansion of States Este  */
	V_mach_sfd1,		/* Mach1 Struggle for Dominance part 1 */
	V_mach_sfd2,		/* Mach1 Struggle for Dominance part 2 */
	V_mach_span,		/* Mach1 Spanish Preponderance	   */
	V_mach_aoc,		/* Mach1 Age of Communes	   */
	V_dipmach,		/* Mach played on the Standard map */
	V_machfive,		/* Machfive-Sort of five italies mach */
	V_mach2_sfd1,		/* Mach2 Struggle for D. part 1	   */
	V_mach2_sfd2,		/* Mach2 Struggle for D. part 2	   */
	V_mach2_span,		/* Mach2 Spanish Preponderance	   */
	V_mach2_fotl,		/* Mach2 Fall of the Lombards	   */
	V_mach2_eai,		/* Mach2 Empire and Invaders	   */
	V_mach2_pamt,		/* Mach2 Papal Ascendancy and Mongol Threat */
	V_mach2_pa,		/* Mach2 4 player pamt	   */
	V_mach2_ttr,		/* Mach2 To The Renaissance	   */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	F_GUNBOAT,		/* Gunboat (anonymous) option      */
	F_BLIND /*| F_QUIET*/,	/* Blind movement option           */
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
	"Syria","Carthage","Macedon","Rome","Greece",
	"Belgium", "Bulgaria", "Denmark", 
	"Portugal", "Rumania", "Serbia", "Sweden", "Tunis",
        /* mach_aoc +6 */
	"Scaligeri", "Pisa", "Genoa", "Savoy", "Trinacria", "Provence",
	/* mach_este +1 */
	"Este",
	/* machfive +5 */
	"First", "Second", "Third", "Fourth", "Fifth",
	/* mach_fotl +1 */
	"Lombardy",
	/* mach_eai +2 */
	"Empire", "Norman",
	/* mach_pamt +1 */
	"Mongols",
	/* mach_ttr +1 */
	"Aragon",
	//** ADD NEW POWERS ABOVE THIS LINE! **/
	"a power", "Neutral", "Autonomous","Observer","Master"};

/*  Owner names (i.e., adjective form of power names).  */

char *owners[] =
{
	"unowned",
	"Austrian","Balkan","British","Burgundian","Byzantine","Chinese",
	"Egyptian","Irish","English","Florentine","French","German",
	"Hungarian","Indian","Indonesian","Israelite","Italian","Japanese",
	"Lowlander","Milanese","Neapolitan","Norwegian","Papal","Persian",
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
	"Northern","Southern","Eastern","Western","Cypriotic","Dissentos",
	"Syrian","Carthaginian","Macedonian","Roman","Greek",
	"Belgian", "Bulgarian", "Danish", 
	"Portuguese", "Rumanian", "Serbian", "Swedish", "Tunisian",
        /* mach_aoc +6 */
	"Scaligerian", "Pisan", "Genoese", "Sabaud", "Trinacrian", "Provencal",
	/* mach_este +1 */
	"Estensian",
	/* machfive +5 */
	"First", "Second", "Third", "Fourth", "Fifth",
	/* mach_fotl +1 */
	"Lombard",
	/* mach_eai +2 */
	"Imperial", "Norman",
	/* mach_pamt +1 */
	"Mongol",
	/* mach_ttr +1 */
	"Aragonese",
	/** ADD NEW POWERS ABOVE THIS LINE! **/
	"??????","Neutral", "Autonomous","Observer","Master"};

/*  Power abbreviations based on variant ('x' means corresponding power isn't
   in this variant).  The last five characters should always be '?=xOM', except
   for Machiavelli where it's '?=&OM'.  Likewise, 'O' and 'M' should not be
   used for anything other than Observer and Master, respecitively.  */

char *pletter[] =
{
    NULL,                               /* Don't use 0 as a variant number */
/** ADD NEW POWERS TO EACH LINE! **/
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Standard variant              */
    ".AxxxxCxxExFGxNxxIJxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Youngstown variant            */
    ".AxxxxxxxExFGxxxxIxxxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Loeb 9 player variant         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Pure variant                  */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxABCDEFGHIJKLNPQRSTUVWXYZ0123456789xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* 34 player nonsense            */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Great Britain variant         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* 1898 variant                  */
    ".ABxxxxxxExFGxxxxIxLxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Eleven player variant         */
    ".AxxxxxxxxLFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* Machiavelli 1454              */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Standard with a fleet in rome */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Shift right variant           */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Shift left variant            */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxA1E2F3G4I5R6T7xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Root Z Diplomacy              */
    ".xxxBZxxExxxxHxxIxxxxxxxxPxxCSxUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* The way things might have been*/
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Doughnut shaped nonsense      */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxYDLRBxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* 5-Italy version with colours  */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Milan Diplomacy               */
    ".xxxxxCxxxxxxxNIxxJxxxxxPxRSxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Asian Diplomacy               */
    ".xxBxxxExxxFGxxxxIxxxxxxxPRxxSTUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Modern Diplomacy              */
    ".xxBxxCxxxxFxxxxxxJxxxxxxxRxxxTxxHxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Colonial Diplomacy            */
    ".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Hundred Years War Diplomacy   */
    ".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* H31 Diplomacy                 */
    ".xxxxxxExxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxSCARxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Classical Diplomacy           */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxBCXFHNPQTUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Empire4 Diplomacy             */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxEILSAZxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* African2 Diplomacy            */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Wraparound2 Diplomacy         */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Shift_Around Diplomacy        */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Layered Diplomacy             */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxNSEWCDxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Sailho2_Crowded Diplomacy     */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxNSEWxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* SailHo2 Diplomacy             */
    ".xxxxxxExxxxxxxxxxxxxxxxPxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxCxRGxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* AncMed Diplomacy 	     */
    ".ABxxxxxxExFGxxxxIxLxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* crowded_1898 */
    ".AxxxxxxxxLFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* Machiavelli2 1454             */
    ".xxBxxCxxxxFxxxxxxJxxxxxxxRxxxTxxHxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=xOM", /* Colonial 1996 Diplomacy       */
    ".AxxxxxxxExFGxxxxIxxxxYxxxRxxNTxxHxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxCBLDPUSWXxxxxxxxxxxxxxxxxx?=xOM", /* Minor power                   */
    ".xxxxxxxxxLxxxxxxxxxINxxxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* Mach Expansion of States      */
    ".xxxxxxxxxLxxxxxxxxxINxxxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxExxxxxxxxxx?=&OM", /* Mach Expansion of States ESTE */
    ".AxxxxxxxxLFxxxxxxxxxxxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach_sfd1                     */
    ".AxxxxxxxxxFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach_sfd2                     */
    ".AxxxxxxxxLFxxxxxxxxxNxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach_span                     */
    ".xxxxxxxxxLxxxxxxxxxINxxxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxAPGSTRxxxxxxxxxxx?=&OM", /* Mach Age of Communes          */
    ".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* Dipmach-Mach on standard map  */
    ".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx12345xxxxx?=&OM", /* machfive                      */
    ".AxxxxxxxxLFxxxxxxxxxxxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach2_sfd1                    */
    ".AxxxxxxxxxFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach2_sfd2                    */
    ".AxxxxxxxxLFxxxxxxxxxNxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?=&OM", /* mach2_span                    */
    ".xxxxBxxxxxFxxxxxxxxxxxPxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxLxxxx?=&OM", /* mach2_fotl                    */
    ".xxxxBxxxxxxxxxxxxxxxxxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxSxxxxxxxxxxLENxx?=&OM", /* mach2_eai                    */
    ".xxxxxxxxxxxxxxxxxxxIxxPxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxExGx?=&OM", /* mach2_pamt                    */
    ".xxxxxxxxxxxxxxxxxxxIxxPxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxExxx?=&OM", /* mach2_pa                    */
    ".xxxxxxxxxLxxxxxxxxxINxPxxxxxxxxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxGxxxxxxxxxxxxxA?=&OM", /* mach2_ttr                    */
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
	"S0001M",		/* Ancient_Med Diplomacy	   */
	"S1899M",		/* crowded_1898 Diplomacy	   */
        "F1453A",               /* Machiavelli2 1454               */
        "S1870M",               /* Colonial 1996 Diplomacy         */
	"F1900B",		/* Minor powers Diplomacy	   */
	"F1384A",		/* Mach1 Expansion of States	   */
	"F1384A",		/* Mach1 Expansion of States Este  */
	"F1498A",		/* Mach1 Struggle for Dominance part 1 */
	"F1512A",		/* Mach1 Struggle for Dominance part 2 */
	"F1520A",		/* Mach1 Spanish Preponderance	   */
	"F1334A",		/* Mach1 Age of Communes	   */
	"F1900A",		/* Mach played on the Standard map */
	"F3000A",		/* Machfive-Sort of five italies mach */
	"F1498A",		/* Mach2 Struggle for D. part 1	   */
	"F1512A",		/* Mach2 Struggle for D. part 2	   */
	"F1520A",		/* Mach2 Spanish Preponderance	   */
	"F724A",		/* Mach2 Fall of the Lombards	   */
	"F1050A",		/* Mach2 Empire and Invaders	   */
	"F1176A",		/* Mach2 Papal Ascendancy and Mongol Threat */
	"F1176A",		/* Mach2 4 player pamt		   */
	"F1249A",		/* Mach2 To The Renaissance	   */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
};
