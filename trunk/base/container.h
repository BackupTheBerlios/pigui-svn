//
// C++ Interface: container
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H


#include "base/frame.h"
#include "base/window.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

namespace GUI {


class ContainerChild {
protected:
friend class Container;

	Frame *frame;
	
	ContainerChild * next;
	ContainerChild * prev;

public:

	inline const Frame *get_frame() const { return frame; }
	inline Frame *get_frame() { return frame; }
	inline const ContainerChild *get_next() const { return next; }
	inline const ContainerChild *get_prev() const { return prev; }
	inline ContainerChild *get_next() { return next; }
	inline ContainerChild *get_prev() { return prev; }

	inline ContainerChild() { frame=NULL; next=NULL; prev=NULL; }

};

class ContainerPrivate;

class Container : public Frame {


	virtual void set_window(Window *p_window); 
	ContainerPrivate *_cp;

	void set_resizing(bool p_resizing);
	void set_pending_resize_tree();	
protected:

	ContainerChild *add_frame_base( Frame *p_frame,bool p_front=false );

	/**
	 * Resize the children to a given size. Children are expected to fit.
         * p_size will never be smaller than get_minimum_size()
	 * @param r_size size
	 */
	virtual void resize_children(Size& p_size)=0;

	/**
	 * Create a container child.
	 * Often, containers may want to create their own derivates of containerchild, to store 
	 * extra information on them.
	 * @return container child.
	 */
	virtual ContainerChild *create_container_child()=0; 


	void resize();
	
private:
friend class Window;

	void window_request_size(const Size& p_size);
	void draw_tree(const Rect& p_exposed);
	
public:

	void check_minimum_size();

	ContainerChild *get_child_list();
	ContainerChild *get_child_list_last();

	const ContainerChild *get_child_list() const;
	const ContainerChild *get_child_list_last() const;

	bool has_child(Frame *p_frame) const;

	void remove_child( Frame *p_frame );

	Container *get_root() const;


	bool is_resizing() const;

	
	
	Frame *find_next_focusable(Frame *p_to) const;
	Frame *find_prev_focusable(Frame *p_to) const;

	Container();	
	~Container();

};

} // end of namespace
#endif
