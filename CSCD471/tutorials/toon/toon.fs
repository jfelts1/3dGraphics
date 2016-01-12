#version 430

in vec3 Position;
in vec3 Normal;

uniform vec4 LightPosition;  // Light position in eye coords.



void main() {

      vec4 color;

      vec3 s = normalize(vec3(LightPosition) - Position);
      float intensity = max( dot(s,Normal), 0.0 );
      
      if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);

	gl_FragColor = color;

}
