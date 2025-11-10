CXX = clang++
CXXFLAGS = -std=c++20 -I./include

srcs := $(shell find src -name '*.cpp')
objs := $(srcs:.cpp=.o)

lib: $(objs)
	ar rcs libsomething.a $(objs)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

