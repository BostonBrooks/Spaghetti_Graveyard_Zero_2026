//mouse hovers over widget, uses pointer defined by widget
bbFlag IsOver_Hover(bbMouse* mouse, bbWidgets* widgets, bbWidget* widget)
{

    bbScreenPointsRect rect = widget->rect;
    bbScreenPoints point = mouse->position;

    if (bbScreenPoints_inScreenPointsRect(point, rect)){
        bbVPool* pool = widgets->pool;
        bbHandle handle;
        bbVPool_reverseLookup(pool,widget,&handle);

        if (!bbVPool_handleIsEqual(pool,handle,mouse->is_over))
        {
            mouse->was_over = mouse->is_over;
            mouse->is_over = handle;
        }
        return bbBreak;
    }
    return bbContinue;
}
