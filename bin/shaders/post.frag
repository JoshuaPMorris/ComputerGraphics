// A simple flat-color
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform int windowWidth;
uniform int windowHeight;
uniform float iTime;

out vec4 FragColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

vec4 Default(vec2 texCoord) {
    return texture(colorTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord) {
    vec2 texel = 1.0f / textureSize(colorTarget, 0);

    vec4 color = texture(colorTarget, texCoord);
    color += texture(colorTarget, texCoord + texel * vec2(-1,  1));
    color += texture(colorTarget, texCoord + texel * vec2(-1,  0));
    color += texture(colorTarget, texCoord + texel * vec2(-1, -1));

    color += texture(colorTarget, texCoord + texel * vec2( 0,  1));
    color += texture(colorTarget, texCoord + texel * vec2( 0, -1));

    color += texture(colorTarget, texCoord + texel * vec2( 1,  1));
    color += texture(colorTarget, texCoord + texel * vec2( 1,  0));
    color += texture(colorTarget, texCoord + texel * vec2( 1, -1));

    return color / 9;
}

vec4 Distort(vec2 texCoord) {
    vec2 mid = vec2(0.5f);
    float distanceFromTheCenter = distance(texCoord, mid);
    vec2 normalizeCoord = normalize(texCoord - mid);
    float bias = distanceFromTheCenter + 
        sin(distanceFromTheCenter * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalizeCoord;
    return texture(colorTarget, newCoord);
}

vec4 EdgeDetection(vec2 texCoord) {
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;

    vec4 k[9];

    k[0] = texture(colorTarget, texCoord + vec2(-w, -h));
    k[1] = texture(colorTarget, texCoord + vec2( 0, -h));
    k[2] = texture(colorTarget, texCoord + vec2( w, -h));
    k[3] = texture(colorTarget, texCoord + vec2(-w,  0));
    k[4] = texture(colorTarget, texCoord);
    k[5] = texture(colorTarget, texCoord + vec2( w,  0));
    k[6] = texture(colorTarget, texCoord + vec2(-w,  h));
    k[7] = texture(colorTarget, texCoord + vec2( 0,  h));
    k[8] = texture(colorTarget, texCoord + vec2( w,  h));

    vec4 sobelEdgeH = k[2] + (2.0f * k[5]) + k[8] - (k[0] + (2.0f * k[3]) + k[6]);
    vec4 sobelEdgeV = k[0] + (2.0f * k[1]) + k[2] - (k[6] + (2.0f * k[7]) + k[8]);

    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    return vec4(1.0f - sobel.rgb, 1.0f);
}

vec4 Sepia(vec2 texCoord) {
    vec4 color = texture(colorTarget, texCoord);

    float rr = .3;
    float rg = .769;
    float rb = .189;
    float ra = 0.0;
    
    float gr = .3;
    float gg = .686;
    float gb = .168;
    float ga = 0.0;
    
    float br = .272;
    float bg = .534;
    float bb = .131;
    float ba = 0.0;
    
    float red =     (rr * color.r) + (rb * color.b) + (rg * color.g) + (ra * color.a);
    float green =   (gr * color.r) + (gb * color.b) + (gg * color.g) + (ga * color.a);
    float blue =    (br * color.r) + (bb * color.b) + (bg * color.g) + (ba * color.a);

    return vec4(red, green, blue, 1.0f);
}

vec4 Scanlines(vec2 texCoord) {
    vec4 color = texture(colorTarget, texCoord);

    float count = windowHeight * 1.3f;

    vec2 s1 = vec2(sin(texCoord.y * count), cos(texCoord.y * count));
    vec3 scanLines = vec3(s1.x, s1.y, s1.x);

    color.xyz += color.xyz * scanLines * .3f;
    color.xyz += color.xyz * vec3(random(texCoord * iTime)) * .3f;
    color.xyz += color.xyz * sin(110.0f * iTime) * 0.03f;

    return color;
}

vec4 GreyScale(vec2 texCoord) {

    vec4 color = texture(colorTarget, texCoord);
    vec3 greyScale = vec3(.5f, .5f, .5f);

    FragColor = vec4( vec3(dot (color.xyz, greyScale)), color.x);

    return FragColor;
}

vec4 Invert(vec2 texCoord) {
    vec4 color = texture(colorTarget, texCoord);

    color.xyz = vec3(1, 1, 1) - color.xyz;

    return color;
}

vec4 Pixilizer(vec2 texCoord) {

    float Pixels = 1024.0;
    float dx = 15.0 * (1.0 / Pixels);
    float dy = 10.0 * (1.0 / Pixels);
    vec2 newCoord = vec2(dx * floor(texCoord.x / dx),
                         dy * floor(texCoord.y / dy));

    return texture(colorTarget, newCoord);
}

vec4 Posterization( vec2 texCoord) {
    vec3 color = texture(colorTarget, texCoord).xyz;

    color = pow(color, vec3(0.6, 0.6, 0.6));
    color = color * 100.0f;
    color = floor(color);
    color = color / 100.0f;
    color = pow(color, vec3(1.0 / 0.6));

    return vec4(color, 1.0f);
}

vec4 DistanceFog( vec2 texCoord) {
    return vec4(0);
}

vec4 DepthOfField( vec2 texCoord) {
    return vec4(0);
}

void main() {
    // this will calculate the texel size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Then we will adjust the coordinate
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    switch (postProcessTarget)
    {
        default: // Default
        {
            // Output post process effect here
            FragColor = Default(texCoord);
        }
        case 0: // BoxBlur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 1: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 2: // Edge Detection
        {
            FragColor = EdgeDetection(texCoord);
            break;
        }
        case 3: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }

        case 4: // Scanlines
        {
            FragColor = Scanlines(texCoord);
            break;
        }
        case 5: // Grey Scale
        {
            FragColor = GreyScale(texCoord);
            break;
        }
        case 6: // Invert
        {
            FragColor = Invert(texCoord);
            break;
        }

        case 7: // Pixilizer
        {
            FragColor = Pixilizer(texCoord);
            break;
        }
        case 8: // Posterization
        {
            FragColor = Posterization(texCoord);
            break;
        }
        case 9: // Distance Fog
        {
            FragColor = Default(texCoord);
            break;
        }
        case 10: // Depth of Field
        {
            FragColor = DepthOfField(texCoord);
            break;
        }
    }
}