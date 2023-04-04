#version 450

in vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
in vec4 shadowCoord;
out vec4 fragColor;

struct PositionLight {
  vec4 ambient, diffuse, specular;
  vec3 position;
};
struct Material {
  vec4 ambient, diffuse, specular;
  float shininess;
};
uniform vec4 globalAmbient;
uniform PositionLight light;
uniform Material material;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_mat;
uniform mat4 shadowMVP2;
uniform float shadowbufferwidth, shadowbufferheight;
layout(binding = 0) uniform sampler2DShadow shTex;

float lookup(float ox, float oy) {
  float x=1.0/shadowbufferwidth;
  float y=1.0/shadowbufferheight;
  float t = textureProj(shTex, shadowCoord + vec4(ox * x * shadowCoord.w,
                                                  oy * y * shadowCoord.w,
                                                  -0.01, 0.0));
  return t;
}

void main() {
  float shadowFactor = 0.0;
  vec3 l = normalize(varyingLightDir);
  vec3 n = normalize(varyingNormal);
  vec3 v = normalize(-varyingVertPos);
  vec3 h = normalize(varyingHalfVec);

  // ----- this section produces a 4-sample dithered soft shadow
  // float swidth = 2.5;
  // // produces one of 4 sample patterns depending on glFragCoord mod 2
  // vec2 offset = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
  // shadowFactor += lookup(-1.5 * swidth + offset.x, 1.5 * swidth - offset.y);
  // shadowFactor += lookup(-1.5 * swidth + offset.x, -0.5 * swidth - offset.y);
  // shadowFactor += lookup(0.5 * swidth + offset.x, 1.5 * swidth - offset.y);
  // shadowFactor += lookup(0.5 * swidth + offset.x, -0.5 * swidth - offset.y);
  // shadowFactor = shadowFactor /
  //                4.0; // shadowFactor is an average of the four sampled points

  // ----- this section, if un-commented, produces a 64-sample hi resolution
  // soft shadow
  float swidth = 2.5; // tunable amount of shadow spread float
  float endp = swidth * 3.0 + swidth / 2.0;
  for (float m = -endp; m <= endp; m = m + swidth) {
    for (float n = -endp; n <= endp; n = n + swidth) {
      shadowFactor += lookup(m, n);
    }
  }
  shadowFactor = shadowFactor / 64.0;

  fragColor =
      globalAmbient * material.ambient + light.ambient * material.ambient;
  vec4 lightedColor = light.diffuse * material.diffuse * max(dot(l, n), 0.0) +
                      light.specular * material.specular *
                          pow(max(dot(h, n), 0.0), material.shininess * 3.0);
  fragColor += vec4(shadowFactor * (lightedColor.xyz), 1.0);
}