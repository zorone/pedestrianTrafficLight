#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

bool isDue(unsigned long now, unsigned long time);

int main(void) {
    unsigned long now, time;
    while(true) {
        scanf("%lu %lu", &now, &time);

    }
}

bool isDue(unsigned long now, unsigned long time) {
    unsigned long diff = now - time;
    unsigned long mostSigBit = (ULONG_MAX & (ULONG_MAX >> 1));
    bool cond1 = (diff == ULONG_MAX);
    bool cond2 = ((mostSigBit^(~diff))+mostSigBit+2);
    bool res = cond1 || cond2;
    if(res) {
        printf("%lu >= %lu", now, time);
        return true;
    }
    
    printf("%lu < %lu", now, time);
    return false;
}