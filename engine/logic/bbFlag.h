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
    bbSuccess, //Generic pass
    bbFail, //Generic fail
    bbNone, //Looking handle not found in pool/lookup table
    bbFull, //Cannot allocate a new element in pool
    bbEmpty, //Getting an element from an empty list
    bbBreak, //Stop mapping the function to the list
    bbRepeat, //Map the function again to the same element of the list
    bbContinue, //Map the function to the next element of the list
    bbDelete, //delete the current entry when mapping a function to a list
    bbHead, //First element of the list
    bbTail, //last element of the list
    bbHandleError_System, //Handle belongs to wrong system
    bbHandleError_Index,  //For when you compare two handles and their indices mismatch
    bbHandleError_Generation, //The handle is stale?
    bbHandleError_Stale, //The handle is stale?
    bbHandleError_NULL, //Using a NULL handle

} bbFlag;

#endif // BB_FLAG_H

