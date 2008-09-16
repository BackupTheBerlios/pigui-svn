//
// C++ Interface: platform
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLATFORM_H
#define PLATFORM_H

#include "base/pixmap.h"
#include "base/font.h"
#include "base/style_box.h"
#include "base/constants.h"
#include "base/geometry.h"
#include "base/object.h"


//#include "base/theme.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

namespace GUI {

class Platform;
class Theme;

class PlatformBase : public Object {
	const Platform *owner;
protected:
	PlatformBase(Platform *p_owner) { owner = p_owner; }
public:
	inline const Platform *get_owner() const { return owner; };

};

class PlatformTheme : public PlatformBase {
protected:
	PlatformTheme(Platform *p_owner) : PlatformBase(p_owner) {};
public:

	virtual Theme * get_theme()=0;
	virtual void configure_default()=0;

	virtual ~PlatformTheme();
};

class PlatformWindow : public PlatformBase {
protected:
	PlatformWindow(Platform *p_owner) : PlatformBase(p_owner) {};
public:
	/* WM Interface */

	virtual void set_title(String p_title)=0;
	virtual String get_title() const=0;

	virtual void set_pos(const Point& p_pos)=0;
	virtual Point get_pos() const=0;

	virtual void set_size(const Point& p_pos)=0;
	virtual Size get_size() const=0;

	virtual void set_icon(const Pixmap& p_icon)=0;
	
	virtual void set_state(WindowState p_state,bool p_enabled)=0;
	virtual bool get_state(WindowState p_state)=0;

	/* drawing interface */

	virtual void draw_rect(const Point& p_from,const Size& p_size,const Color& p_color,bool p_fill=true)=0;
	virtual void draw_line(const Point& p_from,const Point& p_to,const Color& p_color,int p_width=1,bool p_round_endpoints=false)=0;
	virtual void draw_polygon(const Point *p_points, int p_point_count,const Color& p_color,bool p_fill=true,int p_line_width=1)=0;
	virtual void draw_circle(const Point p_center, int p_radius,const Color& p_color,bool p_fill=true)=0;
	virtual void draw_arrow(const Point& p_from,const Size& p_size,Direction p_dir,const Color& p_color,bool p_fill=true)=0;
		
	virtual void draw_pixmap(const Pixmap& p_pixmap, const Point& p_pos,const Rect& p_source)=0;	
	virtual void draw_string(const Font& p_font,const Point& p_pos,const Color& p_color,const String& p_string,Direction p_dir=RIGHT)=0;

	virtual void draw_stylebox( const StyleBox& p_style,const Point& p_from,const Size& p_size)=0;
	
	virtual void draw_set_clipping(bool p_enabled,const Rect& p_rect=Rect())=0;
	virtual void add_dirty_region(const Rect& p_rect)=0;
	virtual void request_size_update()=0; ///< Window will call this only once util answered with a "do_size_update" signal.

	/* signals:
		-mouse_button
			pos:Point
			button:int
			pressed:bool
			mask:int
		
		-mouse_doubleclick
			pos:Point
			mask:int
		-mouse_motion
			pos:Point
			pos_rel:Point
			mask:int
		-mouse_enter_window
		-mouse_exit_window
		-key
			unicode:int
			scancode:int
			pressed:int
			echo:bool
			mask:int
		-update
			area:Rect
		-rect_change
			rect:Rect
		-do_size_update
		-focus_enter
		-focus_exit
		-close_request			
	*/
	virtual ~PlatformWindow();
};

class PlatformPixmap : public PlatformBase {
protected:
	PlatformPixmap(Platform *p_owner) : PlatformBase(p_owner) {};
public:
	virtual Error create(const Size& p_size,PixmapFormat p_format=PIXMAP_FORMAT_RGB)=0;

	virtual Error load_file(const String& p_file)=0;
	virtual Error save_file(const String& p_file)=0;
	
	virtual void load_pixels( const void * p_pixels, const Size& p_size, PixmapFormat p_format )=0;
	virtual void save_pixels( void * p_pixels ) const=0;

	virtual void copy_rect_to( const Rect& p_src_rect,PlatformPixmap * p_to_pixmap,const Point& p_dst_pos ) const=0;

	virtual Size get_size() const=0;
	virtual PixmapFormat get_format() const=0;
	virtual bool is_valid() const=0;

	virtual PlatformPixmap *copy()=0;

	virtual ~PlatformPixmap();
};


class PlatformFont : public PlatformBase {
protected:
	PlatformFont(Platform *p_owner) : PlatformBase(p_owner) {};
public:
	virtual Error load(const String& p_name,float p_size,unsigned int p_font_style_flags=0)=0;
	
	virtual String get_name() const=0;
	virtual float get_size() const=0;
	virtual unsigned int get_flags() const=0;
	virtual bool has_character(unsigned int p_code) const=0;
	
