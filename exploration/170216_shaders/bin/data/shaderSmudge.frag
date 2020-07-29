#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;
uniform mat50 inputColors;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;

    // get grayscale alpha from mask
    float mask = 10*texture(maskTex, texCoordVarying).r; //gets the red value (0 - 1) (since it's grayscale, it works)
	
    color = texture(tex0, );
    
    outputColor = color;
}