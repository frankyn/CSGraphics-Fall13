#include "GridMap.h"
#include <cstdio>

GridMap::GridMap ( int N ) {
	//Caculate line count
	dimension = N;
	totalLines = 4*dimension*dimension;
	totalPoints = 2*totalLines;
	
	//Create Lines of N*N
	lines = new GridLine[totalLines];
	rawData = new vec2[totalPoints];
	
	//Build Rows
	int row, column=0, i = 0;
	for ( row = 0, i = 0; row < dimension; row ++ ) {
		for ( column = 0; column < dimension; column ++  ) {	
			lines[i++].points ( 0.2f*vec2( column, row ) - 0.5f, 0.2f*vec2 ( column+1 , row ) - 0.5f );
			lines[i++].points ( 0.2f*vec2( column, row+1 ) - 0.5f , 0.2f*vec2 ( column+1 , row+1 ) - 0.5f );
			lines[i++].points ( 0.2f*vec2( column, row ) - 0.5f , 0.2f*vec2 ( column , row+1 ) - 0.5f );
			lines[i++].points ( 0.2f*vec2( column+1, row ) - 0.5f , 0.2f*vec2 ( column+1 , row+1 ) - 0.5f );
		}
	}
	
	//Link list all lines together
	for ( i = 1; i < totalLines; i ++ ) {
		lines[i-1] = lines[i-1] + lines[i];
	}
}

GridMap::~GridMap ( ) {
	if ( lines ) {
		delete lines;
		delete rawData;
	}
}

void GridMap::onClick ( double x , double y ) {
	lines[0].onClick ( x , y );
}

const vec2 * GridMap::getData (  ) {
	memset(rawData, 0x0, sizeof(vec2)*totalPoints);

	lines[0].getData ( rawData , 0 );
	
	return rawData;
}

int GridMap::getPoints ( ) {
	return totalPoints;
}



