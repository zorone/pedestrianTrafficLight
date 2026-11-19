#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

// Source - https://stackoverflow.com/a/3208376
// Posted by William Whyte, modified by community. See post 'Timeline' for change history
// Retrieved 2026-11-19, License - CC BY-SA 4.0

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

typedef unsigned _BitInt(128) uint128_t;

void uint8Test();
void ulongTest();

const char* truthinessReport[4] = {
    " True Positive", " True Negative", "False Positive", "False Negative"
};

int main(void) {
    uint8Test();
    getchar();
    ulongTest();
}

void uint8Test() {
    int count[4] = {0, 0, 0, 0};
    int truthinessReportIdx = -1;
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++) {
            uint8_t m = i, n = j;
            uint8_t k = m-n;
            uint8_t mostSigBit = ((uint8_t)(-1) ^ ((uint8_t)(-1) >> 1));
            int16_t k2 = (int16_t)m-(int16_t)n;
            bool finalCond = ((k <= n) && (n <= m)) || ((n <= (m >> 1)) && ((k & ~mostSigBit) == k)) || ((n > m) && (k < mostSigBit)) || ((n - (m & ~mostSigBit) < mostSigBit) && (m & mostSigBit)) || (!(m & mostSigBit) && !(n & mostSigBit) && ((n-m >= mostSigBit) || (n-m < mostSigBit)));

            bool targetCond1 = (k2 >= 0);
            bool targetCond2 = (k >= 0);
            bool targetCond3 = (abs(k2) <= ((uint8_t)(-1)/2)+1);
            
            if(targetCond1) {
                if(targetCond1 == finalCond) {
                    if(finalCond) truthinessReportIdx = 0;
                    else truthinessReportIdx = 1;
                    printf("STRICTLY PASS: Case diff Positive: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[0]++;
                    continue;
                }
                else {
                    if(finalCond) truthinessReportIdx = 2;
                    else truthinessReportIdx = 3;
                    printf("STRICTLY FAIL: Case diff Positive: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    printf("   now: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m));
                    printf("\n  time: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(n));
                    printf("\n  diff: 0b" BYTE_TO_BINARY_PATTERN "\n\n", BYTE_TO_BINARY(k));
                    count[1]++;
                    continue;
                }
            }
            else if(targetCond3) {
                if(targetCond1 == finalCond) {
                    if(finalCond) truthinessReportIdx = 0;
                    else truthinessReportIdx = 1;
                    printf("STRICTLY PASS: Case diff Negative: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    count[0]++;
                    continue;
                }
                else {
                    if(finalCond) truthinessReportIdx = 2;
                    else truthinessReportIdx = 3;
                    printf("STRICTLY FAIL: Case diff Negative: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    printf("   now: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m));
                    printf("\n  time: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(n));
                    printf("\n  diff: 0b" BYTE_TO_BINARY_PATTERN "\n\n", BYTE_TO_BINARY(k));
                    count[1]++;
                    continue;
                }
            }
            else {
                if(targetCond1 == finalCond) {
                    if(finalCond) truthinessReportIdx = 0;
                    else truthinessReportIdx = 1;
                    printf("LOOSENLY PASS: Case diff Negative: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    printf("   now: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m));
                    printf("\n  time: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(n));
                    printf("\n  diff: 0b" BYTE_TO_BINARY_PATTERN "\n\n", BYTE_TO_BINARY(k));
                    count[2]++;
                    continue;
                }
                else {
                    if(finalCond) truthinessReportIdx = 2;
                    else truthinessReportIdx = 3;
                    printf("LOOSENLY FAIL: Case diff Negative: %s: %3u - %3u: %3u (%4d): %3u (%4d): %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k2, k, k2,
                            targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                    printf("   now: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m));
                    printf("\n  time: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(n));
                    printf("\n  diff: 0b" BYTE_TO_BINARY_PATTERN "\n\n", BYTE_TO_BINARY(k));
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
    unsigned long testPair[3][2] = {{0, 255}, {((ULONG_MAX/2)+1-256), ((ULONG_MAX/2)+1+256)}, {ULONG_MAX-256, ULONG_MAX}};
    int truthinessReportIdx = -1;

    for(unsigned long testPairFirstIdx = 0; testPairFirstIdx < 3; ++testPairFirstIdx) {
        for(unsigned long testPairLastIdx = 0; testPairLastIdx < 3; ++testPairLastIdx) {
            for(unsigned long m = testPair[testPairFirstIdx][0]; m <= testPair[testPairFirstIdx][1]; m++) {
                for(unsigned long n = testPair[testPairLastIdx][0]; n <= testPair[testPairLastIdx][1]; n++) {
                    unsigned long k = m-n;
                    unsigned long mostSigBit = (ULONG_MAX ^ (ULONG_MAX >> 1));
                    uint128_t k2 = (uint128_t)m-(uint128_t)n;
                    bool finalCond = ((k <= n) && (n <= m)) || ((n <= (m >> 1)) && ((k & ~mostSigBit) == k)) || ((n > m) && (k < mostSigBit)) || ((n - (m & ~mostSigBit) < mostSigBit) && (m & mostSigBit)) || (!(m & mostSigBit) && !(n & mostSigBit) && (n-m >= mostSigBit));

                    bool targetCond1 = (k2 >= 0);
                    bool targetCond2 = (k >= 0);
                    bool targetCond3 = (llabs(k2) <= (ULONG_MAX/2)+1);
                    
                    if(targetCond1) {
                        if(finalCond) truthinessReportIdx = 0;
                        else truthinessReportIdx = 1;
                        if(targetCond1 == finalCond) {
                            printf("STRICTLY PASS: Case diff Positive: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            count[0]++;
                        }
                        else {
                            if(finalCond) truthinessReportIdx = 2;
                            else truthinessReportIdx = 3;
                            printf("STRICTLY FAIL: Case diff Positive: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            printf("   now: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(m>>56), BYTE_TO_BINARY(m>>48), BYTE_TO_BINARY(m>>40), BYTE_TO_BINARY(m>>32),
                                BYTE_TO_BINARY(m>>24), BYTE_TO_BINARY(m>>16), BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m)
                            );
                            printf("\n  time: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(n>>56), BYTE_TO_BINARY(n>>48), BYTE_TO_BINARY(n>>40), BYTE_TO_BINARY(n>>32),
                                BYTE_TO_BINARY(n>>24), BYTE_TO_BINARY(n>>16), BYTE_TO_BINARY(n>>8), BYTE_TO_BINARY(n)
                            );
                            printf("\n  diff: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n\n",
                                BYTE_TO_BINARY(k>>56), BYTE_TO_BINARY(k>>48), BYTE_TO_BINARY(k>>40), BYTE_TO_BINARY(k>>32),
                                BYTE_TO_BINARY(k>>24), BYTE_TO_BINARY(k>>16), BYTE_TO_BINARY(k>>8), BYTE_TO_BINARY(k)
                            );
                            count[1]++;
                        }
                    }
                    else if(targetCond3) {
                        if(targetCond1 == finalCond) {
                            if(finalCond) truthinessReportIdx = 0;
                            else truthinessReportIdx = 1;
                            printf("STRICTLY PASS: Case diff Negative: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            count[0]++;
                        }
                        else {
                            if(finalCond) truthinessReportIdx = 2;
                            else truthinessReportIdx = 3;
                            printf("STRICTLY FAIL: Case diff Negative: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            printf("   now: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(m>>56), BYTE_TO_BINARY(m>>48), BYTE_TO_BINARY(m>>40), BYTE_TO_BINARY(m>>32),
                                BYTE_TO_BINARY(m>>24), BYTE_TO_BINARY(m>>16), BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m)
                            );
                            printf("\n  time: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(n>>56), BYTE_TO_BINARY(n>>48), BYTE_TO_BINARY(n>>40), BYTE_TO_BINARY(n>>32),
                                BYTE_TO_BINARY(n>>24), BYTE_TO_BINARY(n>>16), BYTE_TO_BINARY(n>>8), BYTE_TO_BINARY(n)
                            );
                            printf("\n  diff: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n\n",
                                BYTE_TO_BINARY(k>>56), BYTE_TO_BINARY(k>>48), BYTE_TO_BINARY(k>>40), BYTE_TO_BINARY(k>>32),
                                BYTE_TO_BINARY(k>>24), BYTE_TO_BINARY(k>>16), BYTE_TO_BINARY(k>>8), BYTE_TO_BINARY(k)
                            );
                            count[1]++;
                        }
                    }
                    else {
                        if(targetCond1 == finalCond) {
                            if(finalCond) truthinessReportIdx = 0;
                            else truthinessReportIdx = 1;
                            printf("LOOSENLY PASS: Case diff Negative: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            printf("   now: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(m>>56), BYTE_TO_BINARY(m>>48), BYTE_TO_BINARY(m>>40), BYTE_TO_BINARY(m>>32),
                                BYTE_TO_BINARY(m>>24), BYTE_TO_BINARY(m>>16), BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m)
                            );
                            printf("\n  time: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(n>>56), BYTE_TO_BINARY(n>>48), BYTE_TO_BINARY(n>>40), BYTE_TO_BINARY(n>>32),
                                BYTE_TO_BINARY(n>>24), BYTE_TO_BINARY(n>>16), BYTE_TO_BINARY(n>>8), BYTE_TO_BINARY(n)
                            );truthinessReport[truthinessReportIdx], 
                            printf("\n  diff: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n\n",
                                BYTE_TO_BINARY(k>>56), BYTE_TO_BINARY(k>>48), BYTE_TO_BINARY(k>>40), BYTE_TO_BINARY(k>>32),
                                BYTE_TO_BINARY(k>>24), BYTE_TO_BINARY(k>>16), BYTE_TO_BINARY(k>>8), BYTE_TO_BINARY(k)
                            );
                            count[2]++;
                        }
                        else {
                            if(finalCond) truthinessReportIdx = 2;
                            else truthinessReportIdx = 3;
                            printf("LOOSENLY FAIL: Case diff Negative: %s: %20lu - %20lu: %20lu: %20lu: %c: %c\n", truthinessReport[truthinessReportIdx], m, n, k, k,
                                    targetCond1 ? 'T': 'F', finalCond ? 'T': 'F');
                            printf("   now: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(m>>56), BYTE_TO_BINARY(m>>48), BYTE_TO_BINARY(m>>40), BYTE_TO_BINARY(m>>32),
                                BYTE_TO_BINARY(m>>24), BYTE_TO_BINARY(m>>16), BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m)
                            );
                            printf("\n  time: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
                                BYTE_TO_BINARY(n>>56), BYTE_TO_BINARY(n>>48), BYTE_TO_BINARY(n>>40), BYTE_TO_BINARY(n>>32),
                                BYTE_TO_BINARY(n>>24), BYTE_TO_BINARY(n>>16), BYTE_TO_BINARY(n>>8), BYTE_TO_BINARY(n)
                            );
                            printf("\n  diff: 0b" 
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n\n",
                                BYTE_TO_BINARY(k>>56), BYTE_TO_BINARY(k>>48), BYTE_TO_BINARY(k>>40), BYTE_TO_BINARY(k>>32),
                                BYTE_TO_BINARY(k>>24), BYTE_TO_BINARY(k>>16), BYTE_TO_BINARY(k>>8), BYTE_TO_BINARY(k)
                            );
                            count[3]++;
                        }
                    }
                    if(n == ULONG_MAX) break;
                }
                if(m == ULONG_MAX) break;
            }
        }
    }
    printf("STRICTLY PASS: %ld\n", count[0]);
    printf("STRICTLY FAIL: %ld\n", count[1]);
    printf("LOOSENLY PASS: %ld\n", count[2]);
    printf("LOOSENLY FAIL: %ld\n", count[3]);
}