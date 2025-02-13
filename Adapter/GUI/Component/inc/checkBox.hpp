#include "widget.hpp"


class CheckBox : public Widget<CheckBox> {
public:
   inline CheckBox& init(Obj_t parent = parent_)
   {
       create_obj(&lv_checkbox_class, parent);
       return *this;
   }

   inline CheckBox& text(const char *text)
   {
       lv_checkbox_set_text(obj_, text);
       return *this;
   }
};