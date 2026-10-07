#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>

int main(void) {
    int count[4] = {0, 0, 0, 0};
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++) {
            unsigned long m = i, n = j;
            unsigned long k = m-n;
            unsigned long mostSigBit = (unsigned long)(-1) & ((unsigned long)-1 >> 1); 
            long long k2 = m-n;
            unsigned long tcond = ((mostSigBit^(~k))+mostSigBit+1);
            bool cond1 = (k == 255);
            bool cond2 = (k < tcond);
            
            if(cond1) {
                printf("%3u - %3u = %4u: %3u - %3u == 1: STRICTLY PASS\n", m, n, k, n, m);
                count[0]++;
                continue;
            }
            if(k2>=0) {
                if(cond2) {
                    printf("%3u - %3u = %4u: %3u < %4u: STRICTLY PASS\n", m, n, k, k, tcond);
                    count[0]++;
                }
                else {
                    printf("%3u - %3u = %4u: %3u < %4u: STRICTLY FAIL\n", m, n, k, k, tcond);
                    count[1]++;
                }
            }
            else if (abs(i-j) >= 128) {
                if(cond2) {
                    printf("%3u - %3u = %4u: %3u < %4u: LOOSENLY PASS\n", m, n, k, k, tcond);
                    count[2]++;
                }
                else {
                    printf("%3u - %3u = %4u: %3u < %4u: LOOSENLY FAIL (delta: %4u\n", m, n, k, k, tcond, abs(i-j)-k);
                    count[3]++;
                }
            }
            else {
                if(cond2) {
                    printf("%3u - %3u = %4u: %3u < %4u: STRICTLY PASS\n", m, n, k, k, tcond);
                    count[0]++;
                }
                else {
                    printf("%3u - %3u = %4u: %3u < %4u: STRICTLY FAIL (delta: %4u\n", m, n, k, k, tcond, abs(i-j)-k);
                    count[1]++;
                }
            }
        }
    }
    printf("STRICTLY PASS: %d\n", count[0]);
    printf("STRICTLY FAIL: %d\n", count[1]);
    printf("LOOSENLY PASS: %d\n", count[2]);
    printf("LOOSENLY FAIL: %d\n", count[3]);
}
