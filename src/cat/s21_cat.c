/* Copyright brandtla */
#include "s21_cat.h"

int main(int argc, char* argv[]) {
    int error = 0;

    if (argc < 2)
        error = 1;

    struct flags cat_flags = { 0 };

    static struct option gnu_flags[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number",          0, NULL, 'n'},
        {"squeeze-blank",   0, NULL, 's'},
        {NULL,              0, NULL,  0 },
    };


    if (!error) {
        while (error != 1) {
            int index = 0;
            char c;
            c = getopt_long(argc, argv, "+beEnstTv", gnu_flags, &index);

            if (c == -1)
                break;
            switch (c) {
            case 'b':
                cat_flags.b = 1;
                break;
            case 'e':
                cat_flags.e = 1, cat_flags.v = 1;
                break;
            case 'n':
                cat_flags.n = 1;
                break;
            case 's':
                cat_flags.s = 1;
                break;
            case 't':
                cat_flags.t = 1, cat_flags.v = 1;
                break;
            case 'v':
                cat_flags.v = 1;
                break;
            case 'T':
                cat_flags.t = 1;
                break;
            case 'E':
                cat_flags.e = 1;
                break;
            default:
                fprintf(stderr,
                    "usage: cat [-beEnstTv] [file ...]\n");
                error = 1;
                break;
            }

            if (cat_flags.b)
                cat_flags.n = 0;
        }
    }

    if (!error) {
        do {
            argv++;
            argc--;
        } while (**argv == '-' && argc > 1);
       int line = 0;
        while (argc--)
            line = s21_cat(cat_flags, argv++, line);
    }

    return 0;
}

int s21_cat(struct flags cat_flags, char** argv, int first_empty_line) {
    FILE* fp = NULL;
    char cur = '\0';
    int error = 0;
        if ((fp = fopen(*argv, "r")) == NULL) {
            fprintf(stderr, "cat: %s: No such file or directory\n", *argv);
            error = 1;
        } else if ((cur = getc(fp)) == EOF) {
            error = 1;
        }

        if (!error) {
            int count = 1;
            if (cat_flags.n || (cat_flags.b && cur != '\n'))
                printf("%6d\t", count++);
            char prev = '\0';

            do {
                if (first_empty_line && cur == '\n') {
                    prev = cur;
                    continue;
                } else {
                    first_empty_line = 0;
                }

                if (cat_flags.s && (prev == '\n' || prev == '\0') && cur == '\n') {
                    first_empty_line += 1;
                }

                if (prev == '\n') {
                    if (cat_flags.n || (cat_flags.b && cur != '\n'))
                        printf("%6d\t", count++);
                }

                if (cat_flags.e && cur == '\n')
                    putchar('$');

                if (cat_flags.v && cur >= 0 && cur < 32 && cur != '\n' && cur != '\t')
                    printf("^%c", 64 + cur);
                else if (cat_flags.t && cur == '\t')
                    printf("^I");
                else
                    putchar(cur);

                prev = cur;
            } while ((cur = getc(fp)) != EOF);
        }

    if (fp != NULL)
        fclose(fp);
    return first_empty_line;
}
