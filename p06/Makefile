TARGET=p06a p06b

ALL: test run

.PHONY: test run

${TARGET}: %: %.cpp
	$(CXX) --std=c++23 -O3 $< -o $@

test: ${TARGET}
	$(foreach name,$^,./${name} < tests;)

run: ${TARGET}
	$(foreach name,$^,./${name} < input;)
