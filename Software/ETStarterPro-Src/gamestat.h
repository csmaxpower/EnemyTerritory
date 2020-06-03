/*
	Copyright (c) 2004 Cory Nelson

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*! \file
	 \brief Defines all the public GameStat functions.
*/

#ifndef __GAMESTAT_H__
#define __GAMESTAT_H__

//! Boolean values.
enum GS_BOOLEAN {
	GS_FALSE=0,					///< False
	GS_TRUE						///< True
};

#ifndef __cplusplus
//! Typedefs for non-C++ compilers.
typedef struct GS_PLAYERINFO GS_PLAYERINFO;
//! Typedefs for non-C++ compilers.
typedef struct GS_PROPERTY GS_PROPERTY;
//! Typedefs for non-C++ compilers.
typedef struct GS_SERVERINFO GS_SERVERINFO;
//! Typedefs for non-C++ compilers.
typedef struct GS_SERVER GS_SERVER;
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! \defgroup Setup Library Setup
//! @{

//! Initialization flags.
enum GS_INITFLAGS {
	GS_INIT_NONE=0,			///< Don't initialize anything.
	GS_INIT_WINSOCK=1			///< Initialize Windows Sockets.  Does nothing on non-Windows platforms.
};

//! Cleanup flags.
enum GS_CLEANUPFLAGS {
	GS_CLEANUP_NONE=0,		///< Don't cleanup anything.
	GS_CLEANUP_WINSOCK=1		///< Cleanup Windows Sockets.  Does nothing on non-Windows platforms.
};

//! Initializes GameStat
/*!
	\param flags Initializes any libraries or internal structures GameStat depends on.  Must be one of the GS_INIT_* enumerations.
	\return GS_TRUE on success, GS_FALSE on error.  To find out why, call GSLastError().
	\sa GS_INITFLAGS, GSCleanup()
*/
unsigned char GSInit(unsigned char flags);

//! Cleans up GameStat
/*!
	\param flags Cleans up any libraries or internal structures GameStat depends on.  Must be one of the GS_CLEANUP_* enumerations.
	\return GS_TRUE on success, GS_FALSE on error.  To find out why, call GSLastError().
	\sa GS_CLEANUPFLAGS
*/
unsigned char GSCleanup(unsigned char flags);

//! Retrieves the current library version
/*!
	Low byte contains the major version, High byte contains the minor version.
	\return Library version
*/
unsigned short GSVersion(void);

//! @}

//! \defgroup Options Setting Options
//! @{

//! Option types.
enum GS_OPTION {
	GS_OPTION_TIMEOUT=0,		///< int, Sets the timeout in milliseconds.  default 1000.
	GS_OPTION_COLORFILTER	///< bool, GS_TRUE = filter strings for color codes, GS_FALSE = no filtering.  default GS_FALSE.
};

//! Sets an internal boolean for GameStat.
/*!
	\param option Describes which option to set.  Must be one of the boolean GS_OPTION_* enumerations.
	\param value The value to set.  Must be GS_TRUE or GS_FALSE.
	\sa GS_OPTION
*/
void GSSetOptBool(unsigned short option, unsigned char value);

//! Sets an internal integer for GameStat.
/*!
	\param option Describes which option to set.  Must be one of the integer GS_OPTION_* enumerations.
	\param value The value to set.
	\sa GS_OPTION
*/
void GSSetOptInt(unsigned short option, int value);

//! Gets an internal boolean for GameStat.
/*!
	\param option Describes which option to get.  Must be one of the boolean GS_OPTION_* enumerations.
	\return On success, the option's current value.  On failure, GS_FALSE.
	\sa GS_OPTION
*/
unsigned char GSGetOptBool(unsigned short option);

//! Gets an internal integer for GameStat.
/*!
	\param option Describes which option to get.  Must be one of the integer GS_OPTION_* enumerations.
	\return On success, the option's current value.  On failure, 0.
	\sa GS_OPTION
*/
int GSGetOptInt(unsigned short option);

