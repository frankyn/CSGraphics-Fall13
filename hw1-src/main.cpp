#include <cmath>
#include "../include/Angel.h"

const int NumPoints = 50;

void init ( ) {

	vec2 points[NumPoints];
    int i;
    double rad = 0.0f;
    double r = 0.5f;

    for ( i = 0 ; i < NumPoints ; ++i ) {

        points[i] = vec2 ( r * cos ( rad ) , r * sin ( rad ) );
        rad += 6.28f / NumPoints;
    }

    // Create a vertex array object
    GLuint vao[1];
    _glGenVertexArrays ( 1 , vao );
    _glBindVertexArray ( vao[0] );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl" , "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor ( 1.0 , 1.0 , 1.0 , 1.0 ); // white background
}

void display ( ) {
    glClear ( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays ( GL_LINE_LOOP , 0 , NumPoints );    // draw the points
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


