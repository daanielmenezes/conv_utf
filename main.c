#include <stdio.h>
#include "conv_utf.h"

int main(void) {
    FILE *input = fopen("utf8_peq.txt", "rb");
    FILE *output = fopen("8to32.txt", "wb");
    utf8_32(input, output);
    fclose(input);
    fclose(output);

    return 0;
}
