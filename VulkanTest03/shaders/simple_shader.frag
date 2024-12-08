#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosition_WS;
layout(location = 2) in vec3 fragNormal_WS;

layout (location = 0) out vec4 outColor_1;
layout (location = 1) out vec4 outColor_2;

layout(push_constant) uniform Push{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
	vec3 mainLightDirection;
	vec4 mainLightColor;
	vec3 pointLightPosition;
	vec4 pointLightColor;
}push;

const vec3 AMBIENT = {0.02f,0.02f,0.02f};

void main(){

	//directional light
//	vec3 lightColor = push.mainLightColor.rgb * push.mainLightColor.a;
//	vec3 diffuseLight = lightColor * max(dot(fragNormal_WS, push.mainLightDirection),0);

	//point light
	vec3 directionToPointLight = push.pointLightPosition - fragPosition_WS.xyz;
	float attenuation = 1.0 / dot(directionToPointLight,directionToPointLight);  //distance squared;
	directionToPointLight = normalize(push.pointLightPosition - fragPosition_WS.xyz);
	vec3 lightColor = push.pointLightColor.rgb * push.pointLightColor.a * attenuation;
	vec3 diffuseLight = lightColor * max(dot(fragNormal_WS, directionToPointLight),0);

	vec3 finalColor = fragColor * (diffuseLight + AMBIENT);

	outColor_1 = vec4(finalColor,1.0);
	outColor_2 = vec4(fragNormal_WS,1.0);
}