//! @}

//! \defgroup Games Querying Game Servers
//! @{

//! Game server types.
enum GS_TYPE {
	GS_TYPE_QUAKEWORLD=0,	///< Quake World
	GS_TYPE_QUAKE2,			///< Quake 2
	GS_TYPE_QUAKE3,			///< Quake 3
	GS_TYPE_DOOM3,				///< Doom 3
	GS_TYPE_HALFLIFE,			///< Half-Life
	GS_TYPE_UNREAL2,			///< Unreal Engine 2
	GS_TYPE_GAMESPY2,			///< GameSpy Protocol 2
	GS_TYPE_BFV,				///< Battlefield Vietnam
	GS_TYPE_PAINKILLER,		///< Painkiller
	GS_TYPE_UT2003,			///< Unreal Tournament 2003
	GS_TYPE_UT2004,			///< Unreal Tournament 2004
	GS_TYPE_UNREAL2XMP,		///< Unreal 2 XMP
	GS_TYPE_SOF2,				///< Soldier of Fortune 2
	GS_TYPE_RTCW,				///< Return to Castle Wolfenstein
	GS_TYPE_WOLFET,			///< Wolfenstein: Enemy Territory
	GS_TYPE_CS,					///< Counter-Strike
	GS_TYPE_CSCZ,				///< Counter-Strike: Condition Zero
	GS_TYPE_CALLOFDUTY,		///< Call of Duty
	GS_TYPE_SWJKJA,			///< Star Wars Jedi Knight: Jedi Academy
	GS_TYPE_MOHAA,				///< Medal of Honor: Allied Assault
	GS_TYPE_MOHBT,				///< Medal of Honor: Breakthrough
	GS_TYPE_MOHSH,				///< Medal of Honor: Spearhead
	GS_TYPE_DOD,				///< Day of Defeat
	GS_TYPE_SOF,				///< Soldier of Fortune
	GS_TYPE_GAMESPY,			///< GameSpy Protocol
	GS_TYPE_UT,					///< Unreal Tournament
	GS_TYPE_SWJK2,				///< Star Wars Jedi Knight 2: Jedi Outcast
	GS_TYPE_DESCENT3,			///< Descent 3
	GS_TYPE_AMERICASARMY,	///< America's Army
	GS_TYPE_BF1942,			///< Battlefield 1942
	GS_TYPE_HALO,				///< Halo: Combat Evolved
	GS_TYPE_POSTAL2,			///< Postal 2
	GS_TYPE_STVEF,				///< Star Trek: Voyager - Elite Force
	GS_TYPE_STVEF2,			///< Star Trek: Voyager - Elite Force 2
	GS_TYPE_DEUSEX,			///< Deus Ex
	GS_TYPE_IL2S,				///< IL-2 Sturmovik
	GS_TYPE_IL2SFB,			///< IL-2 Sturmovik: Forgotten Battles
	GS_TYPE_SOURCE,			///< Source Engine
	GS_TYPE_HALFLIFE2,		///< Half-Life 2
	GS_TYPE_CSS,				///< Counter-Strike: Source
	GS_TYPE_ASE,				///< All-Seeing Eye Protocol
	GS_TYPE_MTA,				///< Multi Theft Auto
	GS_TYPE_MTAVC,				///< Multi Theft Auto: Vice City
	GS_TYPE_HERETIC2,			///< Heretic 2
	GS_TYPE_IGI2,				///< IGI-2: Covert Strike
	GS_TYPE_GORE,				///< Gore
	GS_TYPE_VIETCONG,			///< Vietcong
	GS_TYPE_NWN,				///< Neverwinter Nights
	GS_TYPE_SERIOUSSAM,		///< Serious Sam
	GS_TYPE_SERIOUSSAM2,		///< Serious Sam: The Second Encounter
	GS_TYPE_AOE2,				///< Age of Empires II
	GS_TYPE_AOE2TC,			///< Age of Empires II: The Conquerors
	GS_TYPE_AVP2,				///< Aliens vs Predator 2
	GS_TYPE_CHASER,			///< Chaser
	GS_TYPE_CHROME,			///< Chrome
	GS_TYPE_NOLF,				///< No One Lives Forever
	GS_TYPE_NOLF2,				///< No One Lives Forever 2
	GS_TYPE_SHOGO,				///< Shogo
	GS_TYPE_SOLDAT,			///< Soldat
	GS_TYPE_CODENAMEEAGLE,	///< Codename Eagle
	GS_TYPE_DEVASTATION,		///< Devastation
	GS_TYPE_GIANTSCK,			///< Giants: Citizen Kabuto
	GS_TYPE_GLOBALOPS,		///< Global Operations
	GS_TYPE_NERFAB,			///< Nerf Arena Blast
	GS_TYPE_PURGEJIHAD,		///< Purge Jihad
	GS_TYPE_RSC,				///< RalliSport Challenge
	GS_TYPE_RALLYMASTERS,	///< Rally Masters
	GS_TYPE_CCRENEGADE,		///< Command and Conquer: Renegade
	GS_TYPE_RUNE,				///< Rune
	GS_TYPE_SIN,				///< Sin
	GS_TYPE_TACTICALOPS,		///< Tactical Ops
	GS_TYPE_UCOMBAT,			///< Universal Combat
	GS_TYPE_UNREAL,			///< Unreal
	GS_TYPE_WHEELOFTIME,		///< Wheel of Time
	GS_TYPE_XPANDRALLY,		///< Xpand Rally
	GS_TYPE_DDPT,				///< Deadly Dozen: Pacific Theater
	GS_TYPE_DTR2,				///< Dirt Track Racing II
	GS_TYPE_DRAKAN,			///< Drakan: Order of the Flame
	GS_TYPE_F12002,			///< F1 2002
	GS_TYPE_GUNMAN,			///< Gunman Chronicles
	GS_TYPE_IRONSTORM,		///< Iron Storm
	GS_TYPE_JBNF,				///< James Bond: Nightfire
	GS_TYPE_KINGPIN,			///< Kingpin
	GS_TYPE_NFSHP2,			///< Need for Speed: Hot Pursuit 2
	GS_TYPE_OPFLASHPOINT,	///< Operation Flashpoint
	GS_TYPE_REDLINE,			///< Redline
	GS_TYPE_TERMINUS,			///< Terminus
	GS_TYPE_TUROK2,			///< Turok 2
	GS_TYPE_TRON20,			///< Tron 2.0
	GS_TYPE_TRIBES,			///< Starsiege: Tribes
	GS_TYPE_TRIBES2,			///< Tribes 2
	GS_TYPE_THPS3,				///< Tony Hawk's Pro Skater 3
	GS_TYPE_THPS4,				///< Tony Hawk's Pro Skater 4
	GS_TYPE_V8SCC,				///< V8 Supercar Challenge
	GS_TYPE_TEAMFACTOR,		///< Team Factor
	GS_TYPE_RAINBOWSIX,		///< Rainbow Six
	GS_TYPE_RAINBOWSIXRS,	///< Rainbow Six: Rogue Spear
	GS_TYPE_NITRO,				///< Nitro Family
	GS_TYPE_FARCRY,			///< Far Cry
	GS_TYPE_RISEOFNATIONS,	///< Rise of Nations
	GS_TYPE_CONTRACTJACK,	///< Contract J.A.C.K.
	GS_TYPE_HOMEWORLD2,		///< Homeworld 2
	GS_TYPE_DAIKATANA,		///< Daikatana
	GS_TYPE_SAVAGE,			///< Savage
	GS_TYPE_GHOSTRECON,		///< Ghost Recon
	GS_TYPE_BREED,				///< Breed
	GS_TYPE_MOHPA,				///< Medal of Honor: Pacific Assault
	GS_TYPE_OPFRESISTANCE,	///< Operation Flashpoint: Resistance
	GS_TYPE_SOAF,				///< Sum of All Fears
	GS_TYPE_STBC,				///< Star Trek: Bridge Commander
	GS_TYPE_CODUO,				///< Call of Duty: United Offensive
	GS_TYPE_SWBF,				///< Star Wars: Battlefront
	GS_TYPE_TRIBES3,			///< Tribes: Vengeance
	GS_TYPE_THUG2				///< Tony Hawk's Underground 2
};

