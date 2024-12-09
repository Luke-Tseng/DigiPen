/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform mat4 WorldView, WorldInverse, WorldProj, ModelTr, NormalTr;

in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

out vec3 normalVec, lightVec, eyeVec, worldPos, tanVec;
out vec2 texCoord;
uniform vec3 lightPos;

void main()
{      
	// Computes world position at a pixel used for light and eye vector calculations
	vec3 worldPos = (ModelTr*vertex).xyz;

	// Computes the point’s projection on the screen
	gl_Position = WorldProj*WorldView*ModelTr*vertex;

	// Computes normal vector and outputs to fragment shader
	normalVec = vertexNormal*mat3(NormalTr);

	// Computes vectors toward light and eye and outputs them to the fragment shader
	vec3 eyePos = (WorldInverse*vec4(0,0,0,1)).xyz;
	lightVec = lightPos - worldPos;
	eyeVec = eyePos - worldPos;

	texCoord = vertexTexture;
	tanVec = mat3(ModelTr)*vertexTangent;
}
