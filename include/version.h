#ifndef	BTCM_VERSION_INCLUDED
#define BTCM_VERSION_INCLUDED

#define MAKE_STR_HELPER(a_str) #a_str
#define MAKE_STR(a_str) MAKE_STR_HELPER(a_str)

#define BTCM_VERSION_MAJOR	1
#define BTCM_VERSION_MINOR	0
#define BTCM_VERSION_PATCH	0
#define BTCM_VERSION_BETA	0
#define BTCM_VERSION_VERSTRING	MAKE_STR(BTCM_VERSION_MAJOR) "." MAKE_STR(BTCM_VERSION_MINOR) "." MAKE_STR(BTCM_VERSION_PATCH) "." MAKE_STR(BTCM_VERSION_BETA)

#endif