	virtual int get_height() const=0;
	virtual int get_ascent() const=0;
	virtual int get_descent() const=0;
	virtual int get_string_width(const String& p_string) const=0;	

	virtual bool is_valid() const=0;

	virtual PlatformFont *copy() const=0;

	virtual ~PlatformFont();
};


class PlatformPointer : public PlatformBase {
protected:
	PlatformPointer(Platform *p_owner) : PlatformBase(p_owner) {};
public:
	
	virtual void warp(const Point& p_pos)=0;
	virtual Point get_pos() const=0;

	virtual void set_shape(const Pixmap& p_pixmap,const Point& p_hotspot=Point())=0;
	virtual void set_default_shape(const Pixmap& p_pixmap,const Point& p_hotspot=Point())=0;
};


class PlatformFile : public PlatformBase { 
protected:
	PlatformFile(Platform *p_owner) : PlatformBase(p_owner) {};
public:

	virtual Error open(String p_filename, unsigned int p_flags)=0;

	virtual unsigned int get_flags() const=0;
	virtual	String get_file() const=0;

	virtual void close()=0;
	virtual bool is_open()=0;

	virtual void seek(unsigned int p_position)=0;
	virtual void seek_end(signed int p_position=0)=0;
	virtual unsigned int get_pos()=0;
	virtual unsigned int get_size()=0;

	virtual bool eof_reached() const=0;

	virtual unsigned char get_8() const=0;
	virtual unsigned short get_16() const;
	virtual unsigned int get_32() const;
	virtual gui_u64 get_64() const;

	virtual void get_buffer(unsigned char *p_dst,int p_bytes) const;

	virtual void store_8(unsigned char p_dest)=0;
	virtual void store_16(unsigned short p_dest);
	virtual void store_32(unsigned int p_dest);
	virtual void store_64(gui_u64 p_dest);

	virtual void store_buffer(const unsigned char *p_src,int p_bytes);

	virtual Error last_get_error() const=0;

	virtual ~PlatformFile();
};

class PlatformFileSystem : public PlatformBase {
protected:
	PlatformFileSystem(Platform *p_owner) : PlatformBase(p_owner) {};
public:

	virtual Error list_dir_begin()=0;
	virtual String get_next(bool* p_is_dir=0)=0;
	virtual void list_dir_end()=0; 
	
	virtual int get_drive_count() const=0;
	virtual String get_drive(int p_drive) const=0;
	
	virtual bool change_dir(String p_dir)=0; ///< can be relative or absolute, return false on success
	virtual String get_current_dir() const=0; ///< return current dir location
	
	virtual bool make_dir(String p_dir)=0; ///< Make a directory

	virtual bool file_exists(String p_file) const=0; ///< true wether a (local/global) file exists

	virtual String get_absolute_path(String p_path) const;
	
	virtual bool rename(String p_path,String p_new_path)=0; ///< false on succes
	virtual bool remove(String p_path)=0; ///< false on success
	
	virtual String get_dir_separator() const=0;

	virtual ~PlatformFileSystem();
};


class PlatformTimer : public PlatformBase {
protected:
	PlatformTimer(Platform *p_owner) : PlatformBase(p_owner) {};
public:

	virtual void start(unsigned int p_interval_ms,bool p_single_shot=true)=0;
	virtual void stop()=0;

	virtual bool is_active() const=0;
	virtual unsigned int get_interval() const=0;
	
	/* 
		signals:
			-timeout
	*/

	virtual ~PlatformTimer();
};

class Platform {
protected:

	/* This is the only way to extract the platform pixmap and font from a Pixmap or Font.
	   it is done to ensure that no one uses these directly, even by accident. Platforms
	   can use to extract pixmaps/font of their own type safely */

	inline PlatformPixmap *extract_platform_pixmap(const Pixmap& p_pixmap) {

		PlatformPixmap *pp=p_pixmap.get_platform_pixmap();
		if (!pp || pp->get_owner()!=this) // make sure we OWN this
			return NULL;

		return pp;
	}

	inline PlatformFont *extract_platform_font(const Font& p_font) {

		PlatformFont *pf=p_font.get_platform_font();
		if (!pf || pf->get_owner()!=this) // make sure we OWN this
			return NULL;

		return pf;
	}


	static Platform *singleton;
public:

	
	virtual PlatformTheme *get_theme() const=0;
	virtual PlatformPointer *get_pointer() const=0;

	virtual PlatformWindow* create_window(PlatformWindow* p_parent=NULL)=0;
	virtual PlatformPixmap* create_pixmap()=0;
	virtual PlatformFont* create_font()=0;
	virtual PlatformFile *create_file()=0;
	virtual PlatformFileSystem *create_file_system()=0;
	virtual PlatformTimer *create_timer()=0;
	

	virtual int run()=0;

	static Platform *get_singleton();

	Platform();	
	virtual ~Platform();

};

}

#endif
