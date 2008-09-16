
//
// C++ Implementation: container
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "container.h"

namespace GUI {

class ContainerPrivate {
public:

	struct Root {
		// data only meaningful if you are the root container
		bool resizing;
	} root;

	bool pending_resize;
	List<Container*>::Element *pending_resize_request;

	ContainerChild *child_list_begin;
	ContainerChild *child_list_end;

	Size minsize_cache;

	ContainerPrivate() {

		pending_resize=false;
		pending_resize_request=NULL;
		child_list_begin=NULL;
		child_list_end=NULL;
		root.resizing=false;
	};
};


void Container::set_pending_resize_tree() {

	if (_cp->pending_resize) {
	
		// we found a level of the tree downwards that is already pending resize,
		// so we should quit.
		
		if ( _cp->pending_resize_request ) {
			// before quitting, we should make sure to take over its pending resize request,
			// so the window will ask *us* to resize instead of this previous, less important, one.
			
			_cp->pending_resize_request->erase(); // this list allows us to do this.
			_cp->pending_resize_request=NULL;
			
		}
		
		return;
	}
	
	for( ContainerChild *I = _cp->child_list_begin ; I ; I=I->next ) {
	
		if (I->frame->container_cast())
			I->frame->container_cast()->set_pending_resize_tree();
	}
}

void Container::check_minimum_size() {

	if (_cp->pending_resize)
		return;
		
	if (!get_window())
		return;
		
	if (_cp->pending_resize_request) {
	
		GUI_PRINT_ERROR("Bug, pending resize request not cleared");
		return;
	}
	_cp->pending_resize_request = get_window()->add_container_pending_resize(this);
	_cp->pending_resize=true;
	// set tree to wait for a resize
	for( ContainerChild *I = _cp->child_list_begin ; I ; I=I->next ) {
	
		if (I->frame->container_cast())
			I->frame->container_cast()->set_pending_resize_tree();
	}
}


void Container::set_resizing(bool p_resizing) {

	if (!get_window())
		return;
	if (!get_root())
		return;

	get_root()->_cp->root.resizing=p_resizing;
}

/**
 * This is true when a container is resizing children.
 * @return true when resizing.
 */
bool Container::is_resizing() const {

	if (!get_window())
		return false;
	if (!get_root())
		return false;

	return get_root()->_cp->root.resizing;
}

void Container::set_window(Window *p_window) {

	if (get_window() && p_window!=NULL) {
		return;
	}

	if (!get_window() && !p_window) {
		return;		
	}
	
	Frame::set_window(p_window);
	ContainerChild *c=_cp->child_list_begin;

	while (c) {

		c->frame->set_window(p_window);
		c=c->next;
	}
}

ContainerChild *Container::add_frame_base( Frame *p_frame,bool p_front ) {

	if (p_frame->get_parent()) // poointless
		return NULL;

	ContainerChild* new_child = create_container_child();
	
	if (!new_child) {

		GUI_PRINT_ERROR("Inherited Container BUG.");
		return NULL;
	}

	if (_cp->child_list_begin) { //if exists
		
		if (p_front) {
			
			_cp->child_list_begin->prev=new_child;
			new_child->next=_cp->child_list_begin;
			_cp->child_list_begin=new_child;
		} else {
			_cp->child_list_end->next=new_child;
			new_child->prev=_cp->child_list_end;
			_cp->child_list_end=new_child;
		}
	} else { //if it doesnt
		
		_cp->child_list_begin=new_child;
		_cp->child_list_end=new_child;
	}

	new_child->frame=p_frame;

	p_frame->set_parent(this);

	if (get_window()) {

		p_frame->set_window( get_window() );
	}

	return new_child;
}



void Container::draw_tree(const Rect& p_exposed) {

	
}

void Container::window_request_size(const Size& p_size) {


	set_rect( Rect( Point(), p_size ) );

}

void Container::resize() {
	
	if (!get_window()) {

		GUI_PRINT_ERROR("Attempt resize without Window.");
		return;
	}

	//@TODO maybe we could skip this entire check and try to find is someone has heigh for width...

	Size size = get_size(); 
	
	// substract the decoration size from the actual size
	StyleBox style = theme( get_parent() ? SB_CONTAINER : ( get_window()->get_state( WINDOW_STATE_POPUP ) ? SB_POPUP_CONTAINER : SB_ROOT_CONTAINER ) );	
	size-=style.get_minimum_size();
	
	
	Size minsize = get_minimum_size();

	if (minsize.width > _cp->minsize_cache.width || _cp->minsize_cache.height > get_size().height || size.width < _cp->minsize_cache.width || size.height < _cp->minsize_cache.height) {
		// children are ALREADY bigger than me, even without height for width, so
		// let parent (or window) handle the resizing
		if (get_parent()) {

			get_parent()->resize();
			return;
		} 
		// if there is no parent (we are container root)
		// then just keep going, we'll ask for the window to resize when we know the real
		// minsize (with height for width ).
	}

	// This should be a safe bet, without height for width everything should fit
	resize_children( size );

	// enable REAL resizing
	set_resizing(true);

	// since the "width" of the childrens in the tree should now be correct thanks to the
	// resize_children two lines ago, we will try once more to do the resize, with 
	// height for width enabled. This is the REAL resize check.

	minsize = get_minimum_size(); // check minimum size with height for width enabled.

	if (minsize.width!=_cp->minsize_cache.width || minsize.height!=_cp->minsize_cache.height ) {
		// minimum size really changed, pass up the responsibility
		set_resizing(false);

		if (get_parent()) {

			get_parent()->resize();

			return;
		} else {
			// should resize window
			get_window()->set_minimum_size_changed(minsize);
			return;
		}		
  	}

	// since minsize didn't change, let's just reorganize everything normally then
	resize_children(size); 
	
	set_resizing(false);
}

Container *Container::get_root() const {

	if (!get_window())
		return NULL;

	return get_window()->get_root_container();
}

Frame *Container::find_next_focusable(Frame *p_to) const {

	return NULL;
}
Frame *Container::find_prev_focusable(Frame *p_to) const {

	return NULL;
}

void Container::remove_child( Frame *p_frame ) {

}

bool Container::has_child(Frame *p_frame) const {

	const ContainerChild *c = get_child_list();
	while(c) {

		if (c->get_frame()==p_frame)
			return true;

		c=c->get_next();
	}

	return false;
}
Container::Container()
{
}


Container::~Container()
{
}


}
