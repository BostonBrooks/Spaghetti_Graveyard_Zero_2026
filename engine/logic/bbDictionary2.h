#ifndef BB_DICTIONARY2_H
#define BB_DICTIONARY2_H


#include "bbFlag.h"
#include "bbHandle.h"
#include "engine/data/bbConstants.h"

typedef struct {
    char key[KEY_LENGTH];
    //hash should always be non-zero
    U32 hash;
    bbHandle value;
} bbDictionary2_entry;

typedef struct {
    I32 size;
    bbDictionary2_entry *entries;
} bbDictionary2_bin;


typedef struct {
    I32 num_bins;
    I32 min_bin_size;
    bbDictionary2_bin bins[];
} bbDictionary2;


/// create a new dictionary
bbFlag bbDictionary2_new(bbDictionary2** dict, I32 num_bins, I32 min_bin_size);

/// delete an existing dictionary
bbFlag bbDictionary2_delete(bbDictionary2* dict);

///Add a key, value pair to the dictionary
///if the key exists replace its value, return a handle if asked, return bbNone
///return bbSuccess
bbFlag bbDictionary2_add(bbDictionary2* dict, char* key, bbHandle value, bbHandle* entry_handle);

/// remove a key from dictionary
bbFlag bbDictionary2_removeKey(bbDictionary2* dict, char* key);

/// remove a key from dictionary
bbFlag bbDictionary2_removeHandle(bbDictionary2* dict, bbHandle entry_handle);

/// get value from key
bbFlag bbDictionary2_lookup(bbDictionary2* dict, char* key, bbHandle* value, bbHandle* entry_handle);


#endif //BB_DICTIONARY2_H