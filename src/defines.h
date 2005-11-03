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
 *	Code wide defines and configuration information
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/*=================DON'T CHANGE BELOW HERE=================*/

#include<stdio.h>

#define GOOD	1
#define BAD		0

#define TRUE	0
#define FALSE	!TRUE

#define UNSETINT	-1		/* Unset Int */
#define UNSETUINT	0		/* Unset Unsigned Int */
#define UNSETCHAR	NULL	/* Unset Char */

#define DIR_NONE	0
#define DIR_N		1
#define DIR_NE		3
#define DIR_E		2
#define DIR_SE		6
#define DIR_S		4
#define DIR_SW		12
#define DIR_W		8
#define DIR_NW		9

#endif /* __DEFINES_H__ */
