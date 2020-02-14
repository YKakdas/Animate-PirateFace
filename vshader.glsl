#version 150

in  vec3 vPosition;
in  vec4 vColor;
out vec4 color;
uniform vec3 scale;
uniform vec3 translate;
uniform vec3 rotate;
void main() 
{
  mat4 scaleMat = mat4(scale.x,0.0,0.0,0.0,
					0.0,scale.y,0.0,0.0,
					0.0,0.0,scale.z,0.0,
					0.0,0.0,0.0,1.0);
					
	float angle = radians(rotate.z);
	float angleCos = cos(angle);
	float angleSin = sin(angle);
	
  
  mat4 rotateMat = mat4( angleCos, angleSin, 0.0, 0.0,
						-angleSin,  angleCos, 0.0, 0.0,
						0.0,  0.0, 1.0, 0.0,
						0.0,  0.0, 0.0, 1.0 );
					
  mat4 translateMat = mat4(1.0,0.0,0.0,0.0,
					0.0,1.0,0.0,0.0,
					0.0,0.0,1.0,0.0,
					translate.x,translate.y,translate.z,1.0);

  gl_Position = translateMat*rotateMat*scaleMat*vec4(vPosition,1.0);
  color = vColor;
}
