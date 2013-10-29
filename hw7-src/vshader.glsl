attribute  vec4 vPosition;
vec4 tmp;
void main() 
{
  tmp = .2*vPosition;
  tmp[3] = 1.0;
  gl_Position = tmp;
} 
