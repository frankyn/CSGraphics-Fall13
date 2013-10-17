#include "GridLine.h"
#include <cstdio>
//Declare segment by two points, and initial visible state.
GridLine::GridLine ( vec2 p1 , vec2 p2 , int show ) { 
	segmentPoints[0] = p1;
	segmentPoints[1] = p2;
	visible = show;	
	next = NULL;
}

//Declare segment by two points, and set default visibility to true.
GridLine::GridLine ( vec2 p1 , vec2 p2 ) {
	segmentPoints[0] = p1;
	segmentPoints[1] = p2;
	visible = 1;	
	next = NULL;
}

//Default constructor
GridLine::GridLine ( ) {
	segmentPoints[0] = vec2 ( 0 , 0 );
	segmentPoints[1] = vec2 ( 1 , 0 );
	visible = 1;	
	next = NULL;
}

void GridLine::points ( vec2 p1 , vec2 p2 ) {
	segmentPoints[0] = p1;
	segmentPoints[1] = p2;
}

GridLine GridLine::operator+ ( GridLine &r ) {
	next = &r;
	return *this;
}

//onClick hide/show line
void GridLine::onClick ( double x , double y ) {
	if ( x >= segmentPoints[0].x-.02f && y >= segmentPoints[0].y-.02f && 
		 x <= segmentPoints[1].x+.02f && y <= segmentPoints[1].y+.02f ) {
			visible = visible?0:1;
	}

	if ( next ) {
		next->onClick ( x, y );
	}
}

//append points to a buffer 
int GridLine::getData ( vec2 * buffer , int offset ) {
	if ( visible ) {
		buffer[offset++] = segmentPoints[0];
		buffer[offset++] = segmentPoints[1];
	}

	if ( next ) {
		offset += next->getData ( buffer , offset );
	}

	return offset;
}