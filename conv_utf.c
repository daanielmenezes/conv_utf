#include <stdio.h>
#include "conv_utf.h"

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2

int inverte_bytes_int(int a) {
    return ( (a & 0xff000000u) >> 3*8 ) |
           ( (a & 0x000000ff)  << 3*8 ) | 
           ( (a & 0x00ff0000)  >> 8   ) |
           ( (a & 0x0000ff00)  << 8   )
       ;
}

int machine_endianness(void) {
    short num = 0xff00; 
    unsigned char *temp = (unsigned char *)&num;
    return (*temp == 0xff) ? BIG_ENDIAN : LITTLE_ENDIAN;
}

int itole(int input) {
    /* retorna a versão little endian de input */
    if ( machine_endianness() == BIG_ENDIAN ) {
        input = inverte_bytes_int(input);
    }
    return input;
}

int bom_to_machine_endianness(int bom, int num) {
    /* transforma num de endianness de BOM para
     * o mesmo endianness da maquina */
    int BOM_ENDIANNESS = (bom == itole(0xfeff)) ? LITTLE_ENDIAN : BIG_ENDIAN;
    if ( BOM_ENDIANNESS != machine_endianness() )
        num = inverte_bytes_int(num); 
    return num;
}

int calcula_char_bytes_32to8(int utf32char) {
    if ( utf32char <= 0x7f )
        return 1;
    else if ( utf32char <= 0x7ff )
        return 2;
    else if ( utf32char <= 0xffff )
        return 3;
    else
        return 4;
}

int escreve(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    int qtdEscrito;
    if (stream == NULL || fwrite(ptr, size, nmemb, stream) < nmemb) {
        fprintf(stderr, "erro de gravação\n");
        return -1;
    }
    return 0;
 }

int utf8_32(FILE *arq_entrada, FILE *arq_saida) {
    int i, utfchar, p;
    const int BOM = itole(0xfeff);

    if (arq_entrada == NULL) {
        fprintf(stderr, "erro de leitura\n");
        return -1;
    }

    if ( escreve(&BOM, 4, 1, arq_saida) ) return -1;
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
        utfchar = itole(utfchar);
        if ( escreve(&utfchar, 4, 1, arq_saida) )
            return -1;
    }
    if (ferror(arq_entrada)) {
        fprintf(stderr, "erro de leitura\n");
        return -1;
    }
    return 0;
}

int utf32_8(FILE *arq_entrada, FILE *arq_saida) {
    unsigned char utf8byte;
    unsigned int utf32char, BOM;
    int i;
    if ( arq_entrada == NULL || fread(&BOM, 4, 1, arq_entrada) != 1 ) {
        fprintf(stderr, "erro de leitura\n");
        return -1;
    }
    while ( fread(&utf32char, 4, 1, arq_entrada) == 1 ) {
        utf32char = bom_to_machine_endianness(BOM, utf32char);
        i = calcula_char_bytes_32to8(utf32char);
        if (i == 1){
            utf8byte = (unsigned char)( utf32char%256 );
            if ( escreve(&utf8byte, 1, 1, arq_saida) ) return -1;
        }
        else {
            i--;
            utf8byte = ((char)0x80 >> i) | (char)(utf32char>>(6*i));
            if( escreve(&utf8byte, 1, 1, arq_saida) ) return -1;
            while(i--){
                utf8byte = (0x80) | ( 0x3f & (utf32char>>6*i) );
                if( escreve(&utf8byte, 1, 1, arq_saida) ) return -1;
            }
        }
    }
    if (ferror(arq_entrada)) {
        fprintf(stderr, "erro de leitura\n");
        return -1;
    }
    return 0;
}
