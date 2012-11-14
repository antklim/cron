void slep ()
{
	if ( rstm ) {
		Sleep ( rstm ) ;
		rstm = 0 ;
	} else 	Sleep ( BRTM ) ;
}

int flex ( char* a )
{
	DWORD f ;

    f = GetFileAttributes ( a ) ;
    if ( 0xFFFFFFFF == f ) return 0 ;
		
    return 1 ;
}

int drex ( char* a ) 
{
	DWORD f ;
	
	f = GetFileAttributes ( a ) ;
	if ( 0xFFFFFFFF == f ) return 0 ;
	
	return 1 ;
}

int wlog ( char* a )
{
	FILE* f ;

	pid = GetCurrentProcessId () ;
	time ( &t ) ;
	ti = localtime ( &t ) ;

	f = fopen ( lgfl , "a+" ) ;
	if ( f == NULL ) return CEX001 ;
	fprintf( f , "%4d-%02d-%02d  %02d:%02d:%02d  PID %d %s\n" , 
		( 1900 + ti->tm_year ) , ( ti->tm_mon + 1 ) , ti->tm_mday , 
		ti->tm_hour , ti->tm_min , ti->tm_sec , pid , a ) ;
	fclose( f ) ;
	return CEX000 ;
}

char* flrd ( char* d ) 
{
	char* r ; FILE* f ;
	int i = 0 ; char c ;
	
	f = fopen ( d , "r" ) ;
	if ( f == NULL ) return NULL ;
	c = fgetc ( f ) ;
	if ( c == EOF ) {
		fclose ( f ) ;
		return NULL ; 
	}
	r = ( char* ) malloc ( sizeof ( char ) ) ;
	while ( c != EOF ) {
		r [ i ] = c ;
		c = fgetc ( f ) ;
		i++ ;
		if ( c != feof ( f ) ) r = realloc ( r , ( i + 1 ) * sizeof ( char ) ) ;
	}
	fclose ( f ) ;
	return r ;
}

int mkpt ()
{
	char* p ; TCHAR s [ MAX_PATH ] ;
	p = getenv ( EVAR ) ;
	if ( p == NULL ) {
		if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE , 
			NULL , 0 , s ) ) ) {	
			p = ( char* ) malloc ( strlen ( s ) + 6 ) ;
			sprintf ( p , "%s\\Term\\" , s ) ;
			if ( drex ( p ) == 0 ) {
				if ( !CreateDirectory ( p , NULL ) ) return CEX008 ;
			}
		}
		else return CEX006 ;
	} else {
		if ( drex ( p ) == 0 ) return CEX009 ;
	}
	
	sprintf ( lgfl , "%s\%s%s" , p , PREF , lgnm ) ;
	sprintf ( stfl , "%s\%s%s" , p , PREF , stnm ) ;
	sprintf ( apfl , "%s\%s%s" , p , PREF , apnm ) ;
	sprintf ( rsfl , "%s\%s%s" , p , PREF , rsnm ) ;

	free ( p ) ;
	
	return 0 ;
}

int gtrp ()
{
	char* p ; TCHAR s [ MAX_PATH ] ;
	p = getenv ( EVAR ) ;

	if ( p == NULL ) return 1 ;		
	else if ( drex ( p ) == 0 ) return 1 ;
	
	free ( p ) ;
	
	return 0 ;
}

int gcrd ( char* a ) 
{
   if ( SUCCEEDED ( GetCurrentDirectory ( MAX_PATH , a ) ) ) {
      return 0 ;
  } 
   
   return CEX010 ;   
}

int gapp ( char* d ) 
{
	FILE* f ;
	
	f = fopen ( apfl , "r" ) ;
	if ( f == NULL ) return CEX002 ;
	fgets ( d , APLN , f ) ;
	return CEX000 ;
}

int grst ()
{
	char* d ; int i = 0 ;
	
	if ( flex ( rsfl ) ) d = flrd ( rsfl ) ;
	i = atoi ( d ) ;
	if ( i ) rstm = i ;
	
	return CEX000 ;
}

BOOL CALLBACK eprc ( HWND h , DWORD l )
{
    DWORD p = 0;

    if ( !GetWindowThreadProcessId ( h , &p ) ) return TRUE ;
    if ( l == p ) PostMessage ( h , WM_CLOSE , 0 , 0 ) ;
    return TRUE ;
}

