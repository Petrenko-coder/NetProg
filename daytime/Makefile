CXX = g++
CXXFLAGS = -Wall -Wextra -g

all: daytime_client

daytime_client: daytime_client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f daytime_client

.PHONY: all clean
