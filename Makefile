LDLIBS := -lgtest -lgtest_main -lpthread
CXXFLAGS := -Wall -pedantic

test_name = $(TARGET)_test
test_src = tests/$(test_name).cc
VPATH = src:tests
src = response.cc http_server.cc
srcdir = src
program = sws
main = $(srcdir)/$(program).cc
srcproj = $(addprefix $(srcdir)/, $(src))

all: $(program)

$(program): $(srcproj) $(main)
	$(CXX) $(CXXFLAGS) -o $(program) $(main) $(srcproj)

.PHONY: test

test:
	$(CXX) $(CXXFLAGS) -o test $(test_src) $(srcproj) $(LDLIBS)

clean:
	rm -f *.o test
	cd src && make clean
