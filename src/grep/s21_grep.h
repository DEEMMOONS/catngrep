/* Copyright brandtla */
#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define Max 1024

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <regex.h>

struct flags {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
    char patt[Max];
};

void s21_grep(int index, struct flags grep_flags, char** argv);

#endif  // SRC_GREP_S21_GREP_H_


