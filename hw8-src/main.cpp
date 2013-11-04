#include "../include/Angel.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

typedef Angel::vec4  point4;
typedef struct OBJFile{
    char mtllib[100];
    char mtl_ver[100];
    long vertices_num;
    long faces_num;
    long byte_size;
    int ** faces;
    point4 * vertices;
    mat4 transform;

    void reset ( ) {
        this->transform = mat4();
    };
    
    void translate ( point4 translateVector ) {
        this->transform *= Translate ( translateVector[0] , translateVector[1] , translateVector[2] );
    };

    void scale ( point4 scaleVector ) {
        this->transform *= Scale ( scaleVector[0] , scaleVector[1] , scaleVector[2] );
    };

    void readFile ( const char * filename ) {
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
        
        this->vertices_num = 0;
        this->faces_num = 0;
        
        //Count all vertices and faces
        while ( fgets ( buffer , sizeof(buffer) , fp ) ) {
            if ( strstr ( buffer , "v" ) ) {
                this->vertices_num ++;
            } else
            if ( strstr ( buffer , "f" ) ) {
                this->faces_num ++;
            }
        }

        //Allocate memory for vertices and faces
        //printf ( "Vertices: %ld \n" , this->vertices_num );
        this->vertices = (point4*)malloc ( sizeof(point4) * this->vertices_num );
        if ( !this->vertices ) {
            perror ( "malloc" );
            return;
        }

        //printf ( "Faces: %ld \n" , this->faces_num );
        this->faces = (int**)malloc ( sizeof(int*) * this->faces_num );
        if ( !this->faces ) {
            perror ( "malloc" );
            return;
        }

        for ( i = 0 ; i < this->faces_num ; i ++ ) {
            this->faces[i] = (int*)malloc ( sizeof(int) * 3 );
            if ( !this->faces[i] ) {
                perror ( "malloc" );
                return;
            }
        }

        //Go back to beginning of file
        rewind ( fp );
        
        //Begin reading in data and store into OBJFile struct

        //Read in mtllib type
        if ( fscanf ( fp , "%s %s\n" , buffer , this->mtllib ) ) {
            //printf ( "%s\n", this->mtllib );
        }

        //Read in mtl version
        if ( fscanf ( fp , "%s %s\n" , buffer , this->mtl_ver ) ) {
            //printf ( "%s\n", objFile->mtl_ver );
        }

        //Read in vertices
        for ( i = 0 ; i < this->vertices_num ; i ++ ) {
            fscanf ( fp , "v %f %f %f \n" , &this->vertices[i][0] , &this->vertices[i][1] , &this->vertices[i][2] );
            this->vertices[i][3] = 1.0f;
            //printf ( "%f %f %f\n" , this->vertices[i][0] , this->vertices[i][1] , this->vertices[i][2] );
        }
        
        //Read in faces
        for ( i = 0 ; i < this->faces_num ; i ++ ) {
            fscanf ( fp , "f %i %i %i \n" , &this->faces[i][0] , &this->faces[i][1] , &this->faces[i][2] );
            //printf ( "%d %d %d \n" , this->faces[i][0] , this->faces[i][1] , this->faces[i][2] );
        }

        this->byte_size = sizeof(point4) * 3 * this->faces_num;

    };

    void render ( point4 * points , long offset ) {
        //Counters
        long i , pt;

        for ( i = 0 , pt = 0 ; i < this->faces_num ; i ++ ) {
            points[offset+pt++] = this->transform * this->vertices[this->faces[i][0]-1];
            points[offset+pt++] = this->transform * this->vertices[this->faces[i][1]-1];
            points[offset+pt++] = this->transform * this->vertices[this->faces[i][2]-1];
        }
    };
} OBJFile; 


OBJFile obj;
int teapots = 3;
point4 * points;

//OpenGL initialization
void init ( ) {
    //counter
    int i; 

    //Read in file "teapot.obj" and then render to points buffer
    obj.readFile ( "teapot.obj" );

    //Points buffer
    points = (point4*)malloc ( obj.byte_size * teapots );
    memset ( points , 0x0 , obj.byte_size * teapots );
    
    //Create n Teapots
    for ( i = 0 ; i < teapots ; i ++ ) {
        obj.scale ( point4 ( 0.25f/teapots , 0.25f/teapots , 0.25f/teapots ) );
        obj.translate ( point4 ( i*2.5 , 0 , 0 ) );
        obj.render ( points , i*obj.faces_num*3 );
        obj.reset ( );
    }

    //Create a vertex array object
    GLuint vao[1];
    _glGenVertexArrays ( 1 , vao );
    _glBindVertexArray ( vao[0] );


    //Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1 , &buffer );
    glBindBuffer( GL_ARRAY_BUFFER , buffer );
    glBufferData( GL_ARRAY_BUFFER , obj.byte_size * teapots, points, GL_STATIC_DRAW );
    
    //Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    GLint location = glGetAttribLocation ( program , "vPosition" );
    glEnableVertexAttribArray ( location );
    glVertexAttribPointer ( location , 4 , GL_FLOAT , GL_FALSE ,
                          0 , BUFFER_OFFSET ( 0 ) );
    
    glEnable ( GL_DEPTH_TEST );

    glClearColor( 0.0f , 0.0f , 0.0f , 1.0f ); 
    free ( points );
}



//----------------------------------------------------------------------------

void display ( ) {
    //glPolygonMode( GL_FRONT_AND_BACK , GL_LINE ) ;
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glDrawArrays( GL_TRIANGLES, 0, teapots * obj.faces_num * 3 );
    
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

int getNumberOfTeapots ( int argc , char ** argv ) {
    char c;
    int temp = 2;
    while ((c = getopt (argc, argv, "n:")) != -1) {
        switch ( c ) {
            case 'n':
            {
                temp = atoi(optarg);
            }
            break;
        }
    }
    return temp;
}

int main( int argc , char **argv ) {
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize ( 512, 512 );
    glutInitWindowPosition ( 0 , 0);
    glutCreateWindow ( "Teapot" );

#ifndef __APPLE__
    glewInit ( );
#endif
    
    //Get Number of Teapots
    teapots = getNumberOfTeapots ( argc , argv );

    init ( );
    
    glutDisplayFunc ( display );
    glutKeyboardFunc ( keyboard );
    glutMainLoop ( );
    
    
    return 0;
}
