TARGET=p15a p15b
CXXFLAGS=-Wall -Wextra -pedantic --std=c++23 -O

ALL: test run

.PHONY: test run

${TARGET}: %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: ${TARGET}
	$(foreach name,$^,./${name} test 10;)

run: ${TARGET}
	$(foreach name,$^,./${name} input 2000000;)
