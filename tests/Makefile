OBJFILES := http_test.o
CFLAGS := -g

check: convert $(OBJFILES)
	cc -o check $(OBJFILES) $(CFLAGS) $(LDFLAGS) `pkg-config --cflags --libs check`

convert:
	checkmk http_test.check > http_test.c

clean:
	rm -f *.c *.o
