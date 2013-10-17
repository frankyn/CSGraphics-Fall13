#include <cmath>
#include <unistd.h>
#include "../include/Angel.h"
#include "GridMap.h"

using namespace std;

GridMap * grid;

void init ( void ) {
  GLuint vao;
  _glGenVertexArrays ( 1 , &vao );
  _glBindVertexArray ( vao );

  GLuint buffer;
  glGenBuffers  ( 1 , &buffer );
  glBindBuffer  ( GL_ARRAY_BUFFER , buffer );
  printf ( "%d\n" , grid->getPoints ( ) );
  
  const vec2 * data = grid->getData ( );
  int dataSize = sizeof(vec2)*grid->getPoints ( );
  glBufferData( GL_ARRAY_BUFFER, dataSize , data , GL_STATIC_DRAW );
  printf ( "%d\n" , grid->getPoints ( ) );
  GLuint program = InitShader ( "vshader.glsl" , "fshader.glsl" );
  glUseProgram ( program );

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation( program , "vPosition" );
  glEnableVertexAttribArray( loc );
  glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );


  glEnable ( GL_DEPTH_TEST );

  glClearColor ( 0.0 , 0.0 , 0.0 , 1.0 );
}


void display ( ) {
  glClear ( GL_COLOR_BUFFER_BIT );
  glDrawArrays ( GL_LINES , 0 , grid->getPoints ( ) );
  glFlush ( );
}

void keyboard ( unsigned char key , int x , int y ) {
	switch ( key ) {
		case 033:
			exit ( EXIT_SUCCESS );
		break;
	}
}

void mouse ( int button , int state , int x , int y ) {
  if ( state != GLUT_DOWN )
    return;

  int window_width = glutGet(GLUT_WINDOW_WIDTH);
  int window_height = glutGet(GLUT_WINDOW_HEIGHT);

  double x_cord = ( double ) x / ( window_width / 2.0 ) - 1.0;
  double y_cord = ( double ) ( window_height - y ) / ( window_height / 2.0 ) - 1.0;

  grid->onClick ( x_cord , y_cord );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*grid->getPoints ( ), grid->getData ( ), GL_STATIC_DRAW );
  glutPostRedisplay ( );
}

int main ( int argc , char ** argv ) {
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_RGBA );
  glutInitWindowSize ( 512 , 512 );

  glutCreateWindow ( "Maze" );

#ifndef __APPLE__
  glewInit ( );
#endif
  int nTemp=5, c;

  while ((c = getopt (argc, argv, "n:")) != -1) {
      switch ( c ) {
          case 'n':
          {
              nTemp = atoi(optarg);
          }
          break;
      }
  }

  grid = new GridMap ( nTemp );
  init ( );

  glutDisplayFunc ( display );
  glutKeyboardFunc ( keyboard );
  glutMouseFunc ( mouse );

  glutMainLoop ( );
  
  return 0;
}


