#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

bool isDue_u8(uint8_t now, uint8_t time);
bool isDue_u32(uint32_t now, uint32_t time);
bool isDue_ulong(unsigned long now, unsigned long time);

int main(void) {
    unsigned long now, time;
    unsigned int mode;
    while(true) {
        scanf("%u %lu %lu", &mode, &now, &time);
        if(mode == 8) isDue_u8(now, time);
        else if(mode == 32) isDue_u32(now, time);
        else if(mode == 64) isDue_ulong(now, time);
    }
}

bool isDue_u8(uint8_t now, uint8_t time) {
    uint8_t diff = now - time;
    uint8_t mostSigBit = ((uint8_t)(-1) ^ ((uint8_t)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond5 = (now == time);
    bool cond6 = false; // (((now & mostSigBit) == mostSigBit) && (time & mostSigBit) != mostSigBit) && ((now & ~mostSigBit) - time <= diff);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5 || cond6;
    
    if(res) {
        printf("%u >= %u\n", now, time);
        return true;
    }

    printf("%u < %u\n", now, time);
    return false;
}

bool isDue_u32(uint32_t now, uint32_t time) {
    uint32_t diff = now - time;
    uint32_t mostSigBit = ((uint32_t)(-1) ^ ((uint32_t)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = ((time - (now & ~mostSigBit) < mostSigBit) && (now & mostSigBit));
    bool cond5 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond6 = (now == time);
    bool cond7 = (((now & mostSigBit) == mostSigBit) && (time & mostSigBit) != mostSigBit) && ((now & ~mostSigBit) - time <= diff);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5 || cond6 || cond7;
    
    if(res) {
        printf("%u >= %u\n", now, time);
        return true;
    }

    printf("%u < %u\n", now, time);
    return false;
}

bool isDue_ulong(unsigned long now, unsigned long time) {
    unsigned long diff = now - time;
    unsigned long mostSigBit = ((unsigned long)(-1) ^ ((unsigned long)(-1) >> 1));
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = ((time - (now & ~mostSigBit) < mostSigBit) && (now & mostSigBit));
    bool cond5 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond6 = (now == time);
    bool cond7 = (((now & mostSigBit) == mostSigBit) && (time & mostSigBit) != mostSigBit) && ((now & ~mostSigBit) - time <= diff);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5 || cond6 || cond7;
    
    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}