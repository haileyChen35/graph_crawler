CC=g++
CFLAGS=-std=c++17 -lcurl -I ~/rapidjson/include
TARGET=bfs_crawler
all: $(TARGET)

$(TARGET): graph.cpp
	$(CC) $(CFLAGS) $< -o $(TARGET)

clean:
	rm -f $(TARGET)