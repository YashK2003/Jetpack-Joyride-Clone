#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform int isZapper;


// texture sampler
uniform sampler2D texture1;

void main()
{
	vec4 temp = texture(texture1, TexCoord);
      
    if(temp.a < 1){
		  discard;
	}

    float dist = TexCoord.y;

    vec4 yellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    vec4 red = vec4(1.0f, 0.0f,0.0f, 1.0f);

    float step1 = 0.0f;     
    float step2 = 0.5f;
    float step3 = 1.0f;

    vec4 color = mix(yellow, red, smoothstep(step1, step2, dist));
    color = mix(color, yellow, smoothstep(step2, step3, dist));
    
    if(isZapper == 1)
        FragColor = color;
    else
        FragColor =  temp;

}