//! A linked list containing player information.
struct GS_PLAYERINFO {
	char *name;					///< The player's name
	int score;					///< The player's score
	int ping;					///< The player's ping
	int time;					///< The player's time in the game, in seconds.

	GS_PLAYERINFO *next;		///< The next GS_PLAYERINFO in the linked list.
};

//! A linked list containing miscellaneous server information.
struct GS_PROPERTY {
	char *key;					///< The property's key
	char *value;				///< The property's value

	GS_PROPERTY *next;		///< The next GS_PROPERTY in the linked list.
};

//! Server status information returned from GSQueryServer().
struct GS_SERVERINFO {
	char *host;						///< Server name.
	char *mod;						///< Mod, or game type if mod is not available.
	char *map;						///< Current map.

	int numplayers;				///< Number of players on the server.  May or may not be equal to the number of players in the linked list.
	int maxplayers;				///< Maximum players the server supports.
	int timelimit;					///< Time limit for one map, in minutes.
	int scorelimit;				///< Score limit for one map.

	GS_PROPERTY *properties;	///< A linked list containing miscellaneous properties that doesn't fit into the generic server info.
	GS_PLAYERINFO *players;		///< A linked list containing player information.

	// added in 1.1
	int passworded;				///< A boolean value for password protected servers.  \since 1.1
	int ping;						///< Ping time to the server, in milliseconds.  \since 1.1
};

