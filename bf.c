#include <stdio.h>
#include <string.h>

#define  INSTRUCTIONS_SIZE  100000
#define  MEMORY_SIZE        30000
#define  MAXIMUM_LOOPS      10000
#define  PARSE_STACK_SIZE   10000

const char bfinstructions[] = "+-<>[].,";

char *next_instruction(char *, char *[][2]);
int get_valid_input(char *, int);
void get_loops(char *, char *[][2]);
char *get_start_loop(char *, char *[][2]);
char *get_end_loop(char *, char *[][2]);


int main(int argc, char *argv[]) {
    char instructions[INSTRUCTIONS_SIZE];
    char *loops[MAXIMUM_LOOPS][2];
    char *ins;

    ins = instructions;
    if (get_valid_input(instructions, INSTRUCTIONS_SIZE) > 0) {
        get_loops(instructions, loops);

        do
            ins = next_instruction(ins, loops);
        while (*ins++ != '\0');
    }

    return 0;
}


char *next_instruction(char *ins, char *loops[][2]) {
    static unsigned char memory[MEMORY_SIZE];
    static char *memp = memory;

    switch (*ins) {
        case '>':
            memp++;
            break;
        case '<':
            memp--;
            break;
        case '+':
            (*memp)++;
            break;
        case '-':
            (*memp)--;
            break;
        case '.':
            putchar(*memp);
            break;
        case ',':
            *memp = getchar();
            break;
        case '[':
            if (!*memp)
                ins = get_end_loop(ins, loops);
            break;
        case ']':
            ins = get_start_loop(ins, loops)-1;
            break;
    }

    return ins;
}


int get_valid_input(char *input, int max) {
    int i, c;

    for (i = 0; i < max-1 && (c = getchar()) != EOF; i++)
        if (strchr(bfinstructions, c))
            *input++ = c; 
    
    *input = '\0';
    return i;
}


void get_loops(char *instructions, char *loops[][2]) {
    char *stack[PARSE_STACK_SIZE], **sp;
    char ins;
    int i;

    i = 0;
    sp = stack;
    while ((ins = *instructions++) != '\0') {
        if (ins == '[')
            *sp++ = instructions-1;
        else if (ins == ']') {
            loops[i][0] = *--sp; 
            loops[i++][1] = instructions-1;
        }
    }
}


char *get_start_loop(char *end, char *loops[][2]) {
    char **loop;

    do {
        loop = (char **)*loops;

        if (loop[1] == end)
            return loop[0];
    } while (loops++);
}


char *get_end_loop(char *start, char *loops[][2]) {
    char **loop;

    do {
        loop = (char **)*loops;

        if (loop[0] == start)
            return loop[1];
    } while (loops++);
}
