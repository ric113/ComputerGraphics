#version 330

layout(location = 0) in vec3 SkinPosition;
layout(location = 1) in vec3 SkinNormal;
layout(location = 2) in vec2 SkinTexCor;
layout(location = 3) in vec3 StemPosition;
layout(location = 4) in vec3 StemNormal;
layout(location = 5) in vec2 StemTexCor;
layout(location = 6) in vec3 SkinFacetNor;
layout(location = 7) in vec3 StemFacetNor;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform int CurrentTarget;
uniform int CurrentShadeMode;

out vec2 TexCor;
out vec3 Normal;
out vec3 FragPos;



void main() {
	if(CurrentTarget == 0)
	{
		gl_Position = Projection * ModelView * vec4(SkinPosition,1.0);
		TexCor = SkinTexCor ;
		if(CurrentShadeMode == 1)
			Normal = SkinNormal;
		else
			Normal = SkinFacetNor;
        FragPos = SkinPosition;
	}
	else
	{
		gl_Position = Projection * ModelView * vec4(StemPosition,1.0);
		TexCor = StemTexCor ;
		if(CurrentShadeMode == 1)
			Normal = StemNormal ;
		else
			Normal = StemFacetNor;
		FragPos = StemPosition ;
	}

}