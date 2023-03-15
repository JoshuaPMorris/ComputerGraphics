// A phong flat-color
#version 410

in vec4 vPosition;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 CameraPosition;

// Model Data
uniform vec3 Ka; // The ambient material colour
uniform vec3 Kd; // The diffuse material colour
uniform vec3 Ks; // The specular material color
uniform float SpecularPower; // The specular power of Ks

// Light data
uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightDirection;

void main() {
    // Set the normal and light direction
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the negative light direction (Lambert term)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector...
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    // ..and the reflection vector
    vec3 R = reflect(L, N);

    // Calculate the specular term
    float specularTerm = pow(max(0, dot(R, V)), SpecularPower);

    // Calculate the properties of each color type
    vec3 ambient = AmbientColor * Ka;
    vec3 diffuse = LightColor * Ka * lambertTerm;
    vec3 specular = LightColor * Ks * specularTerm;

    FragColor = vec4(ambient + diffuse + specular, 1);
}