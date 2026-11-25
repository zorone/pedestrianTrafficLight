#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

typedef union {
    uint8_t u8;
    uint32_t u32;
    unsigned long ul;
} integer;

bool isDue_u8(integer now, integer time);
bool isDue_u32(integer now, integer time);
bool isDue_ulong(integer now, integer time);

int main(void) {
    integer now, time;
    unsigned int mode;
    bool (*isDue)(integer, integer);

    scanf("%u", &mode);
    if(mode == 8) isDue = isDue_u8;
    else if(mode == 32) isDue = isDue_u32;
    else if(mode == 64) isDue = isDue_ulong;
    else {
        printf("Error, mode mismatches");
        isDue = NULL;
    }

    while(true) {
        scanf("%lu %lu", &now.ul, &time.ul);
        isDue(now, time);
    }
}

bool isDue_u8(integer nowTmp, integer timeTmp) {
    uint8_t now = nowTmp.u8;
    uint8_t time = timeTmp.u8;
    uint8_t diff = now - time;
    uint8_t mostSigBit = ((uint8_t)(-1) ^ ((uint8_t)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond5 = (now == time);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5;
    
    if(res) {
        printf("%u >= %u\n", now, time);
        return true;
    }

    printf("%u < %u\n", now, time);
    return false;
}

bool isDue_u32(integer nowTmp, integer timeTmp) {
    uint32_t now = nowTmp.u32;
    uint32_t time = timeTmp.u32;
    uint32_t diff = now - time;
    uint32_t mostSigBit = ((uint32_t)(-1) ^ ((uint32_t)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond5 = (now == time);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5;
    
    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}

bool isDue_ulong(integer nowTmp, integer timeTmp) {
    unsigned long now = nowTmp.ul;
    unsigned long time = timeTmp.ul;
    unsigned long diff = now - time;
    unsigned long mostSigBit = ((unsigned long)(-1) ^ ((unsigned long)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond5 = (now == time);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5;
    
    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}