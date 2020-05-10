#include <stdio.h>
#include "conv_utf.h"

int main(void) {
    FILE *input;
    FILE *output;

    /* teste utf8_32 */
    input = fopen("utf8_peq.txt", "rb");
    output = fopen("8to32.txt", "wb");
    utf8_32(input, output);
    fclose(input);
    fclose(output);

    /* teste utf32_8 */
    input = fopen("utf32_peq.txt", "rb");
    output = fopen("32to8.txt", "wb");
    utf32_8(input, output);
    fclose(input);
    fclose(output);

/*
    printf("num:%0x, bom: %x, num_conv: %0x\n",
            0x01020304, 0xfeff, bom_to_machine_endianness(0xfeff,0x01020304));

    printf("num:%0x, bom: %x, num_conv: %0x\n",
            0x01020304, 0xfffe0000, bom_to_machine_endianness(0xfffe0000,0x01020304));

    printf("num:%0x, bom: %x, num_conv: %0x\n",
            0x04030201, 0xfeff, bom_to_machine_endianness(0xfeff,0x04030201));

    printf("num:%0x, bom: %x, num_conv: %0x\n",
            0x04030201, 0xfffe0000, bom_to_machine_endianness(0xfffe0000,0x04030201));
*/
    return 0;
}
