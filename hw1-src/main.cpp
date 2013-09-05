#include "../include/Angel.h"

void init ( ) {
	/*vec2 points[NumPoints];

    // Specifiy the vertices for a triangle
    vec2 vertices[3] = {
        vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = vec2( 0.25, 0.50 );

    // compute and store N-1 new points
    for ( int i = 1; i < NumPoints ; ++i ) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
    }
	*/
    /*
	APPLE
    */
    // Create a vertex array object
    //GLuint vao[1];
    //glGenVertexArraysAPPLE( 1 , vao );
    //glBindVertexArrayAPPLE( vao[0] );

    /*
	Everything else
     */
    
    glClearColor ( 1.0 , 1.0 , 1.0 , 1.0 ); // white background
}

void display ( ) {
    glClear ( GL_COLOR_BUFFER_BIT );     // clear the window
    //glDrawArrays ( GL_POINTS , 0 , NumPoints );    // draw the points
    glFlush ( );

}

void keyboard ( unsigned char key , int x , int y ) {
	switch ( key ) {
		case 033:
			exit ( EXIT_SUCCESS );
		break;
	}
}

int main ( int argc , char ** argv ) {
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA );
    glutInitWindowSize ( 512 , 512 );

    glutCreateWindow ( "Sierpinski Gasket" );

    init ( );

    glutDisplayFunc ( display );
    glutKeyboardFunc ( keyboard );

    glutMainLoop ( );
    
    return 0;
}


