#include "GridLine.h"

#ifndef GRID_MAP_HEADER
#define GRID_MAP_HEADER

class GridMap {
	public:
		GridMap ( int );
		~GridMap ( ); // Destructor

		void onClick ( double , double );
		const vec2 * getData ( ); 

		int getPoints ( );
	private:	
		int totalLines, totalPoints, dimension;
		GridLine * lines;
		vec2 * rawData;


};

#endif