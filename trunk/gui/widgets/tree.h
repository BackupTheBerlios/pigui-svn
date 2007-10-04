//
// C++ Interface: tree
//
// Description: 
//
//
// Author: red,,, <red@hororo>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUITREE_H
#define GUITREE_H


#include "base/widget.h"


		
namespace GUI {

/**
	@author red,,, <red@hororo>
*/
class Tree;

class TreeItem {
friend class Tree;


			
	BitmapID bitmap;
	String text;
	bool checked; // in case it's a checkable tree
	
	bool collapsed; // wont show childs

	Signal<> changed_signal;
	Signal< Method1<TreeItem*> > internal_selected_signal;
	Signal<Method1<TreeItem*> > internal_deselected_signal;

	TreeItem *parent; // parent item
	TreeItem *next; // next in list
	TreeItem *childs; //child items
	Tree *tree; //tree (for reference)

	bool selected;
	
	bool checkable;
	bool selectable;

	bool custom_color;
	Color color;


	
public:

	Signal<> selected_signal;
	Signal< Method1<bool> > checked_signal;
	void set_checked(bool p_checked);
	void set_text(String p_text);
	void set_bitmap(BitmapID p_bitmap);
	void set_collapsed(bool p_collapsed);

	bool is_checked();
	String get_text();
	BitmapID get_bitmap();
	bool is_collapsed();

	TreeItem *get_next();
	TreeItem *get_parent();
	TreeItem *get_childs();


	
	void remove_child(TreeItem *p_item);


	void set_checkable(bool p_checkable);
	void set_selectable(bool p_selectable);

	bool is_selected();
	void select();
	void deselect();

	void set_custom_color(const Color& p_color);
	void clear_custom_color();
	TreeItem();
	~TreeItem();
};


class Tree : public Widget {

	

	void redraw_all();
	
	Size get_item_size(TreeItem *root);
	void tree_changed_callback();
	Size get_minimum_size_internal();

	int draw_item(const Point& p_pos,const Rect& p_exposed,TreeItem *p_item);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);

	int compute_item_height(TreeItem *p_item);
	int propagate_mouse_event(const Point &p_pos,bool p_doubleclick,TreeItem *p_item,int p_mod_state); //return true on handled
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	

	bool allow_multi; //allow multiple item selection

	void item_selected(TreeItem *p_item);
	void item_deselected(TreeItem *p_item);

	void select_single_item(TreeItem *p_item,TreeItem *p_current);

friend class TreeItem;
	
	TreeItem *root;	
	
public:

	void clear();

	TreeItem *create_item(TreeItem *p_parent=0); /// create item as toplevel (replacing it if it exists), or alternatively, create item with another parent
	TreeItem *create_item(String p_text,TreeItem *p_parent=0); /// create item as toplevel (replacing it if it exists), or alternatively, create item with another parent
	TreeItem *create_item(BitmapID p_bitmap, String p_text,TreeItem *p_parent=0); /// create item as toplevel (replacing it if it exists), or alternatively, create item with another parent

	TreeItem* get_root_item();
	
	void set_multi_mode(bool p_enabled);
	Tree();
	~Tree();
};

}


#endif
