#include <stdio.h>
#include <stdlib.h>
#include "base64.h"


int main(void)
{
    char *test_string = "abcdefghijklmn";
    int i = 8;
    char * result;

    for(i = 0; i < 8; i++) {
        result = base64_encode(test_string, i);
        printf("test string size: %d, result: %s\n", i, result);
        printf("decode result: %s\n", base64_decode(result));
        free(result);
    }

    return 0;
}



