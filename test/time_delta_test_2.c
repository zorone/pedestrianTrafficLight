#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

bool isDue(unsigned long now, unsigned long time);

int main(void) {
    unsigned long now, time;
    while(true) {
        scanf("%lu %lu", &now, &time);
        isDue(now, time);
    }
}

bool isDue(unsigned long now, unsigned long time) {
    unsigned long diff = now - time;
    unsigned long mostSigBit = ((unsigned long)(-1) ^ ((unsigned long)(-1) >> 1));
    bool res = ((diff <= time) && (time <= now)) || (time <= (now >> 1)) && ((diff & ~mostSigBit) == diff)
               || ((time > now) && (diff <= mostSigBit))
               || ((time - (now & ~mostSigBit) < mostSigBit) && (now & mostSigBit))
               || (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit))
               || (((now & ~mostSigBit) <= k) && ((now & ~mostSigBit) == mostSigBit))
               || (((now & ~mostSigBit) <= m) && (now & ~mostSigBit) == mostSigBit);

    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}