#include <stdio.h>
#include "conv_utf.h"

int toLittleEndian32(int input) {
    /* retorna a versão little endian de input */
    short num = 0xff00; 
    unsigned char *temp = (unsigned char *)&num;
    if (*temp == 0xff) {
        /* maquina é big endian */
        input = ( (input & 0xff000000u) >> 3*8 ) |
                ( (input & 0x000000ff)  << 3*8 ) | 
                ( (input & 0x00ff0000)  >> 8   ) |
                ( (input & 0x0000ff00)  << 8   )
        ;
    }
    return input;
}

int utf8_32(FILE *arq_entrada, FILE *arq_saida) {
    int i, utfchar, p;
    const int BOM = toLittleEndian32(0xfeff);

    fwrite(&BOM, 4, 1, arq_saida);
    while( (p = fgetc(arq_entrada)) != EOF ) {
        if (!(p & 0x80)) {
            utfchar = p;
        } else{
            for (i=2; p & ( 0x1<<(7-i) ); i++)
                ;
            utfchar = (int) ( p & ( 0xffu >> (i+1) ) );
            i--;
            for(; i; i--){
                p = fgetc(arq_entrada); if (p == EOF) break;
                utfchar = ( utfchar<<6 ) + ( p & 0x3f );
            }
        }
        utfchar = toLittleEndian32(utfchar);
        if (!fwrite(&utfchar, 4, 1, arq_saida)){
            fprintf(stderr, "erro de gravação\n");
            return -1;
        }
    }
    if (ferror(arq_entrada)) {
        fprintf(stderr, "erro de leitura\n");
        return -1;
    }
    return 0;
}

int utf32_8(FILE *arq_entrada, FILE *arq_saida) {

}
