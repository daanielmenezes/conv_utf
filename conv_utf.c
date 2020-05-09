#include <stdio.h>
//#include "conv_utf.h"

const int BOM = 0xfeff;
int utf8_32(FILE *arq_entrada, FILE *arq_saida) {
    int nBytes, c;
    unsigned char p; 

    fwrite(&BOM, 4, 1, arq_saida);
    while( fread(&p, 1, 1, arq_entrada ) ) {
        if (!(p & 0x80)) {
            c = p;
        } else{
            for (nBytes=2; p & ( 0x1<<(7-nBytes) ); nBytes++)
                ;
            c = (int) ( p & ( 0xffu >> (nBytes+1) ) );
            nBytes--;
            for(; nBytes; nBytes--){
                fread(&p, 1, 1, arq_entrada );
                c = ( c<<6 ) + ( p & 0x3f );
            }
        }
        fwrite(&c, 4, 1, arq_saida);
    }
    return 0;
}

int utf32_8(FILE *arq_entrada, FILE *arq_saida) {

}


int main(void) {
    FILE *input = fopen("utf8_peq.txt", "rb");
    FILE *output = fopen("8to32.txt", "wb");
    utf8_32(input, output);
    fclose(input);
    fclose(output);

    return 0;
}
