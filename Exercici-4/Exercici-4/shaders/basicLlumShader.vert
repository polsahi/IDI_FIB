#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 TGModel;
uniform vec3 posFocusEscena;
uniform vec3 posLlum0;
uniform vec3 posLlum1;
uniform vec3 posLlum2;
uniform vec3 posLlum3;


out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec4  fvertex;
out vec3  fnormal;
out vec3  LSCO;
out vec3  LSCO0;
out vec3  LSCO1;
out vec3  LSCO2;
out vec3  LSCO3;


void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;

    // fet per mi
    
    mat3 normalMatrix = inverse(transpose(mat3(View*TG)));
    fvertex = View*TG*vec4(vertex, 1.0);
    fnormal = normalize(normalMatrix*normal); 
    vec4 posFocusEscenaSCO = View*vec4(posFocusEscena, 1.0);
    vec4 posLlum0SCO = View*TGModel*vec4(posLlum0, 1.0);
    vec4 posLlum1SCO = View*TGModel*vec4(posLlum1, 1.0);
    vec4 posLlum2SCO = View*TGModel*vec4(posLlum2, 1.0);
    vec4 posLlum3SCO = View*TGModel*vec4(posLlum3, 1.0);

    LSCO  = normalize(posFocusEscenaSCO.xyz - fvertex.xyz);
    LSCO0 = normalize(posLlum0SCO.xyz - fvertex.xyz);
    LSCO1 = normalize(posLlum1SCO.xyz - fvertex.xyz);
    LSCO2 = normalize(posLlum2SCO.xyz - fvertex.xyz);
    LSCO3 = normalize(posLlum3SCO.xyz - fvertex.xyz);

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}