void stpr ( HANDLE h , DWORD p )
{
	char m [ 512 ] ;

	EnumWindows ( ( WNDENUMPROC ) eprc , (LPARAM) p ) ;
	
//	if ( ( TerminateProcess ( h , 0 ) ) == 0 ) {
//		sprintf ( m , "ERR Terminate process %d , error: %d" , 
//			p , GetLastError () ) ;
//		wlog ( m ) ;			
//	}
}

void stth ( HANDLE h , DWORD p )
{
	char m [ 512 ] ;
	if ( ( TerminateThread ( h , 0 ) ) == 0 ) {
		sprintf ( m , "ERR Terminate thread %d , error: %d" , 
			p , GetLastError () ) ;
		wlog ( m ) ;			
	};
}

void lapp ()
{
	char m [ 512 ] ; 
	char b [ APLN ] ;
	char l [ APLN ] ;
	int r = 0 ;
	
	sprintf ( m , "RUN LAPP THRD. TID %d" , td2 ) ;
	wlog ( m ) ;

	l01 :
	r = gtrp () ;
	if ( r ) {
	   wlog ( "MSG Can't locate TERMPATH directory" ) ;	
	   slep () ; goto l01 ;
	}
	
	r = mkpt () ;
	if ( r ) {
		printf ( "exit %d\n" , r ) ;
		sprintf ( lgfl , ".\\%s%s" , PREF , lgnm ) ;
		wlog ( "ERR Environment variable TERMPATH or CSIDL_APPDATA not found" ) ;
		return ;
	}	
	
	l02 :
	r = gapp ( b ) ;
	if ( gapp ( b ) ) {
	   wlog ( "MSG Can't find app file" ) ;	
	   slep () ; goto l02 ;
	}

	l03 :
	if ( flex ( stfl ) ) {
	    wlog ( "MSG Stop file was found" ) ;
		slep () ; goto l03 ;		
	}
	
	sprintf ( l , "mshta.exe %s" , b ) ;

	slep () ;
	
	ZeroMemory ( &si , sizeof ( si ) ) ;
	si.cb = sizeof ( si ) ;
	ZeroMemory ( &pi , sizeof ( pi ) ) ;
	
	l00 :
	if ( !CreateProcess ( NULL ,
		l ,
		NULL ,
		NULL ,
		FALSE ,
		0 ,
		NULL ,
		NULL ,
		&si ,
		&pi ) ) {
		sprintf ( m , "ERR Create process failed (%d)" , GetLastError () ) ;		
		wlog ( m ) ; wlog ( "END" ) ;
		return ;
	}
	pa = 1 ;
	sprintf ( m , "RUN PROC. CID %d" , pi.dwProcessId ) ;
	wlog ( m ) ;

	WaitForSingleObject ( pi.hProcess , INFINITE ) ;
	
	sprintf ( m , "END PROC. CID %d" , pi.dwProcessId ) ;
	wlog ( m ) ;

	CloseHandle ( pi.hProcess ) ;
	CloseHandle ( pi.hThread ) ;
	
	l99 :
	slep () ; goto l03 ;

	sprintf ( m , "END THRD. TID %d" , td2 ) ;
	wlog ( m ) ;
}

void capp ()
{
	char d [ APLN ] ; char m [ 512 ] ; 
	FILE* f ; int r ;
	
	sprintf ( m , "RUN CAPP THRD. TID %d" , td1 ) ;
	wlog ( m ) ;

	while ( 1 ) {
		if ( flex ( stfl ) ) {
			if ( pa ) { stpr ( pi.hProcess , pi.dwProcessId ) ; pa = 0 ; }			

			sprintf ( m , "END LAPP THRD. TID %d" , td2 ) ;
			wlog ( m ) ;
		}
		
		if ( flex ( rsfl ) ) {
			r = grst ( d ) ;
			
			sprintf ( m , "RST PROC. CID %d" , pi.dwProcessId ) ;
			wlog ( m ) ;
//			if ( pi.dwProcessId ) stpr ( pi.hProcess , pi.dwProcessId ) ;
			if ( pa ) { stpr ( pi.hProcess , pi.dwProcessId ) ; pa = 0 ; }
					
			if ( DeleteFile ( rsfl ) == 0 ) {
				sprintf ( m , "Can't delete rst file, error %d" , GetLastError () ) ;
				wlog ( m ) ;
			}		
		}
		Sleep ( SLTM ) ;
	}
	
	sprintf ( m , "END CAPP THRD. TID %d" , td1 ) ;
	wlog ( m ) ;
}
