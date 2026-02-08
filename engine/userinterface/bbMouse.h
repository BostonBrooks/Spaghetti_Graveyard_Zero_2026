typedef struct bbMouse
{
    bbScreenPoints position;
    bbScreenPoints previous_position;
    bbScreenPoints drag_origin;
    unsigned int left_down : 1;
    unsigned int right_down : 1;
    unsigned int left_changed : 1;
    unsigned int right_changed : 1;
    //what if mouse button is down but nothing is selected?
    // PS we have bbVPool.null
    // and I32 bbVPool_handleIsEqual(bbVPool* pool, bbPool_Handle A, bbPool_Handle B);

    //wasOver == null indicates isover has not change
    bbHandle was_over;
    bbHandle is_over;
    bbHandle selected;
    I32 default_mouse_icon;

    //bbMouseFunctions functions;
} bbMouse;