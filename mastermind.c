#include <stdio.h>
#include "mastermind.h"

static DIGIT sequence;
static COUNT hint;

int main(int argc, char **argv) {
	if (argc > 2)
	    error("too many command line arguments...");	
	char c;
	do {
		action(action_game);
		do {
			printf("try it again?(y/n) ");
			fflush(stdin);
			scanf("%c", &c);
		} while (c != 'y' && c != 'n');
	} while (c == 'y');
	if (argc == 2 && (str_toupper(argv[1]), !strcmp(argv[1], "-SHOW") || !strcmp(argv[1], "-S"))) {
		puts("\nshow the history:");
		action(action_show);
	}
	return 0;
}

static void str_toupper(char *s) {
	while (*s) {
		*s = toupper(*(s++));
	}
}

static void action(ACTION n) {
	void (*pf)(void);
	switch (n) {
		case action_game: pf = game; break;
		case action_count: pf = count; break;
		case action_generate: pf = generate; break;
		case action_clear: pf = clear; break;
		case action_clear_hint: pf = clear_hint; break;
		case action_show: pf = show;
	}
	(*pf)();
}

static void check_action(char *s, CHECK n) {
	void (*pf)(char *);
	switch (n) {
		case size: pf = check_digit_size; break;
		case non_digit: pf = check_non_digit; break;
		case duplicate: pf = check_dup;
	}
	(*pf)(s);
}

static void show(void) {
	puts("===============================");
	printf("---- ");
	FILE *fp = fopen(FILENAME, "r");
	char c;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') {
			printf(" ----");
			putchar(c);
			if (fgetc(fp) != EOF) {
				printf("---- ");
				fseek(fp, -1L, SEEK_CUR);
			}
			continue;
		}
		putchar(c);
	}
	puts("===============================");
	fclose(fp);
}

static void game(void) {
	action(action_clear);
	action(action_generate);
	FILE *fp = fopen(FILENAME, "w");
	char input[DIG_SIZE + 1];
	do {
		action(action_clear_hint);
		puts("please enter the digital sequence:");
		scanf("%s", input);
		check(input);
		fputs(strcat(input, "\n"), fp);
		convert(input);
		action(action_count);
		printf("include: %d\nhit: %d\n\n", hint.blow + hint.hit, hint.hit);
	} while (hint.hit != 4);
	puts("\n===============================\nnice! the digital sequence is:");
	int i;
	for (i = 0; i < DIG_SIZE; i++) {
		printf("%d", sequence.result[i]);
	}
	puts("\n===============================");
	fclose(fp);
}

static void convert(char *s) {
	int i;
	char c;
	for (i = 0; i < DIG_SIZE; i++) {
		c = s[i];
		sequence.input[i] = atoi(&c);
	}
}

static void count(void) {
	int i, j, *result = sequence.result, *input = sequence.input;
	for (i = 0; i < DIG_SIZE; i++) {
		for (j = 0; j < DIG_SIZE; j++) {
			if (result[i] == input[j]) {
				if (i == j) {
					hint.hit++;
					continue;
				}
				hint.blow++;
			}
		}
	}
}

static void check(char *s) {
	CHECK n;
	for (n = size; n <= duplicate; n++) {
		check_action(s, n);
	}
}

static void check_digit_size(char *s) {
	if (strlen(s) != 4) {
		error("wrong size of digital sequence...");
	}
}

static void check_non_digit(char *s) {
	char c;
	while (c = *(s++)) {
		if (!isdigit(c)) {
			error("the sequence has non-digital character...");
		}
	}
}

static void check_dup(char *s) {
	int length = strlen(s), i, j;
	for (i = 0; i < length - 1; i++)
	    for (j = i + 1; j < length; j++)
	        if (s[i] == s[j])
	            error("the sequence has duplicate character...");
}

static void error(char *message) {
	fputs("\a\nerror: ", stderr);
	fputs(message, stderr);
	fputc('\n', stderr);
	action(action_clear);
	exit(EXIT_FAILURE);
}

static void generate(void) {
	int i, value;
	srand(time(NULL));
	for (i = 0; i < DIG_SIZE; i++) {
		do {
			value = rand() % 10;
		} while (is_dup(value, i));
		sequence.result[i] = value;
	}
}

static bool is_dup(int n, int count) {
	int i;
	for (i = 0; i < count; i++) {
		if (sequence.result[i] == n) {
			return true;
		}
	}
	return false;
}

static void clear_hint(void) {
	hint.blow = 0;
	hint.hit = 0;
}

static void clear(void) {
	FILE *fp = fopen(FILENAME, "w");
	fputs("", fp);
	fclose(fp);
	int i;
	for (i = 0; i < DIG_SIZE; i++) {
		sequence.input[i] = 0;
		sequence.result[i] = 0;
	}
	action(action_clear_hint);
}
