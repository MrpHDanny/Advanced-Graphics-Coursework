#version  330  core

// Textures
uniform sampler2D grassTex;
uniform sampler2D sandTex;
uniform sampler2D stoneTex;

// Texture maps
uniform sampler2D grassMap;
uniform sampler2D sandMap;
uniform sampler2D stoneMap;

// Normal maps
uniform sampler2D grassNormal;
uniform sampler2D sandNormal;
uniform sampler2D stoneNormal;

// Light
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex
{
    vec3 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

vec4 getTexture() 
{
    if (texture(sandMap, IN.texCoord / 32 ).g >= 0.1) 
    {
        return texture(sandTex, IN.texCoord * 3);
    }

    if (texture(stoneMap, IN.texCoord / 32).g >= 0.3)
    {
        return texture(stoneTex, IN.texCoord * 3);
    }

    if(texture(grassMap, IN.texCoord/ 32).g >= 0.1)
    {
        return texture(grassTex, IN.texCoord * 3);
    }

    return texture(sandTex, IN.texCoord * 3);
    
}

vec3 getNormal() 
{

    if (texture(sandMap, IN.texCoord / 32).g >= 0.1) 
    {
        return texture(sandNormal, IN.texCoord * 3).rgb;
    }

    if (texture(stoneMap, IN.texCoord / 32).g >= 0.3) 
    {
        return texture(stoneNormal, IN.texCoord * 3).rgb;
    } 
   
    if (texture(grassMap, IN.texCoord / 32).g >= 0.1) 
    {
         return texture(grassNormal, IN.texCoord * 3).rgb;
    } 

    return texture(sandNormal, IN.texCoord * 3).rgb;
}


void  main(void)
{
    vec3 incident = normalize(lightPos - IN.worldPos);
    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
    
    vec4 diffuse = getTexture();    // get correct texture
    vec3 bumpNormal = getNormal();  // get correct normal 

    bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

    float lambert = max(dot(incident, bumpNormal), 0.0f);
    float distance = length(lightPos - IN.worldPos);
    float attenuation = 1.0 - clamp(distance / lightRadius, 0.0, 1.0);
    float specFactor = clamp(dot(halfDir, bumpNormal), 0.0 ,1.0);

    specFactor = pow(specFactor, 60.0);
    
    vec3 surface = (diffuse.rgb * lightColour.rgb);
    fragColour.rgb = surface * lambert * attenuation;
    fragColour.rgb += (lightColour.rgb * specFactor) * attenuation * 0.33;
    fragColour.rgb += surface * 0.3f; // ambient
    fragColour.a = diffuse.a;
    
    //fragColour = getTexture();
}