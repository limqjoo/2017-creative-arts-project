#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;
uniform int pixelSize;

varying vec2 texCoordVarying;

void main()
{
	vec4 color = vec4(0, 0, 0, 0); //different from #version 150; must be assigned value

	// get grayscale alpha from mask
    float mask = texture2DRect(maskTex, texCoordVarying).r;

    //get all colours within areas marked as non-black by mask texture
    if (mask > 0){
    	//get colours within pixelSize area
    	float markerX = floor(texCoordVarying.x/pixelSize)*pixelSize; //x-pos of upper-left pixel within area
    	float markerY = floor(texCoordVarying.y/pixelSize)*pixelSize; //y-pos of upper-left pixel within area
    	//average colour within pixelSize
    	for (int x = 0; x < pixelSize; x++){
    		for (int y = 0; y < pixelSize; y++){
    			color += texture2DRect(tex0, vec2(markerX+x, markerY+y));
    		}
    	}
    	color /= pixelSize*pixelSize;
    } else {
    	color = texture2DRect(tex0, texCoordVarying);
    }

    gl_FragColor = color;
}