#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "split_HTML.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s --max-len=<length> <path_to_HTML>\n", argv[0]);
        return -1;
    }

    int max_len;
    if (strncmp(argv[1], "--max-len=", 10) == 0) {
        max_len = atoi(argv[1] + 10);
    }
    else {
        printf("Invalid length argument. Use --max-len=<length>\n");
        return -1;
    }

    char* path_to_HTML = argv[2];
    FILE* HTML = fopen(path_to_HTML, "r");
    if (HTML == NULL) {
        printf("Could not open file: %s\n", path_to_HTML);
        return -1;
    }

    int result = split_HTML(HTML, max_len);
    if (result == 1) {
        printf("HTML split successfully.\n");
    }
    else {
        printf("An error occurred while splitting HTML.\n");
    }

    fclose(HTML);
    return 0;
}
