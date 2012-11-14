#include "maindefs.h"
#include "utilites.h"

int main () 
{
	int i ; int r ; 
	char m [ 512 ] ;

	FreeConsole () ;
	
	sprintf ( lgfl , ".\\%s%s" , PREF , lgnm ) ;
	r = wlog ( "RUN" ) ;
	if ( r ) {
		printf ( "exit %d\n" , r ) ;
		printf ( "ERR Can't write into log file" ) ;
		return CEX001 ;
	}	
    
	sprintf ( stfl , ".\\%s%s" , PREF , stnm ) ;	
	if ( flex ( stfl ) ) {
		printf ( "exit %d\n" , CEX005 ) ;
		sprintf ( m , "ERR Stop file exists" ) ;
		wlog ( m ) ; wlog ( "END" ) ;
		return CEX005 ;
	}
		
	thr [ 0 ] = CreateThread ( NULL ,
		0 ,
		( LPTHREAD_START_ROUTINE ) capp ,
		NULL ,
		0 ,
		&td1 ) ;

    if( thr [ 0 ] == NULL )
    {
		sprintf ( m , "ERR CreateThread error: %d", GetLastError() ) ;
		wlog ( m ) ; wlog ( "END" ) ;
        return CEX003 ;
    }
	
	thr [ 1 ] = CreateThread ( NULL ,
		0 ,
		( LPTHREAD_START_ROUTINE ) lapp ,
		NULL ,
		0 ,
		&td2 ) ;

	if( thr [ 1 ] == NULL )
    {
		sprintf ( m , "ERR CreateThread error: %d", GetLastError() ) ;
		wlog ( m ) ; wlog ( "END" ) ;
        return CEX004 ;
    }

	WaitForMultipleObjects ( THCN , thr , TRUE , INFINITE ) ;
	
	for	( i = 0 ; i < THCN ; i++ )
		CloseHandle ( thr [ i ] ) ;
	
	wlog ( "END" ) ;

	return 0 ;
}
