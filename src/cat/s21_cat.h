/* Copyright brandtla */
#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

struct flags {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
};

int s21_cat(struct flags cat_flags, char** argv, int first_empty_line);

#endif  // SRC_CAT_S21_CAT_H_
