/* Configure decNumber for either host or target.
   Copyright (C) 2008 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   In addition to the permissions in the GNU General Public License,
   the Free Software Foundation gives you unlimited permission to link
   the compiled version of this file into combinations with other
   programs, and to distribute those combinations without any
   restriction coming from the use of this file.  (The General Public
   License restrictions do apply in other respects; for example, they
   cover modification of the file, and distribution when not linked
   into a combine executable.)

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, see
   <https://www.gnu.org/licenses/>.  */


#if HOST_BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#else
#define WORDS_BIGENDIAN 0
#endif

#ifndef DECDPUN
#define DECDPUN 3
#endif
