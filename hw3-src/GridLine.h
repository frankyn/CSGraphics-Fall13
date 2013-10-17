#include "../include/Angel.h"

#ifndef GRID_LINE_HEADER
#define GRID_LINE_HEADER

class GridLine { 
	public:
		GridLine ( vec2 , vec2 , int ); //Declare segment by two points, and initial visible state.
		GridLine ( vec2 , vec2 ); //Declare segment by two points, and set default visibility to true.
		GridLine ( );


		void points ( vec2 , vec2 );
 		
 		//Add GridLine into a Linked List by setting 'next'
 		GridLine operator+ ( GridLine& );

 		//onClick hide/show line
 		void onClick ( double , double );

		//append points to a buffer 
		int getData ( vec2 * , int );

	private:
		int visible;
		vec2 segmentPoints[2];
		GridLine * next;
};

#endif