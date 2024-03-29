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
 *	Perform actions and manage signals
*/

#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "defines.h"
#include "process.h"

extern int child;

/*
 * Purpose:
 *	Setup signal handlers for wayV - only manage SIGCHLD at this
 *	stage.
 *
 * Parameters:
 * 	NONE
 *
 * Returns:
 * 	NONE
 *
 * History:
 *	17/04/2001 (Mike Bennett): Created function
*/
void setupSignals() {
	signal(SIGCHLD, destroyZombies);
}


/*
 * Purpose:
 *	Signal handler when a child process dies, basically don't leave
 *	it around as a zombie process. *hehe* funky function name - its a bit
 *	sad that there's more lines of comments than code :)
 *
 * Parameters:
 * 	1st - Signal id
 *
 * Returns:
 * 	NONE
 *
 * History:
 *	13/06/2000 (Mike Bennett): Created function
*/
void destroyZombies(int sigNum) {
	wait(NULL);
}
