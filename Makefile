all:
	cd src && $(MAKE) && mv sws ../build/ -u

check:
	cd tests && $(MAKE) && clear && ./check
