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
 *	Backend for wayV
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "display.h"
#include "process.h"
#include "../config.h"

int main(int argc, char **argv) {
  GDISPLAY *video;
  XEvent event;
  WSETUP *wayv;

  wayv = (WSETUP *)malloc(sizeof(WSETUP));

  wayv->pretty = (WPRETTY *)malloc(sizeof(WPRETTY));
  wayv->pretty->display = "yes";
  wayv->pretty->managed = "no";
  wayv->pretty->color = "ff0056";
  wayv->pretty->size = 5; /* Width of the line */
  wayv->pretty->feedback = "yes";
  wayv->pretty->wait = DEF_PRETTY_WAIT;

  /* Setup the display */
  if(!(video = setupDrawable(wayv->pretty, wayv->pretty->window, argc, argv))) {
    printf("Cannot connected to X server\n");
    return BAD;
  }

  /* Setup the mouse/pointer as an input device */
  setupPointer(wayv, video, 1);

  /* Setup to listen to the escape key */
  KeyCode esckc = XKeysymToKeycode( video->display, XK_Escape );
  /* KeyCode esckc = AnyKey; doesn't work with X_GrabKey anymore, see man page. */ 

  Window win;
  if(wayv->pretty->display[0] != 'i')
    win = DefaultRootWindow(video->display);
  else
    win = video->window;

  XGrabKey( video->display, esckc, 0, win, True, 0, 0 );

  handleDrawableWindow(video, MAP);

  setupSignals();

  int lastx = 0, lasty = 0;
   
  /* Process the events */
  while(1) {
    XNextEvent(video->display, &event);

    switch(event.type) {
    case KeyPress:
      closeDisplay(video);
      exit(GOOD);
      break;

    case MotionNotify:
					
      if(wayv->pretty->display[0] == 'y')

        writePixels(video, (int)lastx - GDEF_BORDER,
                    lasty - GDEF_BORDER,
                    (int)event.xbutton.x - GDEF_BORDER,
                    (int)event.xbutton.y - GDEF_BORDER);

      else if(wayv->pretty->display[0] == 'i')
        writePixels(video, (int)lastx,
                    (int)lasty,
                    (int)event.xbutton.x,
                    (int)event.xbutton.y);

      lastx = event.xbutton.x;
      lasty = event.xbutton.y;

      break;

    case ButtonPress:
      if(wayv->pretty->display[0] == 'y')
        handleDrawableWindow(video, MAP);

      lastx = event.xbutton.x;
      lasty = event.xbutton.y;

      break;

    case ButtonRelease:
      break;

    case ClientMessage:
      if(event.xclient.data.l[0] == video->wmdelete) {
        closeDisplay(video);
        exit(GOOD);
      }
      break;
    }
  }
}
