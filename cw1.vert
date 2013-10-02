#version 150

precision highp float;

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

// mvpmatrix is the result of multiplying the model, view, and projection matrices */

uniform mat4 modelmatrix;

uniform mat4 mvpmatrix;

uniform int isShading;

out vec3 ex_Color;


void main(void) {
// Multiply the mvp matrix by the vertex to obtain our final vertex position

     gl_Position = mvpmatrix * vec4(in_Position, 1.0);

    if(isShading == 1){
    vec4 modelInNorm = modelmatrix * vec4(in_Normal, 1.0);

    vec4 modelInPos = modelmatrix * vec4(in_Position, 1.0);

    vec4 modelInLight = vec4(.0, .0, -modelInPos[2], 1.0);

    vec4 normalVec = normalize(modelInNorm - modelInPos);

    vec4 lightVec = normalize(modelInLight -modelInPos);



    ex_Color =  in_Color * dot(normalVec, lightVec);
    }
    else{
    ex_Color = in_Color;
    }
}
