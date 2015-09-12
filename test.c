#include <stdio.h>
#include "base64.h"


int main(void)
{
    char *test_string = "abcdefghijklmn";
    int i;

    for(i = 0; i < 8; i++)
        printf("test string size: %d, result: %s\n", i, base64_encode(test_string, i));

    return 0;
}



