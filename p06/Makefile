TARGET=p06a p06b
CXXFLAGS=-Wall -Wextra -pedantic --std=c++23 -O

ALL: test run

.PHONY: test run

${TARGET}: %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: ${TARGET}
	$(foreach name,$^,./${name} < tests;)

run: ${TARGET}
	$(foreach name,$^,./${name} < input;)
