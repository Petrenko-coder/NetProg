CXX = g++
CXXFLAGS = -Wall -Wextra -g

all: echo_client

echo_client: echo_client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f echo_client

.PHONY: all clean
