/* Graphics coursework 1
    Zacharias Markakis

    Note: The code from tutorial 3 was used as a template
          utils.c and utils.h were copied from tutorial 3

    Decrease or increase global variable dangle by multiples of 5 for greater or less detail respectively.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "utils.h"

int dangle = 10;

int sphereSize = 0, shadedSphereSize = 0, k = 0, shape = 0, coneLoopSize = 0;

GLdouble toRad = M_PI / 180.0;

GLdouble radius = 1.0;

GLdouble normalScale = 1.2;

struct Vertex
{
    GLdouble position[3];
    GLfloat color[3];
    GLdouble normal[3];
};

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint shaderprogram;
GLfloat projectionmatrix[16];
GLfloat modelmatrix[16];
const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;
int icount;
GLdouble icount2 = 0.0;

GLuint vbo[4];

void setSphereSize() //Find number of vertices on the sphere
{
    int i, j;

    for(i = -90; i <= 90 - dangle; i+=dangle)
    {
        for(j = 0; j <= 360 - dangle; j+=dangle)
        {
            sphereSize+=3;
            shadedSphereSize+=4;
            if(i > - 90 && i < 90){
                sphereSize++;
                shadedSphereSize++;
            }
        }
    }

}

void findAngleDivisor()
{

    while(360%dangle != 0)
        dangle--;

}

struct Vertex *getSphere() //Get the vertices of the sphere
{
    int i, j;
    struct Vertex *sphere = malloc(sphereSize * sizeof(struct Vertex));

    //Get 4 vertices every time that form a rectangle of the wireframe unless at top or bottom or sphere where we get 3 (triangle)
    for(i = - 90; i <= 90 - dangle; i+=dangle)
    {
        for(j = 0; j <= 360 - dangle; j+=dangle)
        {
            sphere[k].position[0] = radius * cos(i*toRad) * cos(j*toRad);
            sphere[k].position[1] = radius * cos(i*toRad) * sin(j*toRad);
            sphere[k].position[2] = radius * sin(i*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            sphere[k].position[0] = radius * cos((i+dangle)*toRad) * cos(j*toRad);
            sphere[k].position[1] = radius * cos((i+dangle)*toRad) * sin(j*toRad);
            sphere[k].position[2] = radius * sin((i+dangle)*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            sphere[k].position[0] = radius * cos((i+dangle)*toRad) * cos((j+dangle)*toRad);
            sphere[k].position[1] = radius * cos((i+dangle)*toRad) * sin((j+dangle)*toRad);
            sphere[k].position[2] = radius * sin((i+dangle)*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            if(i > - 90 && i < 90)
            {
                sphere[k].position[0] = radius * cos(i*toRad) * cos((j+dangle)*toRad);
                sphere[k].position[1] = radius * cos(i*toRad) * sin((j+dangle)*toRad);
                sphere[k].position[2] = radius * sin(i*toRad);
                sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
                k++;
            }
        }
    }
    for(i = 0; i < sphereSize; i++){
        sphere[i].normal[0] = sphere[i].position[0] * normalScale;
        sphere[i].normal[1] = sphere[i].position[1] * normalScale;
        sphere[i].normal[2] = sphere[i].position[2] * normalScale;
    }
    return sphere;
}

struct Vertex *getShadedSphere() //Get the vertices of the shaded sphere
{
    k = 0;

    int i, j;
    struct Vertex *sphere = malloc(shadedSphereSize * sizeof(struct Vertex));

    //As sphere, but add one extra vertex every time, so that using TRIANGLE_STRIP we can draw the surface of the sphere
    for(i = - 90; i <= 90 - dangle; i+=dangle)
    {
        for(j = 0; j <= 360 - dangle; j+=dangle)
        {
            sphere[k].position[0] = radius * cos(i*toRad) * cos(j*toRad);
            sphere[k].position[1] = radius * cos(i*toRad) * sin(j*toRad);
            sphere[k].position[2] = radius * sin(i*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            sphere[k].position[0] = radius * cos((i+dangle)*toRad) * cos(j*toRad);
            sphere[k].position[1] = radius * cos((i+dangle)*toRad) * sin(j*toRad);
            sphere[k].position[2] = radius * sin((i+dangle)*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            sphere[k].position[0] = radius * cos((i+dangle)*toRad) * cos((j+dangle)*toRad);
            sphere[k].position[1] = radius * cos((i+dangle)*toRad) * sin((j+dangle)*toRad);
            sphere[k].position[2] = radius * sin((i+dangle)*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            sphere[k].position[0] = radius * cos(i*toRad) * cos(j*toRad); //Extra vertex, same as first one in the loop
            sphere[k].position[1] = radius * cos(i*toRad) * sin(j*toRad);
            sphere[k].position[2] = radius * sin(i*toRad);
            sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
            k++;
            if(i > - 90 && i < 90)
            {
                sphere[k].position[0] = radius * cos(i*toRad) * cos((j+dangle)*toRad);
                sphere[k].position[1] = radius * cos(i*toRad) * sin((j+dangle)*toRad);
                sphere[k].position[2] = radius * sin(i*toRad);
                sphere[k].color[0] = sphere[k].color[1] = sphere[k].color[2] = 1.0f;
                k++;
            }
        }
    }
    for(i = 0; i < shadedSphereSize; i++){
        sphere[i].normal[0] = sphere[i].position[0] * normalScale;
        sphere[i].normal[1] = sphere[i].position[1] * normalScale;
        sphere[i].normal[2] = sphere[i].position[2] * normalScale;
    }
    return sphere;
}


struct Vertex *getHedgehog(struct Vertex *sphere)
{

    int i;

    struct Vertex *hedgehog = malloc(3 * sphereSize * sizeof(struct Vertex));


    //Get the points of the sphere
    for(i = 0; i < k; i++)
    {
        hedgehog[i].position[0] = sphere[i].position[0];
        hedgehog[i].position[1] = sphere[i].position[1];
        hedgehog[i].position[2] = sphere[i].position[2];
        hedgehog[i].color[0] = hedgehog[i].color[1] = hedgehog[i].color[2] = 1.0f;
    }
    //scale them by normalScale to find the vertices of the normals.
    for(i = k; i < 2*k; i+=2)
    {
        hedgehog[i].position[0] = hedgehog[i-k].position[0] * normalScale;
        hedgehog[i].position[1] = hedgehog[i-k].position[1] * normalScale;
        hedgehog[i].position[2] = hedgehog[i-k].position[2] * normalScale;
        hedgehog[i].color[0] = hedgehog[i].color[1] = hedgehog[i].color[2] = 1.0f;
        // Finally, re-add the vertices of the sphere to connect the lines.
        hedgehog[i+1].position[0] = sphere[i-k].position[0];
        hedgehog[i+1].position[1] = sphere[i-k].position[1];
        hedgehog[i+1].position[2] = sphere[i-k].position[2];
        hedgehog[i+1].color[0] = hedgehog[i+1].color[1] = hedgehog[i+1].color[2] = 1.0f;
    }

    return hedgehog;
}

struct Vertex *getCone()
{

    int i;

    struct Vertex *cone = malloc(5 * coneLoopSize * sizeof(struct Vertex));

    //Find vertices on circumference of a circle, connect them to centre (0,0,0), then do the same but connect them to (0,0,radius*1.5), finally connect the vertices
    //of the circle
    for(i = 0; i < coneLoopSize; i++)
    {
        cone[k].position[0] = radius * sin((i * dangle)*toRad);
        cone[k].position[1] = radius * cos((i * dangle)*toRad);
        cone[k].position[2] = 0.0;
        cone[k].color[0] = cone[k].color[1] = cone[k].color[2] = 1.0f;
        k++;
        cone[k].position[0] = 0.0;
        cone[k].position[1] = 0.0;
        cone[k].position[2] = 0.0;
        cone[k].color[0] = cone[k].color[1] = cone[k].color[2] = 1.0f;
        k++;
    }
    for(i = 0; i < coneLoopSize; i++)
    {
        cone[k].position[0] = radius * sin((i * dangle)*toRad);
        cone[k].position[1] = radius * cos((i * dangle)*toRad);
        cone[k].position[2] = 0.0;
        cone[k].color[0] = cone[k].color[1] = cone[k].color[2] = 1.0f;
        k++;
        cone[k].position[0] = 0.0;
        cone[k].position[1] = 0.0;
        cone[k].position[2] = radius*1.5;
        cone[k].color[0] = cone[k].color[1] = cone[k].color[2] = 1.0f;
        k++;
    }
    for(i = 0; i < coneLoopSize; i++)
    {
        cone[k].position[0] = radius * sin((i * dangle)*toRad);
        cone[k].position[1] = radius * cos((i * dangle)*toRad);
        cone[k].position[2] = 0.0;
        cone[k].color[0] = cone[k].color[1] = cone[k].color[2] = 1.0f;
        k++;
    }

    return cone;
}

//Set vbo to a specific shape
void setBuffer(int i, int shapeSize, struct Vertex *shape)
{

    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    glBufferData ( GL_ARRAY_BUFFER, shapeSize * sizeof ( struct Vertex ), shape, GL_STATIC_DRAW );  //cone
    glVertexAttribPointer ( ( GLuint ) 0, 3, GL_DOUBLE, GL_FALSE,  sizeof ( struct Vertex ), ( const GLvoid* ) offsetof (struct Vertex,position) );


    glEnableVertexAttribArray(0);


    glVertexAttribPointer ( ( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex,color) );

    glEnableVertexAttribArray ( 1 );


    glVertexAttribPointer ( ( GLuint ) 2, 3, GL_DOUBLE, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex,normal) );

    glEnableVertexAttribArray ( 2 );

}

//Rebind vbos to draw a different shape
void reBindBuffer(int i){
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    glVertexAttribPointer ( ( GLuint ) 0, 3, GL_DOUBLE, GL_FALSE,  sizeof ( struct Vertex ), ( const GLvoid* ) offsetof (struct Vertex,position) );
    glVertexAttribPointer ( ( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex,color) );
    glVertexAttribPointer ( ( GLuint ) 2, 3, GL_DOUBLE, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex,normal) );
}

void startOpenGL(void)
{
    k = 0;
    sphereSize = 0;

    findAngleDivisor();
    setSphereSize();

    coneLoopSize = 360 / dangle;

    struct Vertex *sphere;
    struct Vertex *hedgehog;
    struct Vertex *cone;
    struct Vertex *shadedSphere;

    sphere = getSphere();
    hedgehog = getHedgehog(sphere);
    shadedSphere = getShadedSphere();
    k = 0;
    cone = getCone();

    glGenBuffers(4, vbo);

    setBuffer(1, 5*coneLoopSize, cone);
    setBuffer(2, 3*sphereSize, hedgehog);
    setBuffer(3, shadedSphereSize, shadedSphere);
    setBuffer(0, sphereSize, sphere);


    vertexsource = filetobuf("cw1.vert");
    fragmentsource = filetobuf("cw1.frag");

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

    glCompileShader(vertexshader);
    glCompileShader(fragmentshader);


    shaderprogram = glCreateProgram();


    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");

    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);


    perspective(projectionmatrix, 45.0, 1.0, 0.1, 100.0);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

}

void key(unsigned char k, int x, int y)
{

    if(k == 'a' || k == 'A'){
        shape = 0;
        reBindBuffer(0);
    }
    if(k == 'b' || k == 'B'){
        shape = 1;
        reBindBuffer(1);
    }
    if(k == 'c' || k == 'C'){
        shape = 2;
        reBindBuffer(2);
    }
    if(k == 'd' || k == 'D'){
        shape = 3;
        reBindBuffer(3);
    }
    if(k == 'e' || k == 'E'){
        shape = 4;
    }
    if(k == 'q' || k == 'Q' || k == 27)
    {
        exit(0);
    }
    else if(k == 'r')
    {
        glClearColor(1., 0., 0., 1.);
    }

}

//Set the properties of the transformation matrices
void setMatrices(GLdouble i, GLdouble j, GLdouble k, int boolean)
{
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    rotate(modelmatrix, (GLfloat)icount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat)icount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat)icount * 0.5, Z_AXIS);
    translate(modelmatrix, i, j, k);
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "modelmatrix"), 1, GL_FALSE, modelmatrix);
    multiply4x4(modelmatrix, projectionmatrix);
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, modelmatrix);
    glUniform1i(glGetUniformLocation(shaderprogram, "isShading"), boolean);
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(shape == 0)//sphere
    {
        setMatrices(0.0, 0.0, -4.0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, sphereSize);
    }
    else if(shape == 1)  //cone
    {
        setMatrices(0.0, 0.0, -4.0, 0);
        glDrawArrays(GL_LINES, 0, 4*coneLoopSize);
        glDrawArrays(GL_LINE_LOOP, 4*coneLoopSize, coneLoopSize);
    }
    else if(shape == 2) //hedgehog
    {
        setMatrices(0.0, 0.0, -4.0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, sphereSize);
        glDrawArrays(GL_LINES, sphereSize, 2*sphereSize);
    }
     else if(shape == 3) //shaded sphere
    {
        setMatrices(0.0, 0.0, -4.0, 1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, shadedSphereSize);
    }
    else if(shape == 4){ // Animation
    reBindBuffer(3); // Draw shaded sphere going around in a circle along the x and z axis
    setMatrices(4*radius*sin(icount2*dangle*toRad), 0.0, 4*radius*cos(icount2*dangle*toRad) - 10.0, 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, shadedSphereSize);
    reBindBuffer(1); // Draw a wireframe cone going up and down along the y axis
    setMatrices(0.0, 2*radius*cos(icount2*dangle*toRad), -10.0, 0);
    glDrawArrays(GL_LINES, 0, 4*coneLoopSize);
    glDrawArrays(GL_LINE_LOOP, 4*coneLoopSize, coneLoopSize);
    }
    glFlush();
    glutSwapBuffers();

}

//Timer function that gets called every 'time' seconds, used for the animations.
void callback(int i)
{
    double time = 16.6;

    icount++;
    icount2 += 0.2;
    glutTimerFunc(time, callback, 0);

}
int main(int argc, char **argv)
{
    char *title = "Coursework 1";
    char *version;
    char outstring[100];

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutCreateWindow("Working ...");
    glewInit();
    glutKeyboardFunc(key);
    version = (char *) glGetString(GL_VERSION);
    fprintf(stderr, "OpenGL Version %s\n", version);
    sprintf(outstring, "%s: OpenGL version %s\n", title, version);
    glutSetWindowTitle(outstring);
    startOpenGL();
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutTimerFunc(0, callback, 0);
    glutMainLoop();

    return 1;
}
