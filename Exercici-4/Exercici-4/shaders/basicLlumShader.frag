#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec4  fvertex;
in vec3  fnormal;
in vec3  LSCO;
in vec3  LSCO0;
in vec3  LSCO1;
in vec3  LSCO2;
in vec3  LSCO3;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform vec3 colorFocusEscena;
uniform vec3 colorLlum0;
uniform vec3 colorLlum1;
uniform vec3 colorLlum2;
uniform vec3 colorLlum3;


out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{

  vec3 nLSCO = normalize(LSCO);
  vec3 nLSCO0 = normalize(LSCO0);
  vec3 nLSCO1 = normalize(LSCO1);
  vec3 nLSCO2 = normalize(LSCO2);
  vec3 nLSCO3 = normalize(LSCO3);
  vec3 normal = normalize(fnormal);

  
  vec3 color = Ambient() + Difus(normal, nLSCO, colorFocusEscena)
          + Especular(normal, nLSCO, fvertex.xyz, colorFocusEscena)
          + Difus(normal, nLSCO0, colorLlum0)
          + Especular(normal, nLSCO0, fvertex.xyz, colorLlum0)
          + Difus(normal, nLSCO1, colorLlum1)
          + Especular(normal, nLSCO1, fvertex.xyz, colorLlum1)
          + Difus(normal, nLSCO2, colorLlum2)
          + Especular(normal, nLSCO2, fvertex.xyz, colorLlum2)
          + Difus(normal, nLSCO3, colorLlum3)
          + Especular(normal, nLSCO3, fvertex.xyz, colorLlum3);

  FragColor = vec4(color, 1.0);
}