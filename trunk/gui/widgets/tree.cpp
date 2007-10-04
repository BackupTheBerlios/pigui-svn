//
// C++ Implementation: tree
//
// Description:
//
//
// Author: red,,, <red@hororo>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tree.h"
#include "base/painter.h"
#include "base/skin.h"
#include <stdio.h>
namespace GUI {


void TreeItem::set_checked(bool p_checked) {

	if (!checkable) {

		PRINT_ERROR("Item not Checkable");
		return;
	}
	checked=p_checked;
	changed_signal.call();
	checked_signal.call(checked);

}
void TreeItem::set_text(String p_text) {

	text=p_text;
	changed_signal.call();

}
void TreeItem::set_bitmap(BitmapID p_bitmap) {

	bitmap=p_bitmap;
	changed_signal.call();


}
void TreeItem::set_collapsed(bool p_collapsed) {

	collapsed=p_collapsed;
	changed_signal.call();

}

bool TreeItem::is_selected() {

	return selected;
}
bool TreeItem::is_checked() {

	return checked;
}
String TreeItem::get_text() {


	return text;
}
BitmapID TreeItem::get_bitmap() {

	return bitmap;
}
bool TreeItem::is_collapsed() {

	return collapsed;
}

TreeItem *TreeItem::get_next() {


	return next;
}

TreeItem *TreeItem::get_parent() {

	return parent;
}
TreeItem *TreeItem::get_childs() {


	return childs;
}



void TreeItem::remove_child(TreeItem *p_item) {

	TreeItem **c=&childs;

	while (*c) {

		if ( (*c) == p_item ) {

			TreeItem *aux = *c;

			*c=(*c)->next;

			aux->parent = 0;
			return;
		}

		c=&(*c)->next;
	}

	PRINT_ERROR("Unable to delete child (unexistent)");

}

void TreeItem::set_checkable(bool p_checkable) {

	checkable=p_checkable;
}

void TreeItem::select() {

	internal_selected_signal.call(this);
}

void TreeItem::deselect() {

	internal_deselected_signal.call(this);
}

void TreeItem::set_selectable(bool p_selectable) {


	selectable=p_selectable;
}

void TreeItem::set_custom_color(const Color& p_color) {


	custom_color=true;
	color=p_color;
	changed_signal.call();
}
void TreeItem::clear_custom_color() {

	changed_signal.call();

}

TreeItem::TreeItem() {

	custom_color=false;
	bitmap=-1;
	checked=false;
	collapsed=false;
	selected = false;

	parent=0; // parent item
	next=0; // next in list
	childs=0; //child items
	tree=0; //tree (for reference)

	checkable=false;
	selectable=true;

}
TreeItem::~TreeItem() {

	TreeItem *c=childs;
	while (c) {

		TreeItem *aux=c;
		c=c->get_next();
		aux->parent=0; // so it wont try to recursively autoremove from me in here
		delete aux;
	}

	if (parent)
		parent->remove_child(this);

	if (tree && tree->root==this) {

		tree->root=0;
	}

}

// -=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=


Size Tree::get_item_size(TreeItem *p_item) {


	Size size;
	size.width += get_painter()->get_font_string_width( font( FONT_TREE) , p_item->text );
	size.height = get_painter()->get_font_height( font( FONT_TREE) );

	if ( p_item->bitmap>=0 && get_painter()->is_bitmap_valid( p_item->bitmap ) ) {

		Size bmpsize=get_painter()->get_bitmap_size( p_item->bitmap );
		size.width+=bmpsize.width + constant( C_TREE_HSPACING );
		if (size.height<bmpsize.height)
			size.height=bmpsize.height;

	}

	if (p_item->checkable) {

		size.width+=constant(C_TREE_CHECK_SIZE);
		size.width+=constant( C_TREE_HSPACING );
		if (size.height<constant(C_TREE_CHECK_SIZE))
			size.height=constant(C_TREE_CHECK_SIZE);

	}

	size.height += constant( C_TREE_VSPACING );
	size.width += constant( C_TREE_GUIDE_WIDTH );


	if (!p_item->collapsed) { /* if not collapsed, check the childs */

		TreeItem *c=p_item->childs;

		while (c) {

			Size child_size = get_item_size( c );
			child_size.width+=constant( C_TREE_GUIDE_WIDTH ); //needed for comparing

			size.height+=child_size.height;
			if (size.width<child_size.width)
				size.width=child_size.width;

			c=c->next;
		}
	}

	return size;
}

int Tree::draw_item(const Point& p_pos,const Rect& p_exposed,TreeItem *p_item) { //return height


	if (p_pos.y > (p_exposed.pos.y + p_exposed.size.y))
		return -1; //draw no more!

	Point pos=p_pos;

	int htotal=0;

	int label_h=compute_item_height( p_item );

	/* Calculate height of the label part */


	label_h+=constant( C_TREE_VSPACING );

	/* Draw label, if height fits */

	bool draw_guide=false;
	Point guide_from;

	if ((pos.y+label_h)>p_exposed.pos.y) {

		int height=label_h-constant( C_TREE_VSPACING );


		pos.y+=constant( C_TREE_VSPACING );

		Point guide_space=Point( constant( C_TREE_GUIDE_WIDTH ) , height );

		if (p_item->childs) { //has childs, draw the guide box
			
			Point guide_box_size = Point( constant(C_TREE_GUIDE_BOX_SIZE),constant(C_TREE_GUIDE_BOX_SIZE));
			
			Direction dir;
			
			if (p_item->collapsed) {
			
				dir=DOWN;
			} else {
				dir=RIGHT;
				
			}
			
			get_painter()->draw_arrow(pos+((guide_space-guide_box_size)/2),guide_box_size ,dir,color(COLOR_TREE_GUIDES));
			
	
		} 

		//draw separation...
		pos.x+=constant( C_TREE_GUIDE_WIDTH );



		if ( p_item->bitmap>=0 && get_painter()->is_bitmap_valid( p_item->bitmap ) ) {

			Size bmpsize=get_painter()->get_bitmap_size( p_item->bitmap );
			get_painter()->draw_bitmap(p_item->bitmap, pos+Point(0, (height-bmpsize.y)/2 ));

			pos.x+=bmpsize.x+constant( C_TREE_HSPACING );

		}

		
		if (p_item->checkable) {
	
			Point box_b=pos;
			box_b.y+=(label_h-constant( C_TREE_VSPACING )-constant(C_TREE_CHECK_SIZE))/2 ;
			
			if (p_item->checked) {
				
				get_painter()->draw_rect(box_b,Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE)),color(COLOR_TREE_GUIDES));
				get_painter()->draw_fill_rect(box_b+Point(2,2),Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE))-Size(4,4),color(COLOR_TREE_GUIDES));
				
			} else {
				get_painter()->draw_rect(box_b,Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE)),color(COLOR_TREE_GUIDES));
				
			}
			
			pos.x+=constant(C_TREE_CHECK_SIZE)+constant( C_TREE_HSPACING );
				
		}

		int font_height=get_painter()->get_font_height( font( FONT_TREE ) );
		int font_ascent=get_painter()->get_font_ascent( font( FONT_TREE ) );
		int text_width=get_painter()->get_font_string_width( font( FONT_TREE ) , p_item->text);
		
		if (p_item->selected) {

			get_painter()->draw_stylebox(stylebox( SB_TREE_SELECTED ),pos,Size(text_width+constant( C_TREE_HSPACING ),label_h) );
		}

		Color col=p_item->custom_color?p_item->color:color( COLOR_TREE_FONT );
		get_painter()->draw_text( font( FONT_TREE ), pos+Point(0, (label_h-font_height)/2 + font_ascent - constant( C_TREE_VSPACING )), p_item->text,col );

		pos=p_pos; //reset pos

	}


	htotal+=label_h;
	pos.y+=htotal;
	pos.x+=constant( C_TREE_GUIDE_WIDTH );


	if (!p_item->collapsed) { /* if not collapsed, check the childs */

		TreeItem *c=p_item->childs;

		while (c) {

			int child_h=draw_item(pos, p_exposed, c );

			if (child_h<0)
				return -1; // break, stop drawing, no need to anymore

			htotal+=child_h;
			pos.y+=child_h;
			c=c->next;
		}
	}

	
	return htotal;


}

