#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

varying vec2 texCoordVarying;

void main()
{
    vec4 colour;

    // get grayscale alpha from mask
    float mask = 10*texture2DRect(maskTex, texCoordVarying).r; //gets the red value (0 - 1) (since it's grayscale, it works)

	// apply kernel
    colour += texture2DRect(tex0, texCoordVarying + vec2(mask * -4.0, mask * -4.0));
    colour += 2 * texture2DRect(tex0, texCoordVarying + vec2(mask * -3.0, mask * -3.0));
    colour += 3 * texture2DRect(tex0, texCoordVarying + vec2(mask * -2.0, mask * -2.0));
    colour += 4 * texture2DRect(tex0, texCoordVarying + vec2(mask * -1.0, mask * -1.0));
    
    colour += 5 * texture2DRect(tex0, texCoordVarying + vec2(mask, mask));
	
    colour += 4 * texture2DRect(tex0, texCoordVarying + vec2(mask * 1.0, mask * 1.0));
    colour += 3 * texture2DRect(tex0, texCoordVarying + vec2(mask * 2.0, mask * 2.0));
    colour += 2 * texture2DRect(tex0, texCoordVarying + vec2(mask * 3.0, mask * 3.0));
    colour += texture2DRect(tex0, texCoordVarying + vec2(mask * 4.0, mask * 4.0));
    
    gl_FragColor = colour/25.0;
}