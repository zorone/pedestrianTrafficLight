#include <cstdint>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<limits.h>

void uint8Test();
void ulongTest();

int main(void) {
    uint8Test();
    getchar();
    ulongTest();
}

void uint8Test() {
    int count[4] = {0, 0, 0, 0};
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++) {
            uint8_t m = i, n = j;
            uint8_t k = m-n;
            uint8_t mostSigBit = ((uint8_t)(-1) & ((uint8_t)(-1) >> 1));
            int16_t k2 = m-n;
            uint8_t tcond = ((mostSigBit^(~k))+mostSigBit+2);
            bool cond1 = (k == (uint8_t)(-1));
            bool cond2 = (k < tcond);
            bool finalCond = cond1 || cond2;

            bool targetCond1 = (k2 >= 0);
            bool targetCond2 = (k >= 0);
            bool targetCond3 = (abs(k2) <= ((uint8_t)(-1)/2)+1);
            
            if(targetCond1) {
                if(targetCond2 == finalCond) {
                    printf("STRICTLY PASS: Case diff Positive: %3u - %3u: %3u: %3u: %c: %c\n", m, n, k, k,
                            targetCond2 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[0]++;
                    continue;
                }
                else {
                    printf("STRICTLY FAIL: Case diff Positive: %3u - %3u: %3u: %3u: %c: %c\n", m, n, k, k,
                            targetCond2 ? 'T': 'F', finalCond ? 'T': 'F'););
                    count[1]++;
                    continue;
                }
            }
            else if(targetCond3) {
                
            }

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

void ulongTest() {
    int count[4] = {0, 0, 0, 0};
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++) {
            unsigned long m = i, n = j;
            unsigned long k = m-n;
            unsigned long mostSigBit = (ULONG_MAX & (ULONG_MAX >> 1));
            long long k2 = m-n;
            unsigned long tcond = ((mostSigBit^(~k))+mostSigBit+2);
            bool cond1 = (k == ULONG_MAX);
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