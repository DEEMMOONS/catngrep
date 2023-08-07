/* Copyright brandtla */
#include "s21_grep.h"

void s21_to_lower(char *str);
int regular(struct flags grep_flags, char *str, char *str2, int *error,
    int index, char **argv, int *str_count, int count);
int print_no_e(char *str, char *str2, int *str_count,
    struct flags grep_flags, char** argv, int index, int count);

int main(int argc, char* argv[]) {
    int error = 0;
    if (argc < 3)
        error = 1;

    struct flags grep_flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ""};

    if (!error) {
        while (error != 1) {
            char c;
            c = getopt(argc, argv, "+ivclnhsf:oe:");
            if (c == -1)
                break;
            switch (c) {
            case 'e':
                grep_flags.e = 1;
                if (!strlen(grep_flags.patt)) {
                    snprintf(grep_flags.patt, Max, "%s", optarg);
                } else {
                    snprintf(grep_flags.patt + strlen(grep_flags.patt),
                                Max - strlen(grep_flags.patt), "|%s", optarg);
                }
                break;
            case 'i':
                grep_flags.i = 1;
                break;
            case 'v':
                grep_flags.v = 1;
                break;
            case 'c':
                grep_flags.c = 1;
                break;
            case 'l':
                grep_flags.l = 1;
                break;
            case 'n':
                grep_flags.n = 1;
                break;
            case 'h':
                grep_flags.h = 1;
                break;
            case 's':
                grep_flags.s = 1;
                break;
            case 'f':
                grep_flags.f = 1;
                grep_flags.e = 1;
                FILE *f;
                if ((f = fopen(optarg, "r")) != NULL) {
                    while (getc(f) != EOF) {
                        fseek(f, -1, SEEK_CUR);
                        if (!strlen(grep_flags.patt)) {
                            fgets(grep_flags.patt, Max, f);
                            strtok(grep_flags.patt, "\n");
                        } else {
                            strcat(grep_flags.patt + strlen(grep_flags.patt), "|");
                            fgets(grep_flags.patt + strlen(grep_flags.patt),
                                Max - strlen(grep_flags.patt), f);
                            strtok(grep_flags.patt, "\n");
                        }
                    }
                    fclose(f);
                } else {
                    fprintf(stderr, "file reading error\n");
                    error = 1;
                }
                break;
            case 'o':
                grep_flags.o = 1;
                break;
            default:
                fprintf(stderr,
                    "usage: grep [-ivclnhsfoe] [template] [file ...]\n");
                error = 1;
                break;
            }
        }
        argv += optind;
        argc -= optind;
    }

    if (grep_flags.v == 1)
        grep_flags.o = 0;
    if (!error) {
        if (argc > 1 && grep_flags.e == 0 && grep_flags.f == 0) {
            snprintf(grep_flags.patt, Max, "%s", *argv);
            argv++;
            argc--;
        }
        int index = 0;
        if (argc > 1)
            index = 1;
        if (grep_flags.i == 1)
            s21_to_lower(&(grep_flags.patt[0]));
        while (argc--)
            s21_grep(index, grep_flags, argv++);
    }
    return 0;
}

