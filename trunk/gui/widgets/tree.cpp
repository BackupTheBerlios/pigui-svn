//Tre
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


/* cell mode */
void TreeItem::set_cell_mode( int p_column, TreeCellMode p_mode ) {

	if (p_column<0 || p_column >=tree->columns)
		return;
		
	Cell &c = cells[ p_column ];
	
	if (c.mode == p_mode )
		return;
	if (c.mode == CELL_MODE_RANGE && c.data.range && c.owned_range) {
	
		GUI_DELETE( c.data.range );
		c.data.range=NULL;
		c.owned_range=false;
	}
	
	if ( p_mode == CELL_MODE_RANGE) {
	
		c.data.range = GUI_NEW( Range );
		c.owned_range=true;
		c.string = "";
		
	}
	
	c.mode = p_mode;
	
	changed_signal.call(p_column);
}
TreeCellMode TreeItem::get_cell_mode( int p_column ) {

	if (p_column<0 || p_column >=tree->columns) 
		return CELL_MODE_STRING;
		
	return cells[ p_column ].mode;
		
}

/* check mode */
void TreeItem::set_checked(int p_column,bool p_checked) {

	if (p_column<0 || p_column >=tree->columns)
		return;
	
	Cell &c = cells[ p_column ];

	if (c.mode != CELL_MODE_CHECK )
		return;
		
	c.data.checked=p_checked;
	
	changed_signal.call(p_column);
	
}
bool TreeItem::is_checked(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return false;

	Cell &c = cells[ p_column ];
	
	return c.data.checked;

}

void TreeItem::set_text(int p_column,String p_text) {

	if (p_column<0 || p_column >=tree->columns)
		return;

	Cell &c = cells[ p_column ];
	
	c.string=p_text;
	changed_signal.call(p_column);

}
String TreeItem::get_text(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return "";
	Cell &c = cells[ p_column ];
	return c.string;
}

void TreeItem::set_bitmap(int p_column,BitmapID p_bitmap) {

	if (p_column<0 || p_column >=tree->columns)
		return;

	Cell &c = cells[ p_column ];
	if (c.mode!=CELL_MODE_BITMAP)
		return;
	c.data.bitmap=p_bitmap;
	
	changed_signal.call(p_column);
}
BitmapID TreeItem::get_bitmap(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return INVALID_BITMAP_ID;
		
	Cell &c = cells[ p_column ];
		
	if (c.mode!=CELL_MODE_BITMAP)
		return INVALID_BITMAP_ID;

	return c.data.bitmap;
}

/* range works for mode number or mode combo */
void TreeItem::set_range(int p_column,RangeBase *p_range,bool p_own_range)  {

	if (p_column<0 || p_column >=tree->columns) 
		return;
		
	Cell &c = cells[ p_column ];
		
	if (c.mode!=CELL_MODE_RANGE)
		return;

	if (c.owned_range && c.data.range)
		GUI_DELETE( c.data.range );
	c.data.range=p_range;
	c.owned_range=p_own_range;
}

RangeBase *TreeItem::get_range(int p_column)   {

	if (p_column<0 || p_column >=tree->columns) 
		return NULL;

	Cell &c = cells[ p_column ];
		
	if (c.mode!=CELL_MODE_RANGE)
		return NULL;

	return c.data.range;
}

void TreeItem::set_collapsed(bool p_collapsed) {

	collapsed=p_collapsed;
	collapsed_signal.call();
}

