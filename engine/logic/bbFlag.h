/// Most functions return a flag to indicate success/failure etc

#ifndef BB_FLAG_H
#define BB_FLAG_H

//some functions return positive values for indices and negative values for flags
//some struct elements take positive values for indices and negative values for flags
#define flag_Success     0
#define flag_None        (-1)

// the vast majority of functions return bbFlag, and other values are
// returned by reference.
typedef enum {
    bbSuccess,
    bbFail,
    bbNone,
    bbFull,
    bbEmpty,
    bbBreak,
    bbRepeat,
    bbContinue,
    bbDelete,
    bbHead,
    bbTail,
} bbFlag;

#endif // BB_FLAG_H

