ALL: test run

.PHONY: test run

${TARGET}: %: %.cpp
	$(CXX) --std=c++23 -O3 $< -o $@

test: ${TARGET}
	$(foreach name,$^,./${name} test;)

run: ${TARGET}
	$(foreach name,$^,./${name} input;)
