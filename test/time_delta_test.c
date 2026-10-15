#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

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
            uint8_t mostSigBit = ((uint8_t)(-1) ^ ((uint8_t)(-1) >> 1));
            int16_t k2 = (int16_t)m-(int16_t)n;
            bool finalCond = ((k <= n) && (n <= m)) || (n <= (m >> 1)) || ((n > m) && (k < mostSigBit));

            bool targetCond1 = (k2 >= 0);
            bool targetCond2 = (k >= 0);
            bool targetCond3 = (abs(k2) <= ((uint8_t)(-1)/2)+1);
            
            if(targetCond1) {
                if(targetCond1 == finalCond) {
                    printf("STRICTLY PASS: Case diff Positive: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[0]++;
                    continue;
                }
                else {
                    printf("STRICTLY FAIL: Case diff Positive: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[1]++;
                    continue;
                }
            }
            else if(targetCond3) {
                if(targetCond1 == finalCond) {
                    printf("STRICTLY PASS: Case diff Negative: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[0]++;
                    continue;
                }
                else {
                    printf("STRICTLY FAIL: Case diff Negative: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[1]++;
                    continue;
                }
            }
            else {
                if(targetCond1 == finalCond) {
                    printf("LOOSENLY PASS: Case diff Negative: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[2]++;
                    continue;
                }
                else {
                    printf("LOOSENLY FAIL: Case diff Negative: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[3]++;
                    continue;
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
    long count[4] = {0, 0, 0, 0};
    unsigned long testPairMin[3] = {0,  ((ULONG_MAX/2)+1-256) , ULONG_MAX-256};
    unsigned long testPairMax[3] = {255, ((ULONG_MAX/2)+1+256), ULONG_MAX};
    for(int testPairIdx = 0; testPairIdx < 3; testPairIdx++){
        unsigned long minBoundaryI = testPairMin[testPairIdx];
        unsigned long minBoundaryJ = testPairMin[testPairIdx];
        unsigned long maxBoundary = testPairMax[testPairIdx];
        for( ; minBoundaryI <= maxBoundary; minBoundaryI++) {
            for( ; minBoundaryJ <= maxBoundary; minBoundaryJ++) {
                unsigned long m = minBoundaryI, n = minBoundaryJ;
                unsigned long k = m-n;
                unsigned long mostSigBit = (ULONG_MAX ^ (ULONG_MAX >> 1));
                long long k2 = (long long)m-(long long)n;
                bool finalCond = ((k <= n) && (n <= m)) || (n <= (m >> 1)) || ((n > m) && (k < mostSigBit));

                bool targetCond1 = (k2 >= 0);
                bool targetCond2 = (k >= 0);
                bool targetCond3 = (abs(k2) <= (ULONG_MAX/2)+1);
                
                if(targetCond1) {
                    if(targetCond1 == finalCond) {
                        printf("STRICTLY PASS: Case diff Positive: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[0]++;
                    }
                    else {
                        printf("STRICTLY FAIL: Case diff Positive: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[1]++;
                    }
                }
                else if(targetCond3) {
                    if(targetCond1 == finalCond) {
                        printf("STRICTLY PASS: Case diff Negative: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[0]++;
                    }
                    else {
                        printf("STRICTLY FAIL: Case diff Negative: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[1]++;
                    }
                }
                else {
                    if(targetCond1 == finalCond) {
                        printf("LOOSENLY PASS: Case diff Negative: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[2]++;
                    }
                    else {
                        printf("LOOSENLY FAIL: Case diff Negative: %20lu - %20lu: %20lu (%21ld): %20lu (%21ld): %c: %c\n", m, n, k, k2, k, k2,
                                targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                        count[3]++;
                    }
                }

                if(minBoundaryJ == maxBoundary) break;
            }
            if(minBoundaryI == maxBoundary) break;
        }
    }
    printf("STRICTLY PASS: %d\n", count[0]);
    printf("STRICTLY FAIL: %d\n", count[1]);
    printf("LOOSENLY PASS: %d\n", count[2]);
    printf("LOOSENLY FAIL: %d\n", count[3]);
}