void s21_grep(int index, struct flags grep_flags, char** argv) {
    FILE* fp = NULL;
    int error = 0, str_count = 0, stat = 0;
    if ((fp = fopen(*argv, "r")) == NULL) {
        if (grep_flags.s == 0)
            fprintf(stderr, "grep: %s: No such file or directory\n", *argv);
        error = 1;
    } else if (getc(fp) == EOF) {
        error = 1;
    }
    if (!error) {
        int count = 0;
        char str[2049], str2[2049];
        do {
            count++;
            fseek(fp, -1, SEEK_CUR);
            fgets(str, 2049, fp);
            strcpy(str2, str);
            if (grep_flags.i == 1)
                s21_to_lower(str);
            if (grep_flags.e == 1) {
                if ((regular(grep_flags, str, str2, &error,
                    index, argv, &str_count, count) && grep_flags.v == 0) ||
                    (grep_flags.v == 1 && !(regular(grep_flags,
                        str, str2, &error, index, argv, &str_count, count)))) {
                    stat++;
                }
            } else {
                stat += print_no_e(str, str2, &str_count, grep_flags, argv, index, count);
            }
        } while (getc(fp) != EOF && !error);
    }
    if (grep_flags.c == 1) {
        if (index && grep_flags.h == 0 && grep_flags.l == 0)
            printf("%s:", *argv);
        if (grep_flags.l == 1 && str_count)
            str_count = 1;
        printf("%d\n", str_count);
    }
    if (grep_flags.l == 1 && stat) {
        printf("%s\n", *argv);
    }
    if (fp != NULL)
        fclose(fp);
}

int print_no_e(char *str, char *str2, int *str_count,
    struct flags grep_flags, char** argv, int index, int count) {
    int ok = 0;
    if ((strstr(str, grep_flags.patt) && grep_flags.v == 0)
        ||(grep_flags.v == 1 && !(strstr(str, grep_flags.patt)))) {
        ok = 1;
        *str_count += 1;
        if (grep_flags.l == 0) {
            if (grep_flags.c == 0) {
                if (index && grep_flags.h == 0)
                    printf("%s:", *argv);
                if (grep_flags.n == 1)
                    printf("%d:", count);
                if (grep_flags.o == 1 && grep_flags.e == 0) {
                    char *c, *c2;
                    char cp[2049];
                    c = strstr(str, grep_flags.patt);
                    strncpy(cp, str2 + (c - str), strlen(grep_flags.patt));
                    printf("%s\n", cp);
                    c++;
                    while ((c2 = strstr(c, grep_flags.patt)) != NULL) {
                        strncpy(cp, str2 + (c2 - str), strlen(grep_flags.patt));
                        printf("%s\n", cp);
                        c = c2 + 1;
                    }
                } else {
                    printf("%s", str2);
                }
            }
        }
    }
    return ok;
}

int regular(struct flags grep_flags, char *str, char *str2,
    int *error, int index, char **argv, int *str_count, int count) {
    int check = 0, reg;
    regex_t patt;
    regmatch_t matchptr[1];
    const size_t nmatch = 1;
    reg = regcomp(&patt, grep_flags.patt, REG_EXTENDED);
    if (reg) {
        fprintf(stderr, "wrong regular\n");
        *error = 1;
    } else {
        check = regexec(&patt, str, nmatch, matchptr, 0);
        if (!(check)) {
            *str_count += 1;
            if (grep_flags.l == 0) {
                if (grep_flags.c == 0) {
                    if (index && grep_flags.h == 0)
                        printf("%s:", *argv);
                    if (grep_flags.n == 1)
                        printf("%d:", count);
                    if (grep_flags.o == 1) {
                        for (int i = matchptr[0].rm_so; i < matchptr[0].rm_eo; i++)
                            putchar(str2[i]);
                        printf("\n");
                        int fin = matchptr[0].rm_eo;
                        while (!(regexec(&patt, (str + fin), nmatch, matchptr, 0)) && *(str + fin) != '\n') {
                            for (int i = fin + matchptr[0].rm_so; i < fin + matchptr[0].rm_eo; i++)
                                putchar(str2[i]);
                            fin +=matchptr[0].rm_eo;
                            printf("\n");
                        }
                    } else {
                        printf("%s", str2);
                    }
                }
            }
        }
        regfree(&patt);
    }
    return (check == 0) ? 1 : 0;
}

void s21_to_lower(char *str) {
    if (str) {
        char* tmp_str = str;
         while (*tmp_str != '\0') {
            if (*tmp_str >= 'A' && *tmp_str <= 'Z')
                *tmp_str += 32;
            tmp_str++;
        }
    }
}

