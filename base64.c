#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#define PRINT_TAG   "base64"

#ifdef DEBUG
#define dprintf(fmt, args...)   printf("[DBG]"PRINT_TAG": "fmt, ##args)
#else
#define dprintf(fmt, args...)
#endif

#define eprintf(fmt, args...)   printf("[ERR]"PRINT_TAG": "fmt, ##args)

const unsigned char * mapptr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int demap[128];

static init_demap(void)
{
    int i;
    for(i = 0; i < 64; i++) {
        demap[mapptr[i]] = i;
        //dprintf("demap[%d] = %d\n", mapptr[i], i);
    }
}

/**
 * if return is not NULL, malloc memory must be freed
 */
unsigned char * base64_encode(const unsigned char * in_code, long len)
{
    unsigned int current;
    int i;
    unsigned char *buffer;
    int buffer_size;
    int buffer_pos;

    if(len <= 0)
        return NULL;
    buffer_size = len / 3 * 4;
    if(len % 3 > 0)
        buffer_size += 4;
    buffer_size++;
    dprintf("buffer_size = %d\n", buffer_size);

    buffer = malloc(buffer_size);
    if(!buffer) {
        eprintf("encode: malloc buffer failure\n");
        return NULL;
    }

    buffer_pos = 0;
    for(i = 0; i < len - 2; i += 3) {
        current = (in_code[i] << 16) | (in_code[i+1] << 8) | (in_code[i+2] << 0);
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 18)) >> 18];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 12)) >> 12];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 6)) >> 6];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 0)) >> 0];
    }
    switch(len - i) {
        case 2:
            current = (in_code[i] << 16) | (in_code[i+1] << 8);
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 18)) >> 18];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 12)) >> 12];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 6)) >> 6];
            buffer[buffer_pos++] = '=';
            break;
        case 1:
            current = in_code[i] << 8;
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 10)) >> 10];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 4)) >> 4];
            buffer[buffer_pos++] = '=';
            buffer[buffer_pos++] = '=';
            break;
    }
    buffer[buffer_pos] = 0;

    return buffer;
}

unsigned char * base64_decode(unsigned char * ciphertext)
{
    long len;
    char *buffer;
    long buffer_size;
    long buffer_pos;
    int i;
    unsigned int current;

    if(!ciphertext)
        return NULL;
    len = strlen(ciphertext);
    if(len % 4) {
        eprintf("decode, format error!\n");
        return NULL;
    }

    buffer_size = len / 4 * 3;
    dprintf("buffer_size = %ld\n", buffer_size);

    buffer = malloc(buffer_size);
    if(!buffer) {
        eprintf("malloc failure\n");
        return NULL;
    }

    init_demap();
    buffer_pos = 0;
    for(i = 0; i < len - 3; i += 4) {
        current = (demap[ciphertext[i]] << 18) | (demap[ciphertext[i+1]] << 12) | (demap[ciphertext[i+2]] << 6) | (demap[ciphertext[i+3]]);
        dprintf("current = %x\n", current);
        buffer[buffer_pos++] = (current & (0xff << 16)) >> 16;
        buffer[buffer_pos++] = (current & (0xff << 8)) >> 8;
        buffer[buffer_pos++] = (current & (0xff << 0)) >> 0;
    }
    if(ciphertext[i + 2] == '=' && ciphertext[i + 3] == '=') {
        current = (demap[ciphertext[i]] << 10) | (demap[ciphertext[i+1]] << 4);
        dprintf("current = %x\n", current);
        buffer[buffer_pos++] = (current & (0xff << 8)) >> 8;
    } else if(ciphertext[i + 3] == '=') {
        current = (demap[ciphertext[i]] << 18) | (demap[ciphertext[i+1]] << 12) | (demap[ciphertext[i+2]] << 6);
        dprintf("current = %x\n", current);
        buffer[buffer_pos++] = (current & (0xff << 16)) >> 16;
        buffer[buffer_pos++] = (current & (0xff << 8)) >> 8;
    }
    buffer[buffer_pos] = 0;

    return buffer;
}


