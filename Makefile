LDLIBS := -lgtest -lgtest_main -lpthread
CXXFLAGS := -Wall -pedantic

test_name = $(TARGET)_test
test_obj = $(test_name).o
test_src = tests/$(test_name).cc
src = src/$(TARGET).cc
src_h = src/$(TARGET).h

all:
	cd src && $(MAKE) && mv sws ../build/ -u

test: $(TARGET); ./test

$(TARGET): $(test_obj)
	$(CXX) $(CXXFLAGS) -o test $(test_obj) $(LDLIBS)

$(test_obj): $(test_src) $(src) $(src_h)
	$(CXX) $(CXXFLAGS) $(LDLIBS) -c $(test_src)

clean:
	rm -f *.o test
	cd src && make clean
