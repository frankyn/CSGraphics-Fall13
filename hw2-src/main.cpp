#include <cmath>
#include <unistd.h>
#include "../include/Angel.h"

int NumberOfLines = 10;
int ArraySize;

void init ( ) {
    int i = 0;
    float p, a, p2, a2;
    float inc = 360.0f / NumberOfLines;
    //SUPER DIRTY
    for ( p = -180.0f ; p <= 180.0f ; p += inc ) {
        for ( a = -180.0f; a <= 180.0f; a += inc ) {
            i++;
        }
    }

    ArraySize = i*2;
    vec3 * sphere = new vec3 [ ArraySize ];  //  Need to points for the poles
    
    i = 0;

    //Compute Latitude and Longitude lines.

    for ( p = -180.0f ; p <= 180.0f ; p += inc ) {
        p2 = p * DegreesToRadians;
        for ( a = -180.0f; a <= 180.0f; a += inc ) {
            a2 = a * DegreesToRadians;
            sphere [ i++ ] = 0.5f * vec3 ( sin ( a2 ) * cos ( p2 ) ,
                                           sin ( p2 ) ,
                                           cos ( a2 ) * cos ( p2 ) );
        }
    }

    for ( a = -180.0f ; a <= 180.0f ; a += inc )  {
        a2 = a * DegreesToRadians;
        for ( p = -180.0f; p <= 180.0f; p += inc ) {
            p2 = p * DegreesToRadians;
            sphere [ i++ ] =  0.5f * vec3 ( sin ( a2 ) * cos ( p2 ) ,
                                            sin ( p2 ) ,
                                            cos ( a2 ) * cos ( p2 ) );
        }
    }

    
    GLuint vao;
    _glGenVertexArrays ( 1 , &vao );
    _glBindVertexArray ( vao );

    GLuint buffer;
    glGenBuffers ( 1 , &buffer );
    glBindBuffer ( GL_ARRAY_BUFFER , buffer );
    glBufferData ( GL_ARRAY_BUFFER , sizeof ( vec3 ) * ArraySize ,
                  sphere , GL_STATIC_DRAW );

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    GLuint program = InitShader ( "vshader.glsl" , "fshader.glsl" );

    glUseProgram ( program );

    GLint location = glGetAttribLocation ( program , "vPosition" );
    glEnableVertexAttribArray ( location );
    glVertexAttribPointer ( location , 3 , GL_FLOAT , GL_FALSE ,
                          0 , BUFFER_OFFSET ( 0 ) );

    glEnable ( GL_DEPTH_TEST );

    glClearColor ( 0.0f , 0.0f , 0.0f , 1.0f );
    
    delete sphere;
}

void display ( ) {
     glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
     glDrawArrays ( GL_LINE_STRIP, 0 , ArraySize  );
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
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize ( 512 , 512 );

    glutCreateWindow ( "Sphere for Days" );

#ifndef __APPLE__
    glewInit ( );
#endif
    int nTemp, c;

    while ((c = getopt (argc, argv, "n:")) != -1) {
        switch ( c ) {
            case 'n':
            {
                nTemp = atoi(optarg);
                if ( nTemp > 0 ) {
                    NumberOfLines = nTemp;
                }
            }
            break;
        }
    }
    init ( );

    glutDisplayFunc ( display );
    glutKeyboardFunc ( keyboard );

    glutMainLoop ( );
    
    return 0;
}


