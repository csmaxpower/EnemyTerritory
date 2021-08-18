gravystats 0.13
===============

requires: Java Runtime Environment (JRE) 1.4.2 or later - http://www.java.com

usage:
------
extract gravystats.jar to your etpro folder.
Set the ET cvar 'logfile' to "2" to enable console logging.
[Optional] Use the following to enable statsall/revives logging
/seta b_endround "statsall"

After playing a game, double click on the gravystats.jar file. Files will be created inside subdirectory /etpro/gravystats.

upgrading:
----------
IMPORTANT - If you are updating from a previous version, delete the /etpro/gravystats/include directory, the latest files will be generated next time the program is run. Remember to upload these as well!

log file backup:
----------------
if you'd like to use the logfile backup option, create a new directory /etpro/gravystats/backup/. When the stats are generated, the logfile will be copied here with the same name as the output html.

changes in this version:
------------------------
darkened yellow colour, better on white background
added team kill to simple stats display
(hopefully) fixed international locale bug with date parsing
added statsall parsing + revive display
added 'doogie howser' award for most revives
multiple recipients for most weapons kills/deaths now display. more then 3 will display as 'multiple'.
fixed null error when mapnames arent logged

changelog history, support + feedback available here:
http://www.gameitis.com/phpBB/viewforum.php?f=17

cheers
blakjack aka gravy