# SRC_DIR = src
# HEADERS = $(wildcard $(SRC_DIR)/*.h)

.PHONY: directories clean stat

all: directories bin/ut_all

bin/ut_all: test/ut_all.cpp test/file_test.h test/folder_test.h src/node.h src/file.h src/folder.h src/iterator.h src/null_iterator.h src/dfs_iterator.h iterator.o
	g++ -g -std=c++11 test/ut_all.cpp obj/iterator.o -o bin/ut_all -lgtest -lpthread

iterator.o: src/iterator.h src/iterator.cpp src/dfs_iterator.h
	g++ -g -std=c++11 -c src/iterator.cpp -o obj/iterator.o

directories:
	mkdir -p bin obj

clean:
	rm -f bin/* obj/*

stat:
	wc src/* test/*

