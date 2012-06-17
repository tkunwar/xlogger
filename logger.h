#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <string.h>
#include<fcntl.h>
#include<sys/types.h>
#include <sys/stat.h>
#include "config.h"
/*define logger facility : possible values :
 * LOG_AUTH  LOG_AUTHPRIV  LOG_CRON  LOG_DAEMON LOG_FTP LOG_KERN  LOG_LPR LOG_MAIL  LOG_NEWS LOG_SYSLOG  LOG_USER  LOG_UUCP 
 */
#define LOGGER_FAC LOG_USER
/* define logger option:possible values 
 *LOG_PID LOG_CONS LOG_ODELAY LOG_NDELAY LOG_NOWAIT LOG_PERROR 
  */
#define LOGGER_OPT (LOG_PERROR | LOG_PID | LOG_CONS)
#ifdef log_debug
/* lets make use of variadic macros 
 * sdebug() macro is when we want to use single string message with no param
 * var_debug() macro can take multiple macros 
 * remember that these are features of C99 only 
 * if no C99 support then use long and clumsy debug macros--we are not mentioning them here..have a 
 * look at debug.h 
 * Note : in case macros are using variable params and the respective macro control flag is set to off
 * then there will be warning msgs about variables not being used. This is ok.
 */
#ifdef _HAVE_LOGGER_C99_
#define sdebug(s) fprintf(stderr, "[" __FILE__ ":%i] debug: " s "\n",__LINE__)
#define var_debug(s, ...) fprintf(stderr, "[" __FILE__ ":%i] debug: " s "\n",__LINE__,__VA_ARGS__)
#endif
#else 
#define sdebug(s)
#define var_debug(s, ...)
#endif
/*now handle wwarning msgs */
#ifndef _LOGGER_NO_WARNING_
#ifdef _HAVE_LOGGER_C99_
#define swarn(s) fprintf(stderr, "[" __FILE__ ":%i] Warning: " s "\n",__LINE__)
#define var_warn(s, ...) fprintf(stderr, "[" __FILE__ ":%i] Warning: " s "\n",__LINE__,__VA_ARGS__)
#endif
#else
#define swarn(s)
#define var_warn(s, ...)
#endif
/*handle error msgs as well*/
#ifndef _LOGGER_NO_ERR_
#ifdef _HAVE_LOGGER_C99_
#define serror(s) fprintf(stderr, "[" __FILE__ ":%i] Error: " s "\n",__LINE__)
#define var_error(s, ...) fprintf(stderr, "[" __FILE__ ":%i] Error: " s "\n",__LINE__,__VA_ARGS__)
#endif
#else
#define serror(s)
#define var_error(s, ...)
#endif
#ifndef _LOGGER_NO_ERETURN_
#ifdef _HAVE_LOGGER_C99_
#define s_ereturn(rv, s) do{ fprintf(stderr, "[" __FILE__ ":%i] ereturn: " s "\n", __LINE__); return rv; }while(0)
#define var_ereturn(rv, s,...) do{ fprintf(stderr, "[" __FILE__ ":%i] ereturn: " s "\n", __LINE__,__VA_ARGS__); return rv; }while(0)
#endif
#else
#define s_ereturn(rv,s) return(rv)
#define var_ereturn(rv,s, ...) return(rv)
#endif
/*=========usual stuff =======*/
#ifndef _LOG_TRUE_FALSE_
#define _LOG_TRUE_FALSE_
#define TRUE 1
#define FALSE 0
#endif
 

/*
 * some platform independent stuff
 * we will use ILP32 data models only for better portability across both 32 bit and 64 bit systems 
 * and also across both MS-WIN and UNIX* systems
 * so use these data types throughout xmpcrypt
 * basically three sizes 16bit ,32 bit,64 bits   
 * TODO: --write a pure 64 bit code later on (truly cross platform code later on)
 */
#ifndef _LOGGER_DTYPES_
#define _LOGGER_DTYPES_
#define _int16 short int
#define _int32 long int
#define _int64 long long int
#define _uint16 unsigned short int
#define _uint32 unsigned long int
#define _uint64 unsigned long long int
#define _uchar unsigned char
#define LOGGER_BOOL _int16
#endif
#define MAX_PROG_NAME 200
#define MAX_FILE_LEN 2048 /*should be enough --though a proper path_len should be used. using system's PATH_LEN (or
                               pathconf() ) should be enough but is not that a too big value*/
/*logging modes :
 * MODE_CONS : log to console only
 * MODE_SYSLOG :log using syslog
 * MODE_FILE: log using custom log file
 * priority levels in decreasing level :        LOG_EMERG,LOG_ALERT,LOG_CRIT,LOG_ERR,LOG_WARNING,LOG_NOTICE,LOG_INFO,LOG_DEBUG
 */
typedef enum{MODE_CONS,MODE_SYSLOG,MODE_FILE}log_mod_t;
typedef struct{
    char prog_name[MAX_PROG_NAME];
    log_mod_t mode;
    //int file_desc; /*for inernal usage only*/
    FILE *fp;
    _uint16 priority; /*only seven priorities --_uint16 should be enough*/
    pid_t pid;   
    char fname[MAX_FILE_LEN];
}logger;
LOGGER_BOOL log_init(logger *l);
//#define log_msg(s) fprintf(stderr,"\n u passed :%s",s)
/*passing logger objects ensures that code will be safe from any modification --(thread-safe ??)
 * but cant i do without it ?
 */
void log_msg(logger l,const char*msg);
LOGGER_BOOL set_logmask(logger *l,_uint16 UPTO_PRIO);
void close_log(logger l);
#endif