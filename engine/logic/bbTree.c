#include "engine/logic/bbTree.h"
#include "engine/logic/bbTerminal.h"


#define isEqual(A, B) bbVPool_handleIsEqual(tree->pool, A, B)
#define isNULL(A) bbVPool_handleIsEqual(tree->pool, A, tree->pool->null)


bbFlag bbTree_new (bbTree** Tree, void* pool, size_t offset){
    bbTree* tree = malloc(sizeof(bbTree));
    bbVPool* Pool = pool;
    tree->pool = Pool;
    tree->root = Pool->null;
    tree->offset = offset;
    *Tree = tree;

    return bbSuccess;
}


bbFlag bbTreeNode_setEmpty(bbTree* tree, void* element){
    bbTree_Node* node = element+tree->offset;
    node->parent = tree->pool->null;
    node->peers.prev = tree->pool->null;
    node->peers.next = tree->pool->null;
    node->children.head = tree->pool->null;
    node->children.tail = tree->pool->null;
    node->num_children = 0;
    node->visible = 1;
    node->children_visible = 1;
    return bbSuccess;
}

bbFlag bbTreeNode_setParent(bbTree* tree, void* element, void* parent){


    bbVPool* pool = tree->pool;

    bbHandle element_handle;
    bbHandle parent_handle;

    bbVPool_reverseLookup(pool, element, &element_handle);
    bbVPool_reverseLookup(pool, parent, &parent_handle);

    bbTree_Node* element_node = element + tree->offset;
    bbTree_Node* parent_node = parent + tree->offset;

    bbAssert(isNULL(element_node->peers.prev), "already in list\n");
    bbAssert(isNULL(element_node->peers.next), "already in list\n");

    bbHandle head_handle = parent_node->children.head;
    bbHandle tail_handle = parent_node->children.tail;

    if (isNULL(head_handle)){
        bbAssert(isNULL(tail_handle), "head/tail\n");
        bbAssert(parent_node->num_children == 0, "empty list but num != 0\n");
        parent_node->num_children = 1;
        parent_node->children.head = element_handle;
        parent_node->children.tail = element_handle;
        element_node->peers.prev = element_handle;
        element_node->peers.next = element_handle;
        element_node->parent = parent_handle;

        return bbSuccess;

    }

    void* head;
    bbVPool_lookup(tree->pool, &head, parent_node->children.head);
    bbTree_Node* head_node = head + tree->offset;

    void* tail;
    bbVPool_lookup(tree->pool, &tail, parent_node->children.tail);
    bbTree_Node* tail_node = tail + tree->offset;

    if(isEqual(head_handle, tail_handle)){
        bbAssert(parent_node->num_children == 1, "only one element\n");


        element_node->peers.prev = head_handle;
        element_node->peers.next = tail_handle;

        head_node->peers.next = element_handle;
        head_node->peers.prev = element_handle;
        parent_node->children.tail = element_handle;
        parent_node->num_children = 2;

        return bbSuccess;
    }

    tail_node->peers.next = element_handle;
    head_node->peers.prev = element_handle;
    element_node->peers.prev = tail_handle;
    element_node->peers.next = head_handle;
    parent_node->children.tail = element_handle;
    parent_node->num_children++;

    return bbSuccess;
}

bbFlag bbTree_descendingMap(bbTree* tree, void* root, bbTreeFunction* my_func,
                            void* cl)
{

    bbAssert(root != NULL, "null object address\n");
    bbTree_Node* root_node = root + tree->offset;
    bbFlag flag = my_func(tree, root, cl);


    if (flag == bbBreak) return bbBreak;

    if(isNULL(root_node->children.head)){
        bbAssert(isNULL(root_node->children.tail), "head/tail\n");
        return bbContinue;
    }

    bbHandle element_handle = root_node->children.head;
    void* element;
    bbTree_Node* element_node;

    while(1){

        bbVPool_lookup(tree->pool, &element, element_handle);
        element_node = element + tree->offset;
        flag = bbTree_descendingMap(tree, element, my_func, cl);
        switch(flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(element_handle, root_node->children.tail))
                return bbContinue;
            element_handle = element_node->peers.next;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
            break;
        }
    }

}

bbFlag bbTree_ascendingMap(bbTree* tree, void* root, bbTreeFunction* my_func,
                           void* cl) {

    bbAssert(root != NULL, "null object address\n");
    bbTree_Node* root_node = root + tree->offset;
    bbFlag flag;

    if(isNULL(root_node->children.head)){
        bbAssert(isNULL(root_node->children.tail), "head/tail\n");
        goto label;
    }

    bbHandle element_handle = root_node->children.tail;
    void* element;
    bbTree_Node* element_node;

    while(1){
        bbVPool_lookup(tree->pool, &element, element_handle);
        element_node = element + tree->offset;
        flag = bbTree_ascendingMap(tree, element, my_func, cl);
        switch(flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(element_handle, root_node->children.head))
                goto label;
            element_handle = element_node->peers.prev;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }
    }
    label:

        flag = my_func(tree, root, cl);
    if(flag == bbBreak) return bbBreak;

    return bbContinue;

}

bbFlag bbTree_descendingMapVisible(bbTree* tree, void* root, bbTreeFunction* my_func,
                            void* cl)
{

    bbAssert(root != NULL, "null object address\n");
    bbTree_Node* root_node = root + tree->offset;

    if(root_node->visible) {
        bbFlag flag = my_func(tree, root, cl);
        if (flag == bbBreak) return bbBreak;
    }

    if(isNULL(root_node->children.head)){
        bbAssert(isNULL(root_node->children.tail), "head/tail\n");
        return bbContinue;
    }

    bbHandle element_handle = root_node->children.head;
    void* element;
    bbTree_Node* element_node;
    bbFlag flag;

    if (root_node->children_visible == false) return bbContinue;
    while(1){

        bbVPool_lookup(tree->pool, &element, element_handle);
        element_node = element + tree->offset;
        flag = bbTree_descendingMapVisible(tree, element, my_func, cl);
        switch(flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(element_handle, root_node->children.tail))
                return bbContinue;
            element_handle = element_node->peers.next;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
            break;
        }
    }

}

bbFlag bbTree_ascendingMapVisible(bbTree* tree, void* root, bbTreeFunction* my_func,
                           void* cl) {

    bbAssert(root != NULL, "null object address\n");
    bbTree_Node* root_node = root + tree->offset;
    bbFlag flag;

    if(isNULL(root_node->children.head)){
        bbAssert(isNULL(root_node->children.tail), "head/tail\n");
        goto label;
    }

    bbHandle element_handle = root_node->children.tail;
    void* element;
    bbTree_Node* element_node;

    if(root_node->children_visible == false) goto label;
    while(1){
        bbVPool_lookup(tree->pool, &element, element_handle);
        element_node = element + tree->offset;
        flag = bbTree_ascendingMapVisible(tree, element, my_func, cl);
        switch(flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(element_handle, root_node->children.head))
                goto label;
            element_handle = element_node->peers.prev;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }
    }
    label:

    if(root_node->visible) {
        flag = my_func(tree, root, cl);
        if (flag == bbBreak) return bbBreak;
    }

    return bbContinue;

}