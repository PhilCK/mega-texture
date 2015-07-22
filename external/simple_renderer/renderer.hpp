#ifndef RENDERER_INCLUDED_7F2E7FF0_5B7F_47AE_8293_BCA5D46ED911
#define RENDERER_INCLUDED_7F2E7FF0_5B7F_47AE_8293_BCA5D46ED911


namespace renderer {


void                initialize();
void                de_initialize();
void                reset();
void                depth(const bool set);
void                clear(const bool color = true, const bool depth = true);
void                clear_color(const float red = 0, const float green = 0, const float blue = 0);


} // include guard


#endif // include guard