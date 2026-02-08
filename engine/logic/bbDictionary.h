///bbDictionary stores a number of key-value pairs for fast lookup.

#ifndef BB_DICTIONARY_H
#define BB_DICTIONARY_H

#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/data/bbConstants.h"





typedef struct
{
    I32 head;
    I32 tail;
} bbDictionary_bin;

typedef struct
{
    I32 self;
    I32 prev;
    I32 next;
    bool in_use;

    char key[KEY_LENGTH];
    bbHandle value;

} bbDictionary_entry;

typedef struct
{
    I32 num_bins;
    bbDictionary_entry* pool[100];
    bbDictionary_bin available;
    bbDictionary_bin bins[];
} bbDictionary;

/// create a new dictionary
bbFlag bbDictionary_new(bbDictionary** dict, I32 num_bins);
/// delete an existing dictionary
bbFlag bbDictionary_delete(bbDictionary* dict);
/// add key/value pair to dictionary and overwrite if duplicate
bbFlag bbDictionary_add(bbDictionary* dict, char* key, bbHandle value);
/// remove a key from dictionary
bbFlag bbDictionary_remove(bbDictionary* dict, char* key);
/// get value from key
bbFlag bbDictionary_lookup(bbDictionary* dict, char* key, bbHandle* value);

/// remove all key/value pairs in dictionary
bbFlag bbDictionary_clear(bbDictionary* dict);
/// print all data in dictionary
bbFlag bbDictionary_print(bbDictionary* dict);

#endif //BB_DICTIONARY_H