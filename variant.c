#include <stdlib.h>

#include "dip.h"
#include "variant.h"

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
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	"gunboat",		/* Gunboat (anonymous) option      */
	"blind",		/* Blind movement option           */
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
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	F_GUNBOAT,		/* Gunboat (anonymous) option      */
	F_BLIND,		/* Blind movement option           */
};

/*  Power names.  */

char *powers[] =
{
	"Unowned",
/* POWER LIST BEGINS HERE */
	"Austria", "Balkan", "Britain", "Burgundy", "Byzantium",
	"China", "Egypt", "Eire", "England", "Florence",
	"France", "Germany", "Hungary", "India", "Indonesia",
	"Israel", "Italy", "Japan", "Lowland", "Milan",
	"Naples", "Norway", "Papacy", "Persia", "Poland",
	"Russia", "Siberia", "Sicily", "Spain", "Turkey",
	"Ukraine", "Venice",
	"Holland",
	"Austria1", "Austria2", "England1", "England2", "France1",
	"France2", "Germany1", "Germany2", "Italy1", "Italy2",
	"Russia1", "Russia2", "Turkey1", "Turkey2",
	"Yellow", "Dark", "Light", "Red", "Blue",
	"A-Ank", "B-Bel", "C-Ber", "D-Bre", "E-Bud", "F-Bul", "G-Con",
	"H-Den", "I-Edi", "J-Gre", "K-Hol", "L-Kie", "N-Lvp", "P-Lon",
	"Q-Mar", "R-Mos", "S-Mun", "T-Nap", "U-Nor", "V-Par", "W-Por",
	"X-Rom", "Y-Rum", "Z-Ser", "0-Sev", "1-Smy", "2-Spa", "3-StP",
	"4-Swe", "5-Tri", "6-Tun", "7-Ven", "8-Vie", "9-War",
	"Republic of Carthage", "Ptolemaic Egypt", "Republic of Rome",
	"Antigonid Macedon", "Seleucid Syria",
/** ADD NEW POWERS ABOVE THIS LINE! **/
	"a power", "Autonomous", "Observer", "Master"};

/*  Owner names (i.e., possessive form of power names).  */

char *owners[] =
{
	"unowned",
	"Austrian", "Balkan", "British", "Burgundian", "Byzantine",
	"Chinese", "Egyptian", "Irish", "English", "Florentine",
	"French", "German", "Hungarian", "Indian", "Indonesian",
	"Israelite", "Italian", "Japanese", "Lowland", "Milanese",
	"Naplese", "Norwegian", "Papal", "Persian", "Polish",
	"Russian", "Siberian", "Sicilian", "Spanish", "Turkish",
	"Ukranian", "Venetian",
	"Dutch",
	"1Austrian", "2Austrian", "1English", "2English", "1French",
	"2French", "1German", "2German", "1Italian", "2Italian",
	"1Russian", "2Russian", "1Turkish", "2Turkish",
	"Yellow", "Dark", "Light", "Red", "Blue",
	"A-Ank", "B-Bel", "C-Ber", "D-Bre", "E-Bud", "F-Bul", "G-Con",
	"H-Den", "I-Edi", "J-Gre", "K-Hol", "L-Kie", "N-Lvp", "P-Lon",
	"Q-Mar", "R-Mos", "S-Mun", "T-Nap", "U-Nor", "V-Par", "W-Por",
	"X-Rom", "Y-Rum", "Z-Ser", "0-Sev", "1-Smy", "2-Spa", "3-StP",
	"4-Swe", "5-Tri", "6-Tun", "7-Ven", "8-Vie", "9-War",
	"Carthaginian", "Ptolemaic", "Roman", "Antigonid", "Seleucid",
/** ADD NEW POWERS ABOVE THIS LINE! **/
	"??????", "Autonomous", "Observer", "Master"};

/*  Power abbreviations based on variant ('x' means corresponding power isn't
   in this variant).  The last five characters should always be '?xOM', except
   for Machiavelli where it's '?&OM'.  Likewise, 'O' and 'M' should not be
   used for anything other than Observer and Master, respecitively.  */

char *pletter[] =
{
	NULL,			/* Don't use 0 as a variant number */
/** ADD NEW POWERS TO EACH LINE! **/
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Standard variant */
	".AxxxxCxxExFGxNxxIJxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Youngstown variant */
	".AxxxxxxxExFGxxxxIxxxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Loeb 9 player variant */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Pure variant */
	".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxABCDEFGHIJKLNPQRSTUVWXYZ0123456789xxxxx?xOM",	/* 34 player nonsense */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Great Britain variant */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* 1898 variant */
	".ABxxxxxxExFGxxxxIxLxxNxxxRxxSTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Eleven player variant */
	".AxxxxxxxxLFxxxxxxxxINxPxxxxxxTxVxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?&OM",	/* Machiavelli 1454 */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Standard with a fleet in rome */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Shift right variant */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Shift left variant */
	".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxA1E2F3G4I5R6T7xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Root Z Diplomacy */
	".xxxBZxxExxxxHxxIxxxxxxxxPxxCSxUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* The way things might have been */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Doughnut shaped nonsense      */
	".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxYDLRBxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* 5-Italy version with colours  */
	".AxxxxxxxExFGxxxxIxxxxxxxxRxxxTxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Milan Diplomacy */
	".xxxxxCxxxxxxxNIxxJxxxxxPxRSxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Asian Diplomacy */
	".xxBxxxExxxFGxxxxIxxxxxxxPRxxSTUxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Modern Diplomacy */
	".xxBxxCxxxxFxxxxxxJxxxxxxxRxxxTxxHxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Colonial Diplomacy */
	".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Hundred Years War Diplomacy */
	".xxxBxxxxExFxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xOM",	/* Hundred Years War Diplomacy */
	".xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxCERAS?xOM",	/* Classical Medit. Diplomacy */
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
	"S1050M",		/* Classical Med.  Diplomacy      */
/** ADD NEW VARIANTS ABOVE THIS LINE! **/
};
