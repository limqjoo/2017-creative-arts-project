#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

varying vec2 texCoordVarying;

void main()
{
    vec4 colour = vec4(0, 0, 0, 0); //different from #version 150; must be assigned value

    // get grayscale alpha from mask
    float mask = 10 * texture2DRect(maskTex, texCoordVarying).r; //gets the red value (0 - 1) (since it's grayscale, it works)

    colour += 1 * texture2DRect(tex0, texCoordVarying + vec2(mask * -4.0, 0));
    colour += 2 * texture2DRect(tex0, texCoordVarying + vec2(mask * -3.0, 0));
    colour += 3 * texture2DRect(tex0, texCoordVarying + vec2(mask * -2.0, 0));
    colour += 4 * texture2DRect(tex0, texCoordVarying + vec2(mask * -1.0, 0));
    
    colour += 5 * texture2DRect(tex0, texCoordVarying + vec2(mask,0));
	
    colour += 4 * texture2DRect(tex0, texCoordVarying + vec2(mask * 1.0, 0));
    colour += 3 * texture2DRect(tex0, texCoordVarying + vec2(mask * 2.0, 0));
    colour += 2 * texture2DRect(tex0, texCoordVarying + vec2(mask * 3.0, 0));
    colour += 1 * texture2DRect(tex0, texCoordVarying + vec2(mask * 4.0, 0));
    
    gl_FragColor = colour/25.0;
}