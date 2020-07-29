#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;

    // get grayscale alpha from mask
    float mask = 10*texture(maskTex, texCoordVarying).r; //gets the red value (0 - 1) (since it's grayscale, it works)
	
    color += 1.0 * texture(tex0, texCoordVarying + vec2(mask * -4.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(mask * -3.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(mask * -2.0, 0.0));
    color += 4.0 * texture(tex0, texCoordVarying + vec2(mask * -1.0, 0.0));
    
    color += 5.0 * texture(tex0, texCoordVarying + vec2(mask, 0));
	
    color += 4.0 * texture(tex0, texCoordVarying + vec2(mask * 1.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(mask * 2.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(mask * 3.0, 0.0));
    color += 1.0 * texture(tex0, texCoordVarying + vec2(mask * 4.0, 0.0));
    
    color /= 25.0;
    
    outputColor = color;
}