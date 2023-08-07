CC = gcc
CC_FLAGS=-Wall -Wextra -Werror -std=c11

.PHONY: clean

all: s21_cat

s21_cat: clean
	gcc ${CC_FLAGS} s21_cat.c -o s21_cat

s21_cat_test: s21_cat
	chmod +x test.sh
	./test.sh

clean:
	rm -f s21_cat
	rm -f t1.txt
	rm -f t2.txt

leaks:
	make test
	leaks -atExit -- ./s21_cat | grep LEAK:
	make clean
	
linter: 
	python3 ~/projects/C3_SimpleBashUtils-0/materials/linters/cpplint.py --extensions=c *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem  *.c *.h
