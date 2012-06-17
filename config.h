#ifndef _LOGGER_CONFIG_H_
#define _LOGGER_CONFIG_H_
/*in this file we define variables which govern the behaviour of logger*/
/* enable debug mode --comment it to disable debugging messages */
#define log_debug
/* decide whether we have C99 support */
#ifndef _HAVE_LOGGER_C99_
#define _HAVE_LOGGER_C99_
#endif
/* disable warning msgs --comment to enable warning msgs */
/*#define _LOGGER_NO_WARNING_**/
/*disable error msgs --comment to enable error msgs */
/*#define _LOGGER_NO_ERR_**/
/* disable error return vectors--apart from printing on stderr also return them to caller.--comment to enable rv errors*/
/*#define _LOGGER_NO_ERETURN_**/
#endif