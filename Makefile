CC = g++
CFLAGS = -std=c++17 -Iinclude -Ithirdparty/span-lite/include -Dspan_FEATURE_MAKE_SPAN -g
TARGETS_DIR = examples/
TARGETS = $(patsubst %.cpp, %, $(shell find $(TARGETS_DIR) -name '*.cpp'))


TEST_LIBS = -lgmock -lgtest -lgtest_main -lpthread -L/usr/lib
TEST_CXX_FLAGS = -std=c++17 -Iinclude -Ithirdparty/span-lite/include -Dspan_FEATURE_MAKE_SPAN -g
TEST_DIR = test/
TEST_TARGETS := $(patsubst %.cpp, %, $(shell find $(TEST_DIR) -name '*_test.cpp'))

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGETS): %: %.cpp
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_TARGETS): % : %.cpp
	$(CC) $(TEST_CXX_FLAGS) -o $@ $< $(TEST_LIBS)

tests: $(TEST_TARGETS)

clean:
	rm -f $(OBJ) $(TARGETS) $(TEST_TARGETS)
