# include <stdio.h>

void report_error (int line_num, char *where, char *message) {
    fprintf(stderr, "[line %i] Error %s: %s\n", line_num, where, message);
}

void error (int line_num, char *message) {
    return report_error(line_num, "", message);
}

void main () {
    error(1, "compile");
}
