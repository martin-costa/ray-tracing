const int maxObjects = 330;
const int reflectionCount = 6;

uniform int width;
uniform int height;

uniform int sphereCount;

uniform vec3 pos;

uniform vec3 dir;
uniform vec3 up;
uniform vec3 side;

uniform vec3 spherePositions[maxObjects];
uniform float sphereRadii[maxObjects];
uniform vec3 sphereColors[maxObjects];

attribute vec3[reflectionCount] colors;
attribute vec3[reflectionCount] objectColors;

//global variables
int bounces = reflectionCount - 1;

float ratio = float(width) / float(height);

float i = gl_FragCoord.x - width / 2;
float j = gl_FragCoord.y - height / 2;

float d = -1;
int index = -1;

int reflections = 0;

vec3 rayDir = normalize(dir + side * i * ratio / (float(width) / 2) + up * j / (float(height) / 2));
vec3 rayOrigin = pos;
vec3 rayEnd = pos;

//method to get intersection of line and sphere
float sphereIntersection(vec3 orig, vec3 lineDir, int sphereIdx) {
  vec3 CO = orig - spherePositions[sphereIdx];
  float x = -(dot(lineDir, CO));
  float y = pow(dot(lineDir, CO), 2) - pow(length(CO), 2) + sphereRadii[sphereIdx] * sphereRadii[sphereIdx];

  if (y < 0) return -1.0;

  y = sqrt(y);
  float d1 = x - y;
  if (d1 >= 0) return d1;
  else return x + y;
}

//method to get closest intersection of a ray of light
void getClosestIntersection(int startIndex) {
  for (int k = 0; k < sphereCount; k++) {
	if (k != startIndex) {
	  float d2 = sphereIntersection(rayOrigin, rayDir, k);
      if (d2 > 0 && (d2 < d || d < 0)) {
        d = d2;
        index = k;
      }
	}
  }
  if (startIndex != sphereCount) {
    float d2 = -rayOrigin.y / rayDir.y;
    if (d2 > 0 && (d2 < d || d < 0)) {
      d = d2;
      index = sphereCount;
    }
  }
}

//get normal to object at index
vec3 getNormal(int index) {
  if (index < sphereCount) {
    return normalize(rayEnd - spherePositions[index]);
  }
  return vec3(0, 1, 0);
}

//recursive ray tracing method
void traceRay() {
 
  int startIndex = index;
  index = -1;
  d = -1;
  
  getClosestIntersection(startIndex);
  rayEnd = rayOrigin + rayDir * d;
  
  vec3 n = getNormal(index);
  
  //if the floor is hit
  if (index == sphereCount) {
    vec2 tile = vec2(int(rayEnd.x) % 51, int(rayEnd.z) % 51);
	//int tile = int(sqrt(rayEnd.x * rayEnd.x + rayEnd.z * rayEnd.z)) % 50;
    int t = 0;
    if (tile.x >= 25) t++;
    if (tile.y >= 25) t++;
	//if (tile >= 25) t++;
    if (t % 2 == 0)
      colors[reflections] = vec3(0.05, 0.05, 0.05);
    else
      colors[reflections] = vec3(0.75, 0.75, 0.75);
	objectColors[reflections] = vec3(1, 1, 1);
  }

  //if a sphere is hit
  else if (index > -1) {
    float f = max(0, dot(n, vec3(0, 0.8, 0)));
    if (reflections == 0) colors[reflections] = sphereColors[index] * f;
    colors[reflections] = sphereColors[index] * f * 1;
	objectColors[reflections] = sphereColors[index];
  }

  //if ray goes into void
  else {
    if (reflections == 0) {
  	  colors[reflections] = vec3(0, 0, 0.1);
    }
    else {
	  colors[reflections] = vec3(0, 0, 0.07);
	}
	objectColors[reflections] = vec3(0.0, 0.0, 0.07);
	bounces = reflections;
	reflections = reflectionCount;
  }
  
  rayOrigin = rayEnd;
  rayDir = rayDir - n * 2 * dot(n , rayDir);
}

//get the final color of the pixel
vec3 collapse(vec3[] colors) {
  for (int i = bounces; i > 0; i--) {
    colors[i - 1] = colors[i - 1] + objectColors[i - 1] * colors[i];
  }
  return colors[0];
}

//main method to get color of pixel
void main() {

  while (reflections < reflectionCount) {
    traceRay();
	reflections++;
  }

  //set the color of the pixel to color
  gl_FragColor = vec4(collapse(colors), 1);
}