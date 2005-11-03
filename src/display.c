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
 * 	Provide high level functionality required for manipulating
 * 	the display and related aspects. The API should be
 * 	platform independent for porting to other platforms
 * 	later
 */

#include<stdlib.h>
#include <string.h>
#include "display.h"
#include "../config.h"

#ifdef HAVE_X_TEST
	#include<X11/extensions/XTest.h>
#endif /* HAVE_X_TEST */


/*
 * Purpose:
 *	Setup the video display
 *
 * Parameters:
 * 	NONE
 *
 * Returns:
 * 	GDISPLAY structure with display, screen, width and height
 * 	complete
 *
 * History:
 *	21/02/2001 (Mike Bennett): Created
*/
GDISPLAY *createDisplay() {
	GDISPLAY *video;

	/* Create a display structure and set to defaults */
	video = (GDISPLAY *)malloc(sizeof(GDISPLAY));
	video->display = UNSETCHAR;
	video->dwidth = UNSETUINT;
	video->dheight = UNSETUINT;
	video->screen = UNSETINT;
	video->window = UNSETINT;
	video->gcontext = UNSETCHAR;
	video->font = UNSETCHAR;

	/* Open the display */
	if(!(video->display = XOpenDisplay(XDisplayName(NULL))))
		return NULL;

	video->screen = DefaultScreen(video->display);

	getDisplayInfo(video, GDISPLAY_WIDTH);
	getDisplayInfo(video, GDISPLAY_HEIGHT);

	return video;
}


/*
 * Purpose:
 *	Setup the video display and windows required
 *
 * Parameters:
 * 	1st - WPRETTY structure with display style info
 *
 * Returns:
 * 	GDISPLAY * to setup display
 *	NULL on error
 *
 * History:
 *	12/03/2001 (Mike Bennett): Rewrote to setup the input window
 *				   correctly and handle managed windows
 *	18/03/2001 (Mike Bennett): Ammended to set window properties and
 *				   receive the close window client message
 *	26/03/2001 (Mike Bennett): Put a 1 pixel black border around a
 *				   non-managed always on top input window
*/
GDISPLAY *setupDrawable(WPRETTY *style, int *window, int argc, char **argv) {
	GDISPLAY *video;
	XSetWindowAttributes winAttrib;
	XSizeHints sizeHints;
	unsigned long mask, col_border, col_background;
	unsigned int width_border;

	if(!(video = createDisplay()))
		return NULL;

	if(!video->display || video->screen == UNSETINT)
		return NULL;

	/* Default input window look - may be overrode below */
	width_border = 0;
	col_border = WhitePixel(video->display, video->screen);
	col_background = WhitePixel(video->display, video->screen);

	mask = CWSaveUnder;
	winAttrib.save_under = True;	/* Let the X server take care of whats
					   underneath this window, rather than
					   the client applications, it'll also
					   prevent needless Expose events.
					   NOTE: Not all X servers support this */

	/* Don't let the window manager try to set borders or title bars, etc */
	if(style->managed[0] == 'n' || style->display[0] == 'y') {
		mask |= CWOverrideRedirect;
		winAttrib.override_redirect = True;
	}

	/* Setup an input window to have a 1 pixel black border */
	if(style->display[0] == 'i') {
		width_border = 1;
		col_border = BlackPixel(video->display, video->screen);
	}

	/* How should the window shape and position be setup? */
	if(style->display[0] == 'y') {
		window[WI_X] = 0;
		window[WI_Y] = 0;
		window[WI_WIDTH] = video->dwidth;
		window[WI_HEIGHT] = video->dheight;
	}

	video->window = XCreateSimpleWindow(video->display,
		RootWindow(video->display, video->screen),
	  	window[WI_X], window[WI_Y], window[WI_WIDTH],
	  	window[WI_HEIGHT], width_border,
	  	col_border, col_background);

	video->gcontext = XCreateGC(video->display, video->window, 0, NULL);
	
	if(style->display[0] == 'y') {
		mask |= CWBackPixmap;	
		winAttrib.background_pixmap = None;	/* Don't set a background, i.e. 
							   transparent window */
		XSetFunction(video->display, video->gcontext, GXinvert);
	}

	XChangeWindowAttributes(video->display, video->window,
		mask, &winAttrib);

//	XSetForeground(video->display, video->gcontext,
//			BlackPixel(video->display, video->screen));

	XSetLineAttributes(video->display, video->gcontext, style->size, LineSolid,
			CapButt, JoinMiter);

	sizeHints.x = window[WI_X];
	sizeHints.y = window[WI_Y];
	sizeHints.width = window[WI_WIDTH];
	sizeHints.height = window[WI_HEIGHT];
	sizeHints.flags = PPosition | PSize;

	XSetStandardProperties(video->display, video->window, PACKAGE, PACKAGE,
				(Pixmap)NULL, argv, argc, &sizeHints);

	/* Receive the close window client message */
	if(style->display[0] == 'i') {
		video->wmdelete = XInternAtom(video->display, "WM_DELETE_WINDOW",
				False);
		XSetWMProtocols(video->display, video->window, &video->wmdelete,
				True);
	}

	return video;
}


/*
 * Purpose:
 *	Setup the video display and windows required
 *
 * Parameters:
 * 	1st - video structure
 *
 * Returns:
 * 	NONE
 *
 * History:
 *	10/05/2000 (Mike Bennett): Rewrote
*/
void closeDisplay(GDISPLAY *video) {

	XCloseDisplay(video->display);
}