int Tree::compute_item_height(TreeItem *p_item) {

	int item_h = get_painter()->get_font_height( font( FONT_TREE) );

	if ( p_item->bitmap>=0 && get_painter()->is_bitmap_valid( p_item->bitmap ) ) {

		Size bmpsize=get_painter()->get_bitmap_size( p_item->bitmap );
		if (item_h<bmpsize.height)
			item_h=bmpsize.height;
	}

	if (p_item->checkable) {

		if (item_h<constant(C_TREE_CHECK_SIZE))
			item_h=constant(C_TREE_CHECK_SIZE);

	}

	return item_h;

}

int Tree::propagate_mouse_event(const Point &p_pos,bool p_doubleclick,TreeItem *p_item,int p_mod_state) {

	int item_h=compute_item_height( p_item )+constant( C_TREE_VSPACING );


	if (p_pos.y<item_h) {
		// check event!


		int x=p_pos.x;
		if ( x< constant(C_TREE_GUIDE_WIDTH) ) {


			if (p_item->childs)
				p_item->set_collapsed( ! p_item->is_collapsed() );

			return -1; //handled!
		}

		x-=constant(C_TREE_GUIDE_WIDTH);


		if (p_item->checkable) {

			int checkw=constant(C_TREE_CHECK_SIZE)+constant( C_TREE_HSPACING )*2;


			if (x<checkw) {

				p_item->set_checked( !p_item->checked );
				return -1;
			}

			x-=checkw;
		}



		if (allow_multi && p_mod_state&KEY_MASK_CTRL) {

			if (!p_item->selected) {
				p_item->selected=true;
				p_item->selected_signal.call();
			} else {

				p_item->selected=false;

			}

			update();
		} else {

			p_item->select();
		}

		return -1; //select
	} else {
		
		Point new_pos=p_pos;
		new_pos.x-=constant( C_TREE_GUIDE_WIDTH );
		new_pos.y-=item_h;

		if (!p_item->collapsed) { /* if not collapsed, check the childs */

			TreeItem *c=p_item->childs;

			while (c) {

				int child_h=propagate_mouse_event( new_pos,p_doubleclick,c,p_mod_state);

				if (child_h<0)
					return -1; // break, stop propagating, no need to anymore

				new_pos.y-=child_h;
				c=c->next;
				item_h+=child_h;
			}
		}

		

	}

	return item_h; // nothing found

}