bool TreeItem::is_selected(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return false;
	
	Cell &c = cells[ p_column ];
	
	return c.selected;
	
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

void TreeItem::select(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return;
	
	Cell &c = cells[ p_column ];
	
	if (!c.selectable)
		return;
	c.selected=true;
	
	selected_signal.call( p_column );
}

void TreeItem::deselect(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return;
	
	Cell &c = cells[ p_column ];
	
	if (!c.selectable)
		return;
		
	c.selected=false;
	
	deselected_signal.call( p_column );
}

void TreeItem::set_selectable(int p_column,bool p_selectable) {

	if (p_column<0 || p_column >=tree->columns) 
		return;
	
	Cell &c = cells[ p_column ];
	
	c.selectable=p_selectable;
		return;

}

void TreeItem::set_custom_color(int p_column,const Color& p_color) {

	if (p_column<0 || p_column >=tree->columns) 
		return;
	
	Cell &c = cells[ p_column ];
	
	c.custom_color=true;
	c.color=p_color;
	
	changed_signal.call(p_column);

}

void TreeItem::clear_custom_color(int p_column) {

	if (p_column<0 || p_column >=tree->columns) 
		return;
	
	Cell &c = cells[ p_column ];
	
	c.custom_color=false;
	
	changed_signal.call(p_column);

}

TreeItem::TreeItem(Tree *p_tree) {

	tree=p_tree;
	cells = GUI_NEW_ARRAY( Cell, tree->columns );
	collapsed=false;

	parent=0; // parent item
	next=0; // next in list
	childs=0; //child items


}

TreeItem::~TreeItem() {

	TreeItem *c=childs;
	while (c) {

		TreeItem *aux=c;
		c=c->get_next();
		aux->parent=0; // so it wont try to recursively autoremove from me in here
		GUI_DELETE( aux );
	}

	if (parent)
		parent->remove_child(this);

	if (tree && tree->root==this) {

		tree->root=0;
	}

	GUI_DELETE_ARRAY( cells );
}

// -=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=


int Tree::get_item_height(TreeItem *p_item) {

	int height=compute_item_height(p_item);
	
	if (!p_item->collapsed) { /* if not collapsed, check the childs */

		TreeItem *c=p_item->childs;

		while (c) {

			height += get_item_height( c );

			c=c->next;
		}
	}

	return height;
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
		int font_ascent=get_painter()->get_font_ascent( font( FONT_TREE ) );

		int ofs = pos.x;
		for (int i=0;i<columns;i++) {
		
			int w = get_column_width(i);
						
			if (i==0) {
				w-=pos.x;
				if (w<=0) {
				
					ofs=get_column_width(0);
					continue;
				}
			}
			
			ofs+=constant( C_TREE_HSPACING );
			w-=constant( C_TREE_HSPACING );
			
			Rect item_rect = Rect( Point( ofs, pos.y ), Size( w, label_h ));
	
			if (p_item->cells[i].selected) {

				get_painter()->draw_stylebox(stylebox( SB_TREE_SELECTED ),item_rect.pos,item_rect.size );
			}

			get_painter()->push_clip_rect( item_rect );
			
			Color col=p_item->cells[i].custom_color?p_item->cells[i].color:color( COLOR_TREE_FONT );
			Point text_pos=item_rect.pos;
			text_pos.y+=font_ascent;

			switch (p_item->cells[i].mode) {
			
				case CELL_MODE_STRING: {
				
					get_painter()->draw_text( font(FONT_TREE), text_pos, p_item->cells[i].string, col,item_rect.size.x );
				} break;
				case CELL_MODE_CHECK: {
				
					Point check_ofs=item_rect.pos;
					check_ofs.y+=(item_rect.size.y-constant(C_TREE_CHECK_SIZE))/2;
				
					if (p_item->cells[i].data.checked) {
						
						get_painter()->draw_rect(check_ofs,Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE)),color(COLOR_TREE_GUIDES));
						get_painter()->draw_fill_rect(check_ofs+Point(2,2),Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE))-Size(4,4),color(COLOR_TREE_GUIDES));
						
					} else {
						get_painter()->draw_rect(check_ofs,Size(constant(C_TREE_CHECK_SIZE),constant(C_TREE_CHECK_SIZE)),color(COLOR_TREE_GUIDES));
						
					}
				
					int check_w = constant(C_TREE_CHECK_SIZE)+constant( C_TREE_HSPACING );
					
					text_pos.x+=check_w;
					
					
					get_painter()->draw_text( font(FONT_TREE), text_pos, p_item->cells[i].string, col,item_rect.size.x-check_w );
				
				} break;
				case CELL_MODE_RANGE: {
				
					if (p_item->cells[i].range_is_combo) {
					
						//?
						int arrow_w = item_rect.size.y/2;
						Point arrow_pos=item_rect.pos;
						arrow_pos.x+=item_rect.size.x-arrow_w;
							
						get_painter()->draw_arrow( arrow_pos,Size(arrow_w,item_rect.size.y), DOWN, color(COLOR_TREE_GUIDES));
						
					} else {
					
						RangeBase *r=p_item->cells[i].data.range;
						if (!r)
							break;
							
						get_painter()->draw_text( font(FONT_TREE), text_pos, r->get_as_text(), col,item_rect.size.x );
						
						int updown_w = item_rect.size.y/2;
						Point updown_pos=item_rect.pos;
						updown_pos.x+=item_rect.size.x-updown_w;
						
						get_painter()->draw_arrow( updown_pos,Size(updown_w,updown_w), UP, color(COLOR_TREE_GUIDES));
						get_painter()->draw_arrow( updown_pos+Point(0,updown_w),Size(updown_w,updown_w), DOWN, color(COLOR_TREE_GUIDES));
						
						
					}
				
				} break;
				case CELL_MODE_BITMAP: {
				
					Size bitmap_size = get_painter()->get_bitmap_size( p_item->cells[i].data.bitmap);
					
					Point bitmap_ofs = (item_rect.size-bitmap_size)/2;
					bitmap_ofs+=item_rect.pos;
					
					get_painter()->draw_bitmap( p_item->cells[i].data.bitmap, bitmap_ofs, col );
				
				} break;
				case CELL_MODE_CUSTOM: {
				
					get_painter()->draw_text( font(FONT_TREE), text_pos, p_item->cells[i].string, col,item_rect.size.x );
				
					int arrow_w = item_rect.size.y/2;
					Point arrow_pos=item_rect.pos;
					arrow_pos.x+=item_rect.size.x-arrow_w;
						
					get_painter()->draw_arrow( arrow_pos,Size(arrow_w,item_rect.size.y), DOWN, color(COLOR_TREE_GUIDES));
									
				} break;
			}
			
			get_painter()->pop_clip_rect();
			
			if (i==0) {
			
				ofs=get_column_width(0);
			} else {
			
				ofs+=w;
			}
			
		}
		//separator
		//get_painter()->draw_fill_rect( Point(0,pos.y),Size(get_size_cache().width,1),color( COLOR_TREE_GRID) );
	
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

	int height=get_painter()->get_font_height( font( FONT_TREE) );
	
	for (int i=0;i<columns;i++) {
	
		switch(p_item->cells[i].mode) {
		
			case CELL_MODE_CHECK: {
			
				
				if (height<constant(C_TREE_CHECK_SIZE))
					height=constant(C_TREE_CHECK_SIZE);
			
				
			
			} break;
			case CELL_MODE_BITMAP: {
			
				BitmapID bitmap = p_item->cells[i].data.bitmap;
				if (bitmap!=INVALID_BITMAP_ID) {
				
					Size  s = get_painter()->get_bitmap_size( bitmap );
					if (s.height > height )
						height=s.height;
				}
				
			} break;
			default: {}
		}
	}
	

	height += constant( C_TREE_VSPACING );

	return height;

}

