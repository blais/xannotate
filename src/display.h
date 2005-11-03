/* wayV - NGUI (Next Generation User Interfaces)
 *
 * Copyright (C) 2000-2003 Mike Bennett (smoog at stressbunny dot com)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * 
 * Purpose:
 * 	Prototypes, defines and structs for display.c
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "defines.h"
#include "setup.h"

/* Actions that are performable on a window */
#define MAP			1
#define UNMAP			2
#define DESTROY			3
#define CLEAR			4

#define PBUTTONNONE		None

/* Defaults for setting up the standard window */
#define GDEF_XWIN		0
#define GDEF_YWIN		0
#define GDEF_BORDER		5

#define KMODNONE		None
#define KMODSHIFT		ShiftMask
#define KMODLOCK		LockMask
#define KMODCONTROL		ControlMask
#define KMOD1			Mod1Mask
#define KMOD2			Mod2Mask
#define KMOD3			Mod3Mask
#define KMOD4			Mod4Mask
#define KMOD5			Mod5Mask

/* Type of key press to send */
#define PRESS			True
#define RELEASE			False

/* For use by getDisplayInfo on the type of info to get */
#define GDISPLAY_HEIGHT		1
#define GDISPLAY_WIDTH		2

typedef struct {
	Display *display;
	unsigned int dwidth, dheight;

	int screen;

	Window window;
	GC gcontext;

	XFontStruct *font;
	Atom wmdelete;
} GDISPLAY;


GDISPLAY *createDisplay();
GDISPLAY *setupDrawable(WPRETTY *, int *, int, char **);
void closeDisplay(GDISPLAY *);
int getDisplayInfo(GDISPLAY *, int);
int setupPointer(WSETUP *, GDISPLAY *, int pbutton);
int handleDrawableWindow(GDISPLAY *, int);
GDISPLAY *writeText(char *);
int writePixels(GDISPLAY *, int, int, int, int);
/*int sendKey(WSETUP *, GDISPLAY *, char *, int);*/

#endif /* __DISPLAY_H__ */
