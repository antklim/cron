#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <shlobj.h>

#define EVAR "TERMPATH"
#define SLTM 1000
#define BRTM 1000
#define APLN 100
#define THCN 2
#define PREF "cron"

char lgnm [ ] = { ".log" } ;
char stnm [ ] = { ".stp" } ;
char apnm [ ] = { ".app" } ;
char rsnm [ ] = { ".rst" } ;

char lgfl [ 100 ] ;
char stfl [ 100 ] ;
char apfl [ 100 ] ;
char rsfl [ 100 ] ;

time_t t ;
struct tm* ti ;
DWORD pid = 0 ;

STARTUPINFO si ;
PROCESS_INFORMATION pi ;
HANDLE thr [ THCN ] ;
DWORD td1 ;
DWORD td2 ;

int pa   = 0 ; // *.hta process alive flag; 0 - process is died, 1 - process is alive
int rstm = 0 ;

static UINT pMsg = 0;

#define CEX000 0
#define CEX001 1
#define CEX002 2
#define CEX003 3
#define CEX004 4
#define CEX005 5
#define CEX006 6
#define CEX007 7
#define CEX008 8
#define CEX009 9
#define CEX010 10