//! Queries a server for it's status information.
/*!
	\param type Describes the type of server to query.  Must be one of the GS_TYPE_* enumerations.
	\param host A string containing a valid hostname, IPv4, or IPv6 address.
	\param port The port to contact the server on.  If this is 0 GameStat will try to guess it for you.
	\return On success, status information for the server.  On failure, NULL.  To find out why, call GSLastError().
	\note \a port must be the server's query port, which is usually different from the game port.
	\sa GS_TYPE, GSFreeServerInfo()
*/
GS_SERVERINFO *GSQueryServer(unsigned short type, const char *host, unsigned short port);

//! Frees the status information returned by GSQueryServer()
/*!
	\param info The status information to free.
*/
void GSFreeServerInfo(GS_SERVERINFO *info);

//! Strips any color or formatting from a string.
/*!
	\param type Describes the type of server the string came from.  Must be one of the GS_TYPE_* enumerations.
	\param out Specifies the destination string.
	\param in Specifies the input string.
	\return On success, a pointer to the new string.  On failure, NULL.  To find out why, call GSLastError().
	\note \a out can safely overlap \a in.  If \a out == \a in, conversion is done in place.
	\since 1.1
	\sa GS_TYPE
*/
char *GSStripColor(unsigned short type, char *out, const char *in);

//! @}

//! \defgroup Masters Querying Master Servers
//! @{

//! Master server types.
enum GS_MASTER {
	GS_MASTER_DOOM3=0,		///< Doom 3
	GS_MASTER_QUAKE3,			///< Quake 3
	GS_MASTER_QUAKE2,			///< Quake 2
	GS_MASTER_RTCW,			///< Return to Castle Wolfenstein
	GS_MASTER_WOLFET,			///< Wolfenstein: Enemy Territory
	GS_MASTER_SWJK2,			///< Star Wars Jedi Knight 2: Jedi Outcast
	GS_MASTER_HALFLIFE,		///< Half-Life
	GS_MASTER_CS,				///< Counter-Strike
	GS_MASTER_CSCZ,			///< Counter-Strike: Condition Zero
	GS_MASTER_CSS,				///< Counter-Strike: Source
	GS_MASTER_DOD,				///< Day of Defeat
	GS_MASTER_SOF2,			///< Soldier of Fortune 2
	GS_MASTER_SWJKJA,			///< Star Wars Jedi Knight: Jedi Academy
	GS_MASTER_STVEF,			///< Star Trek: Voyager - Elite Force 
	GS_MASTER_STVEF2			///< Star Trek: Voyager - Elite Force 2
};

