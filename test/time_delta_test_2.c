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
    bool res = ((diff <= time) && (time <= now)) || ((now >> 1)+1 <= diff);
    if(res) {
        printf("%lu >= %lu\n", now, time);
        return true;
    }

    printf("%lu < %lu\n", now, time);
    return false;
}