#version 450
//#extension GL_KHR_vulkan_glsl:enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosition_WS;
layout(location = 2) out vec3 fragNormal_WS;

layout(push_constant) uniform Push{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
	vec3 mainLightDirection;
	vec4 mainLightColor;
	vec3 pointLightPosition;
	vec4 pointLightColor;
}push;

layout(binding = 0) uniform UniformBufferObject {
    vec3 TestColor;
} ubo;

void main(){
 	gl_Position = push.projectionMatrix * push.viewMatrix * push.modelMatrix * vec4(position,1.0);
	mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
	fragNormal_WS = normalize(normalMatrix * normal);
	fragPosition_WS = (push.modelMatrix * vec4(position,1.0f)).rgb;
	fragColor = color;
}