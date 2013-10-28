//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "../include/Angel.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices*2];
color4 colors[NumVertices*2];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices_cube_1[8] = {
    point4( 1.0, 3.0, 1.0, 1.0 ),
    point4( 1.0, 4.0, 1.0, 1.0 ),
    point4( 2.0, 4.0, 1.0, 1.0 ),
    point4( 2.0, 3.0, 1.0, 1.0 ),
    point4( 1.0, 3.0, 2.0, 1.0 ),
    point4( 1.0, 4.0, 2.0, 1.0 ),
    point4( 2.0, 4.0, 2.0, 1.0 ),
    point4( 2.0, 3.0, 2.0, 1.0 )
};

point4 vertices_cube_2[8] = {
    point4( 1.0, 3.0, 1.0, 1.0 ),
    point4( 1.0, 4.0, 1.0, 1.0 ),
    point4( 2.0, 4.0, 1.0, 1.0 ),
    point4( 2.0, 3.0, 1.0, 1.0 ),
    point4( 1.0, 3.0, 2.0, 1.0 ),
    point4( 1.0, 4.0, 2.0, 1.0 ),
    point4( 2.0, 4.0, 2.0, 1.0 ),
    point4( 2.0, 3.0, 2.0, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta = 45.0f;
GLfloat ThetaX = 36.86;
GLfloat ThetaY = 18.43494;

GLfloat  dx =  1.5f;
GLfloat  dy =  3.5f;
GLfloat  dz =  1.5f;
GLfloat fdx = -3.0f;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
cube1( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices_cube_1[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices_cube_1[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices_cube_1[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices_cube_1[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices_cube_1[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices_cube_1[d]; Index++;
}

void
cube2( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices_cube_2[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices_cube_2[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices_cube_2[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices_cube_2[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices_cube_2[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices_cube_2[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    cube1( 1, 0, 3, 2 );
    cube1( 2, 3, 7, 6 );
    cube1( 3, 0, 4, 7 );
    cube1( 6, 5, 1, 2 );
    cube1( 4, 5, 6, 7 );
    cube1( 5, 4, 0, 1 );

    
    cube2( 1, 0, 3, 2 );
    cube2( 2, 3, 7, 6 );
    cube2( 3, 0, 4, 7 );
    cube2( 6, 5, 1, 2 );
    cube2( 4, 5, 6, 7 );
    cube2( 5, 4, 0, 1 );

}

// OpenGL initialization
void
init()
{
    colorcube();
 
    // Create a vertex array object
    GLuint vao[1];
    _glGenVertexArrays ( 1 , vao );
    _glBindVertexArray ( vao[0] );


    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );
    
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );
    
    
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
   
}



//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    mat4  transform = ( Translate( -dx, -dy, -dz ) *
      RotateX( -ThetaX ) *
      RotateY( -ThetaY ) *
      RotateZ( Theta ) *
      RotateY( ThetaY ) *
      RotateX( ThetaX ) *
      Translate( dx, dy, dz ) );

    transform = transform * Translate( -3 , 0 , 0 ) ;
    point4  transformed_points[NumVertices];

    for ( int i = 0; i < NumVertices; ++i ) {
        transformed_points[i] = Scale( .15 , .15 , .15 )*transform*points[i];
    }
    
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
         transformed_points );

    for ( int i = 0; i < NumVertices; ++i ) {
        transformed_points[i] = Scale( .15 , .15 , .15 )*points[i+NumVertices];
    }

    
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(transformed_points) , sizeof(transformed_points),
         transformed_points );

    glDrawArrays( GL_TRIANGLES, 0, NumVertices*2 );
    
    glutSwapBuffers();

}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

struct Obj{

};


void readObjFile ( const char * filename ) {
    FILE * fp = fopen ( filename , "r" );

    char buffer[512];
    long vertices = 0;
    long fsss = 0;
    while ( fgets ( buffer , sizeof(buffer) , fp ) ) {
        if ( strstr ( buffer , "v" ) ) {
            vertices ++;
        }
        if ( strstr ( buffer , "f" ) ) {
            fsss ++;
        }
    }

    printf ( "Vertices: %ld \n" , vertices );
    printf ( "Fsss??: %ld \n" , fsss );

}

int
main( int argc, char **argv )
{

    /*
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitWindowPosition(0,0);
    glutCreateWindow( "Color Cube" );

#ifndef __APPLE__
    glewInit ( );
#endif
    char c;
    int temp;
    while ((c = getopt (argc, argv, "t:")) != -1) {
        switch ( c ) {
            case 't':
            {
                temp = atoi(optarg);
                if ( temp > 0 ) {
                    Theta = temp;
                }
            }
            break;
        }
    }


    init();
    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMainLoop();
    */

    return 0;
}
