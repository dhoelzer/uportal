uportal : uportal.c client_codes.h client_handling.h input.h \
	local_commands.h network.h output.h status_bars.h \
	utility.h windows.h Makefile
	gcc -g -Wall -o uportal uportal.c -lncurses
tar : Makefile uportal.c client_codes.h client_handling.h input.h \
	local_commands.h network.h output.h status_bars.h \
	utility.h windows.h
	tar -cf uportal.tar uportal.c client_codes.h client_handling.h input.h \
	local_commands.h network.h output.h status_bars.h \
	utility.h triggers.h windows.h Makefile
