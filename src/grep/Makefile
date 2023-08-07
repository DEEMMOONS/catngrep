CC = gcc
CC_FLAGS=-Wall -Wextra -Werror -std=c11
TEMP="f"
FILE=s21_grep.c
FLAGS= 
.PHONY: clean

all: s21_grep

s21_grep: clean
	gcc ${CC_FLAGS} s21_grep.c -o s21_grep

s21_grep_test: s21_grep
	chmod +x test.sh
	./test.sh

clean:
	rm -f s21_grep
	rm -f t1.txt
	rm -f t2.txt

leaks:
	make test
	leaks -atExit -- ./s21_grep | grep LEAK:
	make clean
	
linter: 
	python3 ~/projects/C3_SimpleBashUtils-0/materials/linters/cpplint.py --extensions=c *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
