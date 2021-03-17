# include <stdio.h>
# include <stdlib.h>

void run_file(char *file_name) {
    printf("Executing %s\n", file_name);
}

void repl() {
    char *input_line = NULL;
    ssize_t input_size = 0;
    ssize_t input_bytes;
    
    while (1) {
        printf("remo>> "); // repl prompt
        input_size = getline(&input_line, &input_size, stdin);
        if (input_size == -1) return;
        printf("%s", input_line); // input_line is newline terminated
    }
    free(input_line);
}

int main (int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: remo [script]\n");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        repl();
    }
}
    
        
