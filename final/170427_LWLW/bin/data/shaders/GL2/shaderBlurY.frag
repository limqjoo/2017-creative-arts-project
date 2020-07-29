#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

varying vec2 texCoordVarying;

void main()
{
    vec4 colour = vec4(0, 0, 0, 0);

    // get grayscale alpha from mask
    float mask = 10 * texture2DRect(maskTex, texCoordVarying).r;
	
	colour += 1.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * 4.0));
	colour += 2.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * 3.0));
	colour += 3.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * 2.0));
	colour += 4.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * 1.0));
    
	colour += 5.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask));
	
	colour += 4.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * -1.0));
	colour += 3.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * -2.0));
	colour += 2.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * -3.0));
	colour += 1.0 * texture2DRect(tex0, texCoordVarying + vec2(0.0, mask * -4.0));
    
    gl_FragColor = colour/25.0;
}