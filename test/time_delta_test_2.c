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
    bool cond1 = ((diff <= time) && (time <= now));
    bool cond2 = ((time <= (now >> 1)) && ((diff & ~mostSigBit) == diff));
    bool cond3 = ((time > now) && (diff < mostSigBit));
    bool cond4 = ((time - (now & ~mostSigBit) < mostSigBit) && (now & mostSigBit));
    bool cond5 = (!(now & mostSigBit) && !(time & mostSigBit) && (time - now >= mostSigBit));
    bool cond6 = (((now & ~mostSigBit) <= diff) && (now & mostSigBit) == mostSigBit);
    bool cond7 = (((now & mostSigBit) == mostSigBit) && (time & mostSigBit) != mostSigBit) && ((now & ~mostSigBit) - time <= diff);
    bool res = cond1 || cond2 || cond3 || cond4 || cond5 || cond6 || cond7;
    
    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}