void Tree::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {


	if (p_button!=BUTTON_LEFT)
		return;

	if (!p_press)
		return;


	propagate_mouse_event(p_pos,false,root,p_modifier_mask);

}

void Tree::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {


	get_painter()->draw_stylebox( stylebox( SB_TREE_NORMAL ), Point() , p_size, p_exposed );
	if (has_focus())
		get_painter()->draw_stylebox( stylebox( SB_TREE_FOCUS ), Point() , p_size, p_exposed);

	if (!root)
		return;

	draw_item(Point(), p_exposed,root);
}


Size Tree::get_minimum_size_internal() {

	Size size = root?get_item_size(root):Size();


	return size;
}

void Tree::tree_changed_callback() {

	update();
}



TreeItem *Tree::create_item(TreeItem *p_parent) {


	return create_item(-1,"",p_parent);
}

TreeItem *Tree::create_item(String p_text,TreeItem *p_parent) {


	return create_item(-1,p_text,p_parent);
}
TreeItem *Tree::create_item(BitmapID p_bitmap, String p_text,TreeItem *p_parent) {

	TreeItem *ti = new TreeItem;
	if (!ti) {

		PRINT_ERROR("Can't create new TreeItem");
		return 0;
	}

	if (p_parent) {

		/* Always append at the end */

		TreeItem *last=0;
		TreeItem *c=p_parent->childs;

		while(c) {

			last=c;
			c=c->next;
		}

		if (last) {

			last->next=ti;
		} else {

			p_parent->childs=ti;
		}

	} else {

		if (root)
			ti->childs=root;

		root=ti;

	}

	ti->set_bitmap( p_bitmap );
	ti->set_text( p_text );
	ti->changed_signal.connect(this,&Tree::redraw_all);
	ti->internal_selected_signal.connect(this,&Tree::item_selected);
	ti->internal_deselected_signal.connect(this,&Tree::item_deselected);
	return ti;

}

TreeItem* Tree::get_root_item() {

	return root;	
};


void Tree::redraw_all() {

 update();
}
void Tree::select_single_item(TreeItem *p_item,TreeItem *p_current) {



	if (p_item==p_current) {

		if (!p_item->selected) {

			p_item->selected=true;
			p_item->selected_signal.call();
		}
	} else {

		p_current->selected=false;
	}


	TreeItem *c=p_current->childs;


	while (c) {

		select_single_item(p_item,c);
		c=c->next;
	}
}

void Tree::item_selected(TreeItem *p_item) {


	select_single_item(p_item,root);

	update();
}
void Tree::item_deselected(TreeItem *p_item) {

	p_item->selected=false;

	update();
}


void Tree::set_multi_mode(bool p_enabled) {


	allow_multi=p_enabled;
}

void Tree::clear() {

	if (root) {
		delete root;
		root = NULL;
	};
};

Tree::Tree() {

	allow_multi=true;
	root=0;
}


Tree::~Tree() {

	if (root)
		delete root;
}


}
