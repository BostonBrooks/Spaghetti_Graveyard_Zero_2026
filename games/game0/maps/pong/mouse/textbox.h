


bbFlag Textbox_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;
    bbMouse* mouse = Mouse;
    bbWidgets* widgets = Widgets;

    bbVPool* pool = widgets->pool;
    bbHandle handle;

    bbVPool_reverseLookup(pool, widget, &handle);
    widget->mtable.hover = true;
    home.UI.widgets.selected_textbox = widget;
    return bbSuccess;
}