//! Master server filters.
enum GS_MASTER_FILTER {
	GS_MASTER_FILTER_NONE=0,			///< All servers.
	GS_MASTER_FILTER_FULL=1,			///< Full servers.
	GS_MASTER_FILTER_NOTFULL=2,		///< Servers that aren't full.
	GS_MASTER_FILTER_EMPTY=4,			///< Empty servers.
	GS_MASTER_FILTER_NOTEMPTY=8,		///< Servers that aren't empty.
	GS_MASTER_FILTER_PASSWORD=16,		///< Servers that require a password.
	GS_MASTER_FILTER_NOPASSWORD=32	///< Servers that don't require a password.
};

//! A linked list of servers returned from GSQueryMaster().
struct GS_SERVER {
	char *host;					///< The server's hostname or IP.
	unsigned short port;		///< The server's port.

	GS_SERVER *next;			///< The next GS_SERVER in the linked list.
};

//! Queries a master server for game servers.
/*!
	\param type Describes the type of server to query.  Must be one of the GS_MASTER_* enumerations.
	\param host A string containing a valid hostname, IPv4, or IPv6 address.  If this is NULL, GameStat will try to guess it for you.
	\param port The port to contact the server on.  If this is 0 GameStat will try to guess it for you.
	\param filter Describes the type of servers to retrieve.  Can be a combination of the GS_MASTER_FILTER* enumerations.
	\return On success, a linked list of servers.  On failure, NULL.  To find out why, call GSLastError().
	\sa GS_MASTER, GS_MASTER_FILTER, GSFreeServers()
	\since 1.1
*/
GS_SERVER *GSQueryMaster(unsigned short type, const char *host, unsigned short port, unsigned short filter, unsigned short protocol);

//! Frees the server list returned by GSQueryMaster()
/*!
	\param servers The server list to free.
	\since 1.1
*/
void GSFreeServers(GS_SERVER *servers);

//! @}

//! \defgroup Errors Error Handling
//! @{

//! Error types.
enum GS_ERROR {
	GS_OK=0,								///< Everything went okay
	GS_ERROR_WINSOCK,					///< An internal Windows Sockets error occured
	GS_ERROR_UNKNOWNTYPE,			///< Unknown server type
	GS_ERROR_UNKNOWNHOST,			///< Unable to resolve host or invalid IP
	GS_ERROR_CONNECT,					///< Unable to connect to host
	GS_ERROR_TIMEOUT,					///< Connection timed out, host may be unreachable
	GS_ERROR_CORRUPT,					///< Unknown or currupt response recieved
	GS_ERROR_UNKNOWNPORT,			///< Could not determine the server port
	GS_ERROR_NOTIMPLEMENTED,		///< Function not yet implemented
	GS_ERROR_INVALIDPARAMETER		///< Invalid parameter
};

//! Retrieves information on the last error, if any.
/*!
	\return The error code of the last error.  If no error has occured, GS_OK.
	\sa GS_ERROR, GSLastErrorString()
*/
unsigned short GSLastError(void);

//! Translates an error code into a human-readable string.
/*!
	\param err The error code to retrieve.
	\return The string associated with the error, or NULL on an invalid error code.
*/
const char *GSErrorString(unsigned short err);

//! Retrieves information on the last error, if any.
/*!
	\return A human-readable string representing the last error to occur.
	\note This is equivalent to calling GSErrorString(GSLastError())
*/
const char *GSLastErrorString(void);

//! @}

#ifdef __cplusplus
}
#endif

#endif