int Tree::propagate_mouse_event(const Point &p_pos,bool p_doubleclick,TreeItem *p_item,int p_mod_state) {

	int item_h=compute_item_height( p_item )+constant( C_TREE_VSPACING );


	if (p_pos.y<item_h) {
		// check event!

		/*
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

		*/
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
	
	int ofs=0;
	int from_y=p_exposed.pos.y+get_painter()->get_stylebox_margin( stylebox(SB_TREE_NORMAL), MARGIN_TOP);
	int size_y=p_exposed.size.height-get_painter()->get_stylebox_min_size(stylebox(SB_TREE_NORMAL)).height;


	for (int i=0;i<(columns-1);i++) {
		
		ofs+=get_column_width(i);
		get_painter()->draw_fill_rect( Point(ofs+constant( C_TREE_HSPACING )/2, from_y), Size( 1, size_y ),color( COLOR_TREE_GRID) );
	}
}


Size Tree::get_minimum_size_internal() {

	int w=0;
	
	for (int i=0;i<columns;i++) {
	
		w+=column_min_size[i];
	}
	
	int h = root?get_item_height(root):0;

	return Size(w,h);
}

void Tree::tree_changed_callback() {

	update();
}



TreeItem *Tree::create_item(TreeItem *p_parent) {

	TreeItem *ti = GUI_NEW( TreeItem(this) );
	
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

	ti->changed_signal.connect( Method1<int>(Method2<int,TreeItem*>(this,&Tree::item_changed),ti));
	
	ti->selected_signal.connect( Method1<int>(Method2<int,TreeItem*>(this,&Tree::item_selected),ti));

	ti->deselected_signal.connect( Method1<int>(Method2<int,TreeItem*>(this,&Tree::item_deselected),ti));

	return ti;

}

TreeItem* Tree::get_root_item() {

	return root;	
};


void Tree::item_changed(int p_column,TreeItem *p_item) {

	update(); //just redraw all for now, could be optimized for asking a redraw with exposure for the item
}
void Tree::select_single_item(TreeItem *p_item,TreeItem *p_current) {

/*
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
	*/
}

void Tree::item_selected(int p_column,TreeItem *p_item) {


	select_single_item(p_item,root);

	update();
}
void Tree::item_deselected(int p_column,TreeItem *p_item) {

	update();
}


void Tree::set_multi_mode(bool p_enabled) {


	allow_multi=p_enabled;
}

void Tree::clear() {

	if (root) {
		GUI_DELETE( root );
		root = NULL;
	};
};


void Tree::compute_column_size_caches() {


}
int Tree::get_column_width(int p_column) {

	if (p_column<0 || p_column >= columns)
		return 0;
		
	return column_width_caches[p_column];
}

Tree::Tree(int p_columns) {

	if (p_columns<1)
		p_columns=1;
	
	if (p_columns>255)
		p_columns=255;
		
	columns=p_columns;
	column_min_size=GUI_NEW_ARRAY( int, columns );
	column_expand=GUI_NEW_ARRAY( bool, columns );
	column_width_caches=GUI_NEW_ARRAY( int, columns );
	
	for (int i=0;i<columns;i++) {
	
		column_expand[i]=true;
		column_min_size[i]=100;
		column_width_caches[i]=100;
	}
	
	allow_multi=true;
	root=0;
}


Tree::~Tree() {

	if (root) {
		GUI_DELETE( root );
	}
	GUI_DELETE_ARRAY( column_min_size );
	GUI_DELETE_ARRAY( column_expand );
	GUI_DELETE_ARRAY( column_width_caches );
	
}


}
