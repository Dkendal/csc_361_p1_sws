CFLAGS := -g

all:
	cd src && $(MAKE) && mv sws ../build/ -u

test:
	cd tests && $(MAKE) && ./test

clean:
	cd tests && $(MAKE) clean
	cd src && $(MAKE) clean
