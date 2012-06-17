#include "logger.h"
extern int errno;
LOGGER_BOOL log_init(logger *l){
    /*fname is ignored if mode is MODE_CONS or MODE_SYSLOG
     * priority cant be greater than 7
     * log.priority=log.priority & 0x07
     */
    switch(l->mode){
        case MODE_CONS:            
            break;
        case MODE_SYSLOG:
            /*open syslog : ident,log_opt,fac*/
            openlog(l->prog_name,LOGGER_OPT,LOGGER_FAC);            
            break;
        case MODE_FILE:
            /*try to open file*/
            if((l->fp=fopen(l->fname,"a+b"))==NULL){
                fprintf(stderr,"Error in creating file: \n");
                perror(l->prog_name);
                return FALSE;
            }
            break;
        default :
            /*if an invalid mode then use syslog*/
            l->mode=MODE_SYSLOG;
            break;
    }
   /*only extrem case of error such as direcory or file not writeble will be reported as FATAL
    * and log_init() will fail 
    */
    return TRUE;
}
void log_msg(logger l,const char*msg){
    /*ready the output string */
    
    time_t the_time;
    char time_buff[256],hostname[256];
    int len=0;
    gethostname(hostname,256);
    (void) time(&the_time);           
     strncpy(time_buff,ctime(&the_time),255);
     /*overwrite the newline marker attached with ctime() returnred string */
     len=strlen(time_buff);    
     time_buff[len-1]='\0';
    switch(l.mode){
        case MODE_CONS:           
            fprintf(stderr,"%s %s[%d] @ %s: %s\n",time_buff,l.prog_name,l.pid,hostname,msg);
            break;        
        case MODE_FILE:
            fprintf(l.fp,"%s %s[%d] @ %s: %s\n",time_buff,l.prog_name,l.pid,hostname,msg);
            break;
        default:
            /*default is MODE_SYSLOG*/
            syslog(l.priority,"%s:",msg);
            break;
    }
}
void close_log(logger l){
    switch(l.mode){
        case MODE_CONS:
            /*nothing special but we dont use closelog() in this mode so special case -not same as of MODE_SYSLOG*/
            break;
        case MODE_FILE:
            /*close file descriptor*/
            fclose(l.fp);
            break;
        default:
            /*MODE_SYSLOG is handled here */
            closelog();
            break;
    }
}
int main(){
    logger log;
    strcpy(log.prog_name,"logger_test");
    log.mode=MODE_FILE;
    log.priority=LOG_INFO; /*any one out of seven*/
    strcpy(log.fname,"test.log");
    log.pid=getpid();
    if(log_init(&log)==FALSE){
        sdebug("log_init:FAILED");
        return EXIT_FAILURE;
    }
    log_msg(log,"starting up");
    close_log(log);
    return EXIT_SUCCESS;
}
