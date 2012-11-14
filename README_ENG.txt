This application (below "cron") is for running any .hta 
application and re-running of this application after closing.
The main task is to keep running .hta (for Windows OS only)

System demands:
1. Environment variable TERMPATH should be set up, it should contain 
   path to the directory with settings files (closing slash must be present).
   In case if TERMPATH variable is absent then default working directory is:
   c:\Documents and Settings\< USER >\Application Data\Term
   (if there is `c:\Documents and Settings\< USER >\Application Data` no 
   Term directory, it will be created automatically).
   In case when TERMPATH variable contains the path to non-existent directory
   - the application won't start, error message will be written into cron.log
   in the same directory where cron.exe is.

2. Two files(cron.app, cron.log) must be presented in the directory from 
   TERMPATH variable.
   In the first string of cron.app the path to the .hta application must 
   be presented, cron.log is a logging file.
   
Working flow:
1. "cron" starts and checks if TERMPATH, cron.app and cron.logand exists 
   (without cron.app .hta application won't start and "cron" will be stopped).

2. Running of .hta, mentioned in cron.app.

3. Re-running .hta after closing.

Restarting of the application:
For restarting of the .hta - cron.rst file(empty is available) should be 
created in TERMPATH.  
After finding cron.rst .hta will be restarted shortly.
If cron.rst is empty then .hta will be restarted in 1000 milliseconds, else 
it will be restarted in milliseconds mentioned within the file. 
If it's impossible to get the number of milliseconds from the file
then .hta will be restarted in 1000 milliseconds.
cron.rst will be deleted automatically.
   
Stopping of the application:
For stopping of the .hta - cron.stp file(empty is available) should be 
created in TERMPATH. As soon as cron.stp file will be found both
.hta and "cron" will stop.

Delayed start:
In default .hta will start in 1000 milliseconds after "cron" starting.
If cron.rst found then .hta start will be delayed for number of 
milliseconds mentioned cron.rst
