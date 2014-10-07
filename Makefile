CFLAGS := -g

all:
	cd src && $(MAKE) && mv sws ../build/ -u

test:
	cd tests && $(MAKE) && ./test

clean:
	cd tests && make clean
	cd src && make clean
