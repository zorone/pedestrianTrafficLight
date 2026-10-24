#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
    bool res = ((diff <= time) && (time <= now)) || (time <= (now >> 1)) || ((time > now) && (diff <= mostSigBit));

    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}