#ifndef LIST_BOX_H
#define LIST_BOX_H

#include "containers/box_container.h"

namespace GUI {
	
class List;
class HScrollBar;
	
class ListBox : HBoxContainer {

private:
	
	List* list;
	HScrollBar* scroll_bar;

public:

	List* get_list();

	ListBox();
	~ListBox();
};
	
};

#endif
