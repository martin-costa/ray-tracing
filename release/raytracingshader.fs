const int maxObjects = 330;

uniform int width;
uniform int height;

uniform int sphereCount;

uniform bool light;

uniform vec3 pos;

uniform vec3 dir;
uniform vec3 up;
uniform vec3 side;

uniform vec3 spherePositions[maxObjects];
uniform float sphereRadii[maxObjects];
uniform vec3 sphereColors[maxObjects];

float ratio = float(width) / float(height);

float i = gl_FragCoord.x - width / 2;
float j = gl_FragCoord.y - height / 2;

//method to get intersection of line and sphere
float closestIntersection(vec3 orig, vec3 lineDir, int sphereIdx) {
  lineDir = normalize(lineDir);
  vec3 CO = orig - spherePositions[sphereIdx];
  float x = -(dot(lineDir, CO));
  float y = pow(dot(lineDir, CO), 2) - pow(length(CO), 2) + sphereRadii[sphereIdx] * sphereRadii[sphereIdx];

  if (y < 0) return -1.0;

  y = sqrt(y);
  float d1 = x - y;
  if (d1 >= 0) return d1;
  else return x + y;
}

void main() {

  int index = -1;
  float d = -1.0;

  for (int k = 0; k < sphereCount; k++) {
    float d2 = closestIntersection(pos, dir + side * i * ratio / (float(width) / 2) + up * j / (float(height) / 2), k);
    if (d2 > 0 && (d2 < d || d < 0)) {
      d = d2;
      index = k;
    }
  }

  if (d > 0) {
    vec3 v = pos + normalize(dir + side * i * ratio / (float(width) / 2) + up * j / (float(height) / 2)) * d;
    float f = dot( normalize(v - spherePositions[index]), -dir) * 0.1;

    if (light) gl_FragColor = vec4(sphereColors[index] * f, 1);
  }
  else gl_FragColor = vec4(0, 0, 0.2, 1);
}