#include "../include/Angel.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef struct OBJFile{
    char mtllib[100];
    char mtl_ver[100];
    long vertices_num;
    long faces_num;
    int ** faces;
    point4 * vertices;
} OBJFile; 

void readObjFile ( const char * , OBJFile * );
void renderObj ( OBJFile * , point4 ** );

OBJFile objFile;
point4 * points;

//OpenGL initialization
void init ( ) {
    //Points buffer
    
    //Read in file "teapot.obj" and then render to points buffer
    readObjFile ( "teapot.obj" , &objFile );
    renderObj ( &objFile , &points );

    //Create a vertex array object
    GLuint vao[1];
    _glGenVertexArrays ( 1 , vao );
    _glBindVertexArray ( vao[0] );


    //Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, 3*sizeof(point4)*objFile.faces_num, points, GL_STATIC_DRAW );
    
    //Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    GLint location = glGetAttribLocation ( program , "vPosition" );
    glEnableVertexAttribArray ( location );
    glVertexAttribPointer ( location , 4 , GL_FLOAT , GL_FALSE ,
                          0 , BUFFER_OFFSET ( 0 ) );
    
    glEnable ( GL_DEPTH_TEST );

    glClearColor( 0.0f , 0.0f , 0.0f , 1.0f ); 

}



//----------------------------------------------------------------------------

void display( )
{
    glPolygonMode( GL_FRONT_AND_BACK , GL_LINE ) ;
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    mat4  transform = Scale( .50 , .50 , .50 );
    point4 * transformed_points = (point4*)malloc(sizeof(point4) * 3 * objFile.faces_num );

    for ( int i = 0; i < (3 * objFile.faces_num); ++i ) {
        points[i] = transform*points[i];
    }
    
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * 3 * objFile.faces_num, points );

    glDrawArrays( GL_TRIANGLES, 0, objFile.faces_num * 3 * sizeof(point4) );
    
    glutSwapBuffers();

}

//----------------------------------------------------------------------------

void keyboard( unsigned char key , int x , int y ) {
    switch( key ) {
    	case 033: // Escape Key
    	case 'q': 
        case 'Q':
    	    exit( EXIT_SUCCESS );
    	break;
    }
}


void readObjFile ( const char * filename , OBJFile * objFile ) {
    //Counter
    long i;
    
    //Open handle for filename
    FILE * fp = fopen ( filename , "r" );
    
    //Make sure it was opened correctly.
    if ( !fp ) {
        perror ( "fopen" );
        return;
    }

    char buffer[512];
    
    objFile->vertices_num = 1;
    objFile->faces_num = 0;

    //Count all vertices and faces
    while ( fgets ( buffer , sizeof(buffer) , fp ) ) {
        if ( strstr ( buffer , "v" ) ) {
            objFile->vertices_num ++;
        } else
        if ( strstr ( buffer , "f" ) ) {
            objFile->faces_num ++;
        }
    }

    //Allocate memory for vertices and faces
    //printf ( "Vertices: %ld \n" , objFile->vertices_num );
    objFile->vertices = (point4*)malloc ( sizeof(point4) * objFile->vertices_num );
    if ( !objFile->vertices ) {
        perror ( "malloc" );
        return;
    }

    //printf ( "Faces: %ld \n" , objFile->faces_num );
    objFile->faces = (int**)malloc ( sizeof(int*) * objFile->faces_num );
    if ( !objFile->faces ) {
        perror ( "malloc" );
        return;
    }

    for ( i = 0 ; i < objFile->faces_num ; i ++ ) {
        objFile->faces[i] = (int*)malloc ( sizeof(int) * 3 );
        if ( !objFile->faces[i] ) {
            perror ( "malloc" );
            return;
        }
    }

    //Go back to beginning of file
    rewind ( fp );
    
    //Begin reading in data and store into OBJFile struct

    //Read in mtllib type
    if ( fscanf ( fp , "%s %s" , buffer , objFile->mtllib ) ) {
        //printf ( "%s\n", objFile->mtllib );
    }

    //Read in mtl version
    if ( fscanf ( fp , "%s %s" , buffer , objFile->mtl_ver ) ) {
        //printf ( "%s\n", objFile->mtl_ver );
    }

    //Read in vertices
    for ( i = 0 ; i < objFile->vertices_num ; i ++ ) {
        fscanf ( fp , "v" );
        fscanf ( fp , "%f %f %f" , &objFile->vertices[i][0] , &objFile->vertices[i][1] , &objFile->vertices[i][2] );
        fscanf ( fp , " " );
        objFile->vertices[i][3] = 1.0f;
        //printf ( "%f %f %f\n" , objFile->vertices[i][0] , objFile->vertices[i][1] , objFile->vertices[i][2] );
    }

    //Read in faces
    for ( i = 0 ; i < objFile->faces_num ; i ++ ) {
        fscanf ( fp , "f" );
        fscanf ( fp , "%i %i %i" , &objFile->faces[i][0] , &objFile->faces[i][1] , &objFile->faces[i][2] );
        fscanf ( fp , " " );
        //printf ( "%d %d %d\n" , objFile->faces[i][0] , objFile->faces[i][1] , objFile->faces[i][2] );
    }

}

void renderObj ( OBJFile * obj , point4 ** points) {
    //Counters
    long i , pt;

    //Create enough memory to store points of triangles
    *points = (point4*)malloc ( sizeof(point4) * 3 * obj->faces_num );

    for ( i = 0 , pt = 0 ; i < obj->faces_num ; i ++ ) {
        (*points)[pt] = obj->vertices[obj->faces[i][0]];
        (*points)[pt++] = obj->vertices[obj->faces[i][1]];
        (*points)[pt++] = obj->vertices[obj->faces[i][2]];
    }

}

int main( int argc , char **argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitWindowPosition(0,0);
    glutCreateWindow( "Color Cube" );

#ifndef __APPLE__
    glewInit ( );
#endif
    
    init();
    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMainLoop();
    
    
    return 0;
}
