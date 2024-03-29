#version 150

uniform sampler2D uTex0;
float Scale = 1.0;
in vec2 TexCoord0;
out vec4 oColor;

float find_closest(int x, int y, float c0)
{
    int dither[8][8] = {
        { 0, 32, 8, 40, 2, 34, 10, 42},
        {48, 16, 56, 24, 50, 18, 58, 26},
        {12, 44, 4, 36, 14, 46, 6, 38},
        {60, 28, 52, 20, 62, 30, 54, 22},
        { 3, 35, 11, 43, 1, 33, 9, 41},
        {51, 19, 59, 27, 49, 17, 57, 25},
        {15, 47, 7, 39, 13, 45, 5, 37},
        {63, 31, 55, 23, 61, 29, 53, 21}};
    
    float limit = 0.0;
    if(x < 8)
    {
        limit = (dither[x][y]+1)/64.0;
    }
    
    if(c0 < limit)
        return 0.0;
    return 1.0;
}

void main(void)
{
    vec4 lum = vec4(0.299, 0.587, 0.114, 0);
    float grayscale = dot(texture(uTex0, TexCoord0.xy), lum);
    vec3 rgb = texture(uTex0, TexCoord0).rgb;
    
    vec2 xy = gl_FragCoord.xy * Scale;
    int x = int(mod(xy.x, 8));
    int y = int(mod(xy.y, 8));
    
    vec3 finalRGB;
    finalRGB.r = find_closest(x, y, rgb.r);
    finalRGB.g = find_closest(x, y, rgb.g);
    finalRGB.b = find_closest(x, y, rgb.b);
    
    float final = find_closest(x, y, grayscale);
    oColor = vec4(finalRGB, 1.0);
}
