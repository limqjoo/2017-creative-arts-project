#version 120

// these are for the programmable pipeline system
//uniform mat4 modelViewProjectionMatrix;
//uniform mat4 textureMatrix;

//attribute vec4 position;
//attribute vec2 texcoord;
//attribute vec4 normal;
//attribute vec4 color;

varying vec2 texCoordVarying;

void main()
{
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}
