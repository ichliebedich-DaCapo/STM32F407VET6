#include "widget.hpp"


class CheckBox : public Widget<CheckBox> {
public:
   inline CheckBox& init(Obj_t parent = parent_)
   {
       create_obj(&lv_checkbox_class, parent);
       return *this;

   }
};