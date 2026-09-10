# Spaghetti-Graveyard-UI

Note: I am using this distro as a USB drive to switch between my laptop and 
my workstation, as such this work in progress will likely not compile and 
run properly.

Spaghetti Graveyard Zero will be the Nth iteration of a project I started around the year 2018.
The project is an action RPG game that quizzes you on mathematics skills (numeracy).

Spaghetti - Because I did not know much about how write well-structured code at the start of this project.
Graveyard - Because since learning how to write better code, I have discarded much of the code that I had written.
Zero      - Because this is very much a work in progress and so comes before release 1.0

#define BB_CSFML3 to compile with BB_CSFML3, 
compile for CSFML2.6.1 otherwise

To run the profile on Ubuntu, I ran the following commands separately:

sudo sh -c 'echo 1 > /proc/sys/kernel/perf_event_paranoid'
sudo sh -c 'echo 0 > /proc/sys/kernel/kptr_restrict'

To check that that worked, I ran

cat /proc/sys/kernel/perf_event_paranoid

it should return 1