#version 330 core

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 uv_in;
layout(location = 3) in vec3 tangent_in;
layout(location = 4) in vec3 bitangent_in;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceMatrix;

out vec3 v_normal;
out vec2 v_uv;
out mat3 v_tbnMatrix;
out vec3 v_fragmentPosition;
out vec4 v_fragmentLightSpacePosition;

vec3 getNormals() {
    return normalize(normalMatrix * normal_in);
}

mat3 getTBNMatrix(){
    vec3 t = normalize(normalMatrix * tangent_in);
    vec3 b = normalize(normalMatrix * bitangent_in);
    vec3 n = v_normal;
    return mat3(t, b, n);
}

vec3 getFragmentPosition(){
    return vec3(modelMatrix * vec4(position_in, 1.0));
}

vec4 getFragmentLightSpacePosition() {
    return lightSpaceMatrix * vec4(v_fragmentPosition, 1.0);
}

void main()
{
    v_normal = getNormals();
    v_uv = uv_in;
    v_tbnMatrix = getTBNMatrix();
    v_fragmentPosition = getFragmentPosition();
    v_fragmentLightSpacePosition = getFragmentLightSpacePosition();

    gl_Position = mvpMatrix * vec4(position_in, 1.0);
}