/*
 * Purpose:
 *	Get display specific information. NOTE the display must
 *	be opened and screen must be set
 *
 * Parameters:
 * 	1st - video structure to fill in with data required
 * 	2nd - type of data to get
 *
 * Returns:
 * 	GOOD on success
 * 	BAD on failure
 * 	
 * History:
 *	13/05/2000 (Mike Bennett): Created
*/
int getDisplayInfo(GDISPLAY *video, int itype) {

	if(!video->display || video->screen == UNSETINT)
		return BAD;

	switch(itype) {
		case GDISPLAY_HEIGHT:
			video->dwidth = DisplayWidth(video->display, video->screen);
			video->dwidth -= (2 * GDEF_BORDER);
		break;

		case GDISPLAY_WIDTH:
			video->dheight = DisplayHeight(video->display, video->screen);
			video->dheight -= (2 * GDEF_BORDER);
		break;

		default:
			return BAD;
		break;
	}

	return GOOD;
}


/*
 * Purpose:
 * 	Handle key window actions on a display
 *
 * Parameters:
 * 	1st - filled in video structure 
 *	2nd - action to perform
 *
 * Returns:
 * 	GOOD on success
 * 	BAD on failure
 * 	
 * History:
 *	13/05/2000 (Mike Bennett): Created
 *	21/02/2001 (Mike Bennett): Renamed and rewrote
 *	12/03/2001 (Mike Bennett): Add CLEAR
 *
*/
int handleDrawableWindow(GDISPLAY *video, int action) {

	if(!video->display || !video->window)
		return BAD;

	switch(action) {
		case MAP:
			XMapRaised(video->display, video->window);
			XFlush(video->display);
		break;

		case UNMAP:
			XUnmapWindow(video->display, video->window);
			XFlush(video->display);
		break;

		case CLEAR:
			XClearWindow(video->display, video->window);
		break;

		case DESTROY:
			if(video->font)
				XFreeFont(video->display, video->font);

			if(video->display)
				XDestroyWindow(video->display, video->window);

			if(video->gcontext)
				XFreeGC(video->display, video->gcontext);

			if(video->display != UNSETCHAR)
				XCloseDisplay(video->display);

			free(video);
		break;
	}

	return GOOD;
}



/*
 * Purpose:
 *	Setup the video display and windows required
 *
 * Parameters:
 * 	1st - general wayv data configuration
 * 	2nd - video display setup
 *
 * Returns:
 * 	GOOD on success
 * 	BAD on failure
 *
 * History:
 *	04/03/2001 (Mike Bennett): Updated for input box
 *	22/03/2001 (Mike Bennett): Moved into display.c and renamed
 *	16/04/2001 (Mike Bennett): Added else check for input windows
*/
int setupPointer(WSETUP *wayv, GDISPLAY *video, int pbutton) {
	Window win;

//	XAllowEvents(video->display, AsyncBoth, CurrentTime);

	if(wayv->pretty->display[0] != 'i')
		win = DefaultRootWindow(video->display);
	else
		win = video->window;

	XGrabButton(video->display, pbutton,
		0, win, False,
		ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeAsync, None, None);

	/* We don't want Caps Lock interfering */
	XGrabButton(video->display, pbutton,
		0 | KMODLOCK, win, False,
		ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeAsync, None, None);

	return GOOD;
}


/*
 * Purpose:
 *      Write text on the display
 *
 * Parameters:
 *      1st - video structure
 *      2nd - text to display
 *
 * Returns:
 *      Pointer to a GDISPLAY structure with text output window
 *      NULL on failure
 *  
 * History:
 *      13/02/2001 (Mike Bennett): Created
 *		21/02/2001 (Mike Bennett): Added create output window
 *		01/03/2001 (Mike Bennett): Got output window working
 *					   correctly
*/
GDISPLAY *writeText(char *text) {
	GDISPLAY *video;
	XSetWindowAttributes winAttrib;
	int twidth, theight;

	video = createDisplay();

	if((video->font = XLoadQueryFont(video->display, "9x15")) == NULL) {
		printf("Error: Couldn't load font\n");
		return NULL;
	}

	twidth = XTextWidth(video->font, text, strlen(text)) + 6;
	theight = video->font->ascent + video->font->descent + 7;

	video->window = XCreateSimpleWindow(video->display,
			RootWindow(video->display, video->screen),
			(video->dwidth - twidth) >> 1, (video->dheight - theight) >> 1,
			twidth, theight, 1,
			WhitePixel(video->display, video->screen),
			BlackPixel(video->display, video->screen));

	winAttrib.override_redirect = True;
	winAttrib.save_under = True;

	XChangeWindowAttributes(video->display, video->window,
			CWOverrideRedirect | CWSaveUnder, &winAttrib);

	video->gcontext = XCreateGC(video->display, video->window, 0, NULL);
	XSetForeground(video->display, video->gcontext,
			WhitePixel(video->display, video->screen));
	XSetFont(video->display, video->gcontext, video->font->fid);

	handleDrawableWindow(video, MAP);

	XDrawImageString(video->display, video->window, video->gcontext,
		3, video->font->ascent + 3, text, strlen(text));

	XFlush(video->display);

	return video; 
}


/*
 * Purpose:
 * 	Write a pixel onto a window
 *
 * Parameters:
 * 	1st - video structure to fill in with data required
 * 	2nd - x position to draw the pixel at
 * 	3rd - y position to draw the pixel at
 *
 * Returns:
 * 	GOOD on success
 * 	BAD on failure
 * 	
 * History:
 *	13/05/2000 (Mike Bennett): Created
*/
int writePixels(GDISPLAY *video, int x1, int y1, int x2, int y2) {

	if(!video->display || !video->window)
		return BAD;

/*
	x1 -= GDEF_BORDER;
	y1 -= GDEF_BORDER;
	x2 -= GDEF_BORDER;
	y2 -= GDEF_BORDER;
*/
	XDrawLine(video->display, video->window, video->gcontext, x1, y1, x2, y2);

	XFlush(video->display);

	return GOOD;
}
