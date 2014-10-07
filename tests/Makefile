LDLIBS := -lgtest -lgtest_main -lpthread
OBJECTS ?= http_server_test.o request_test.o
CFLAGS := -Wall -pedantic

all: $(OBJECTS)
	g++ -o test $(CFLAGS) $(OBJECTS) $(LDLIBS)

clean:
	rm *.o *.out -f
