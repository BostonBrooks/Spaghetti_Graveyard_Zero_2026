/// Prime numbers are good to use as the number of bins in a dictionary

#ifndef BB_PRIME_H
#define BB_PRIME_H

#include "engine/logic/bbIntTypes.h"

static I32 isPrime(I32 n){
    for (I32 i = 2; i*i <= n; i++){
        if (n % i == 0) return 0;
    }
    return 1;
}

static I32 nextPrime(I32 n){

    I32 m = n;
    while (!isPrime(m)) m++;

    return m;
}

#endif //BB_PRIME_H