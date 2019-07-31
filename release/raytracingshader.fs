const int maxObjects = 330;

uniform int width;
uniform int height;

uniform int sphereCount;
uniform int reflectionCount = 6;

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

  vec3 color = vec3(0, 0, 0);

  int index = -1;
  float d = -1.0;

  vec3 rayDir = normalize(dir + side * i * ratio / (float(width) / 2) + up * j / (float(height) / 2));
  vec3 rayOrigin = pos;
  vec3 rayEnd = pos;

  int reflections = 0;
  do {

	d = -1;

	int startIndex = index;
	index = -1;

	for (int k = 0; k < sphereCount; k++) {
	  if (k != startIndex) {
	    float d2 = closestIntersection(rayOrigin, rayDir, k);
        if (d2 > 0 && (d2 < d || d < 0)) {
          d = d2;
          index = k;
        }
	  }
    }
	rayEnd = rayOrigin + rayDir * d;

	float f = max(0, dot( normalize(rayEnd - spherePositions[index] ), vec3(0, 1, 0) ) );
	if (index > -1) {
	  if (reflections == 0) color = color + sphereColors[index] * f;
	  else color = color + sphereColors[index] * f * 0.6;
	}
	else {
	  if (reflections == 0) {
		color = vec3(0, 0, 0.1);
	  }
	  else color = color + vec3(0, 0, 0.07);
	  break;
	}

	rayOrigin = rayEnd;
  	vec3 n = normalize(rayEnd - spherePositions[index]);
	rayDir = rayDir - n * 2 * dot(n , rayDir);

  } while (reflections++ < reflectionCount);

  gl_FragColor = vec4(color, 1);
}