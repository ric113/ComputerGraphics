#version 330

in vec2 TexCor;
in vec3 Normal;
in vec3 FragPos;
out vec4 outColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform vec3 lightPos; 
uniform vec3 viewPos;

uniform int CurrentTarget;
uniform int CurrentShadeMode;

void main() {

	// lightColor 為白光 , 因此是vec3(1.0) .
	
	// Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0);
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * vec3(1.0);  
        
    if(CurrentShadeMode == 1)
    {
    	if(CurrentTarget == 0)
			outColor = vec4(ambient + texture(ourTexture1, TexCor) * diffuse + specular,1.0) ;
		else
			outColor = vec4(ambient + texture(ourTexture2, TexCor) * diffuse + specular,1.0) ;
	}
	else
	{
		if(CurrentTarget == 0)
			outColor = vec4(ambient + texture(ourTexture1, TexCor) * diffuse ,1.0) ;
		else
			outColor = vec4(ambient + texture(ourTexture2, TexCor) * diffuse ,1.0) ;
	}

}
