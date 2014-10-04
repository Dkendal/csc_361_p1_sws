LDLIBS := -lgtest -lgtest_main -lpthread
OBJFILES := http_server_test.o request_test.o
CFLAGS := -Wall -pedantic

all: $(OBJFILES)
	g++ -o test $(CFLAGS) $(OBJFILES) $(LDLIBS)

clean:
	rm *.o *.out -f
