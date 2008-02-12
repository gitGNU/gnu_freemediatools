/*
Copyright (c) 2003 Daniel W. Howard

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without
limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef BXMLNODE_H
#define BXMLNODE_H

#include <stdio.h>

namespace bxmlnode
{

/* basicxmlnode: simple xml node memory representation */
struct basicxmlnode
{
  char * tag; /* always non-NULL */
  char * text; /* body + all whitespace, always non-NULL */
  char * * attrs; /* array of strings, NULL marks end */
  char * * values; /* array of strings, NULL marks end */
  struct basicxmlnode * * children; /* similar */
  int * childreni; /* children positions in text */
};

/* readbasicxmlnode: reads simple XML file */
struct basicxmlnode * readbasicxmlnode( FILE * fpi ) ;

/* deletebasicxmlnode: frees all memory for xml tree */
void deletebasicxmlnode( struct basicxmlnode * node ) ;

/* printbasicxmlnodetagnames: visits each node once */
void printbasicxmlnodetagnames( struct basicxmlnode * node ) ;

/* printbasicxmlnode: prints to console */
void printbasicxmlnode( struct basicxmlnode * node ) ;

} //  namespace bxmlnode

#endif // BXMLNODE_H
