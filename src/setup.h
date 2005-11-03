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
 *	Handle the setup and configuration
*/

#ifndef __SETUP_H__
#define __SETUP_H__

#include <X11/Xlib.h>

/* Type of sub-actions that are possible, used in WA_PROCESS struct */
#define WA_EXEC		1		/* Execute the program listed in tparam */
#define WA_WARP		2		/* Move the mouse back to the start position */
#define WA_DELAY	3		/* Delay nparam seconds */
#define WA_KILL		4		/* Kill last wayV started program */
#define WA_SENDKEY	5		/* Send key press */

/* Different type of include files */
#define WF_KEYMAP	"Keymap"

/* Parameters for setup.c:getKeyText(), SKT = Strip Key Text */
#define SKT_NONE			0
#define SKT_QUOTES			1

/* Defines for accessing window box information */
#define	WI_X		0
#define WI_Y		1
#define WI_WIDTH	2
#define WI_HEIGHT	3

/* Defaults */
#define DEF_UNI_XGRID		8
#define DEF_UNI_YGRID		8
#define DEF_UNI_MAXGPOINTS	2000
#define DEF_UNI_MAXVECTORS	10
#define DEF_PRETTY_SIZE		2
#define DEF_PRETTY_WAIT		250

typedef struct {
	char *display;
	char *managed;
	char *color;
	int size;
	char *feedback;
	int wait;

	int window[4];
} WPRETTY;

typedef struct {
	WPRETTY *pretty;
} WSETUP;

#endif /* __SETUP_H__ */
