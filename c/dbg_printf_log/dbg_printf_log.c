#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#define EXECUTABLE_NAME "watchdog"
#define NONE            "\033[0m"
#define GRAY            "\033[1;30m"
#define RED             "\033[1;31m"
#define GREEN           "\033[1;32m"
#define YELLOW          "\033[1;33m"
#define BLUE            "\033[1;34m"
#define MAGENTA         "\033[1;35m"

typedef enum {
	DBG_FATAL,	   /** Fatal result */
	DBG_ERROR,     /** Un-expected flow */
	DBG_WARNING,   /** Weird flow */
	DBG_INFO,      /** Tracking flow */
	DBG_DEBUG,
	DBG_TRACE,
	DBG_ALL,
} DBG_LEVEL;

int dbg_print(DBG_LEVEL level, const char *file_name, const char *function_name, int line_nums, const char *format, ...) {

	DBG_LEVEL env_dbg_level = DBG_INFO;				/* As default */
	char *dbgenv = getenv("WATCHDOG_DBG_LEVEL");	/* Usage: export WATCHDOG_DBG_LEVEL=1 */
	if (dbgenv) {
		env_dbg_level = (DBG_LEVEL)atoi(dbgenv);
	}

	/* Check the dbg level is vaild or not */
	if (level < DBG_FATAL || level > DBG_ALL) {
        return 0;
	}
	
	/* DBG LOG visibility */ 
    if (level > env_dbg_level)
        return 0;

	/* EXAMPLE FORMAT: [watchdog][DEBUG] hello_world.cpp: main() @95: hi DBG_DEBUG */
	/* EXECUTABLE NAME */
	fprintf(stdout, GRAY "[%s]" NONE, EXECUTABLE_NAME); 

	/* DBG LEVEL and log color*/
    switch (level) {
        case DBG_INFO:
            fprintf(stdout, GREEN "[INFO] ");
            break;
        case DBG_DEBUG:
            fprintf(stdout, NONE "[DEBUG] ");
            break;
        case DBG_TRACE:
            fprintf(stdout, NONE "[TRACE] ");
            break;
        case DBG_WARNING:
            fprintf(stdout, YELLOW "[WARNING] ");
            break;
        case DBG_ERROR:
            fprintf(stdout, RED "[ERROR] ");
            break;
        case DBG_FATAL:
            fprintf(stdout, RED "[FATAL] ");
            break;
        default:
            fprintf(stdout, YELLOW "Not Match DBG LEVEL, forget to add?");
            break;
    }

	/* FILE NAME, FUNCTION NAME and NUMS of LINE */
    if (file_name != NULL) {
        if (function_name != NULL)
            fprintf(stdout, "%s: %s() @%d: ", file_name, function_name, line_nums);
        else
            fprintf(stdout, "%s: @%d: ", file_name, line_nums);
    }
	
	/* CUSTOM format & arg */
    va_list arg;
    va_start(arg, format);
    vfprintf(stdout, format, arg);
    va_end(arg);

	/* restore the log color */
	fprintf(stdout, NONE); 
    
    return 0;
}

#define DBG_PRINT(level, fmt, args...) \
	dbg_print(level, __FILE__, __func__, __LINE__, fmt, ##args)

int main(void)
{
	DBG_PRINT(DBG_INFO,"Hi, it's %d\n", DBG_INFO);
	DBG_PRINT(DBG_DEBUG,"Hi, it's %d\n", DBG_DEBUG);
	DBG_PRINT(DBG_TRACE,"Hi, it's %d\n", DBG_TRACE);
	DBG_PRINT(DBG_WARNING,"Hi, it's %d\n", DBG_WARNING);
	DBG_PRINT(DBG_ERROR,"Hi, it's %d\n", DBG_ERROR);
	DBG_PRINT(DBG_FATAL,"Hi, it's %d\n", DBG_FATAL);
	return 0;
}
