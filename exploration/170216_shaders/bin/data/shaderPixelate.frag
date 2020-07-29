#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;
uniform int pixelSize;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
	vec4 color;

	// get grayscale alpha from mask
    float mask = texture(maskTex, texCoordVarying).r; //gets the red value (0 or 10)

    //get all colours within areas marked as non-black by mask texture
    if (mask > 0){
    	//get colours within pixelSize area
    	float markerX = trunc(floor(texCoordVarying.x/pixelSize)*pixelSize); //x-pos of upper-left pixel within area
    	float markerY = trunc(floor(texCoordVarying.y/pixelSize)*pixelSize); //y-pos of upper-left pixel within area
    	//average colour within pixelSize
    	for (int x = 0; x < pixelSize; x++){
    		for (int y = 0; y < pixelSize; y++){
    			color += texture(tex0, vec2(markerX+x, markerY+y));
    		}
    	}
    	color /= pixelSize*pixelSize;
    } else {
    	color = texture(tex0, texCoordVarying);
    }

    outputColor = color;
}