#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 ex_Normal;
in vec3 ex_light;

uniform sampler2D tex;
uniform mat4 mdlMatrix;
uniform vec3 light;
uniform int textures;
uniform vec4 color;


float shade;

vec4 tmp_color;

vec3 s,n;
mat3 lightMdl = mat3(mdlMatrix);
void main(void)
{
    s = normalize(light);
    n = normalize(ex_Normal);
    shade = dot(s, n);
    shade = max(shade, 0);
    if(textures == 1){
    	tmp_color = texture(tex, texCoord);
    }else{
        tmp_color = color;
    }
    outColor = tmp_color + shade*tmp_color;
}
