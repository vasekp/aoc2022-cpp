ALL: test run
CXXFLAGS=-Wall -Wextra -pedantic --std=c++23 -O

.PHONY: test run

${TARGET}: %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: ${TARGET}
	$(foreach name,$^,./${name} test;)

run: ${TARGET}
	$(foreach name,$^,./${name} input;)
