LDLIBS := -lgtest -lgtest_main -lpthread
OBJECTS :=
CFLAGS := -Wall -pedantic
VPATH := ../src
SRC := $(OBJECTS:_test.o=.cc)

all: $(OBJECTS) $(SRC)
	g++ $(CFLAGS) -o test $(OBJECTS) $(LDLIBS)

clean:
	rm *.o *.out -f
