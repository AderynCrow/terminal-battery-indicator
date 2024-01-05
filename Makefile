terminal-battery-indicator:
	gcc main.c -o terminal-battery-indicator
check:
	test terminal-battery-indicator
install:
	install -m 755 terminal-battery-indicator /usr/bin
