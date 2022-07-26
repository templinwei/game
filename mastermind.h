#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#ifndef DIG_SIZE
    #define DIG_SIZE 4
#endif
#ifndef FILENAME
    #define FILENAME "record.txt"
#endif
typedef enum check_valid {
    size,
    non_digit,
    duplicate
} CHECK;
typedef enum action_mode {
    action_game,
    action_count,
    action_generate,
    action_clear,
    action_clear_hint,
    action_show
} ACTION;
typedef struct {
    int input[DIG_SIZE];
    int result[DIG_SIZE];
} DIGIT;
typedef struct {
    int blow;
    int hit;
} COUNT;
static void game(void);
static void str_toupper(char *);
static void convert(char *);
static void count(void);
static void check_action(char *, CHECK);
static void check(char *);
static void check_digit_size(char *);
static void check_non_digit(char *);
static void check_dup(char *);
static void error(char *);
static void generate(void);
static bool is_dup(int, int);
static void clear(void);
static void clear_hint(void);
static void show(void);
static void action(ACTION);
