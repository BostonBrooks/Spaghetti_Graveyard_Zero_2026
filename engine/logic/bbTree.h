///bbTree defines a structure where each member has a parent and a list of daughter nodes.

#ifndef BB_TREE_H
#define BB_TREE_H


#include <stddef.h>
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbFlag.h"

typedef struct {
    bbHandle parent;
    bbListElement_Handle peers;
    bbList_Handle children;
    I32 num_children;
    bool visible;
    bool children_visible;

} bbTree_Node;


typedef struct {
    bbVPool* pool;
    size_t offset;
    bbHandle root;
} bbTree;

typedef bbFlag bbTreeFunction(bbTree* tree, void* node, void* cl);

bbFlag bbTree_new (bbTree** tree, void* pool, size_t offset);


bbFlag bbTree_init (bbTree* tree, void* pool, size_t offset);

bbFlag bbTreeNode_setEmpty(bbTree* tree, void* element);
/// add a new node as a daughter node to parent;
bbFlag bbTreeNode_setParent(bbTree* tree, void* element, void* parent);
///recursively search through nodes until myFunc(node, cl); returns f_Break
bbFlag bbTree_descendingMap(bbTree* tree,
                            void* root,
                            bbTreeFunction* my_func,
                            void* cl);
///recursively search through nodes until myFunc(node, cl); returns f_Break
bbFlag bbTree_ascendingMap(bbTree* tree,
                           void* root,
                           bbTreeFunction* my_func,
                           void* cl);

///recursively search through nodes until myFunc(node, cl); returns f_Break
bbFlag bbTree_descendingMapVisible(bbTree* tree,
                            void* root,
                            bbTreeFunction* my_func,
                            void* cl);
///recursively search through nodes until myFunc(node, cl); returns f_Break
bbFlag bbTree_ascendingMapVisible(bbTree* tree,
                            void* root,
                            bbTreeFunction* func,
                            void* cl);


#endif //BB_TREE_H