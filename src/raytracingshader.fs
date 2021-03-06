//#version 430

//// ------------------ ////
//// constant variables ////
//// ------------------ ////

const int maxObjects = 50;
const int maxLights = 20;

const int maxCastCount = 6;

// the types of scene objects [VOID: -1, SPHERE: 0, FLOOR: 1, TRIANGLE: 2]
const int VOID_OBJ = -1, SPHERE_OBJ = 0, FLOOR_OBJ = 1, TRIANGLE_OBJ = 2;

//// ----------------- ////
//// uniform variables ////
//// ----------------- ////

// width and height
uniform int width;
uniform int height;

uniform float reflectivity = 0;
uniform float diffuse = 0;

// camera position and orientation
uniform vec3 pos;

uniform vec3 dir;
uniform vec3 up;
uniform vec3 side;

// the data for the scene objects
uniform int time = 0;
uniform float atten = 1000;

// color of the void
uniform vec3 voidColor = vec3(0, 0.05, 0.2);

// spheres
uniform int sphereCount = 0;

uniform vec3 spherePositions[maxObjects];

uniform float sphereRadii[maxObjects];
uniform vec3 sphereColors[maxObjects];

// triangles
uniform int triangleCount = 0;

uniform vec3 triP1[maxObjects];
uniform vec3 triP2[maxObjects];
uniform vec3 triP3[maxObjects];

uniform vec3 triangleColors[maxObjects];

// lights
uniform int dirLightCount = 0;
uniform int pointLightCount = 0;

uniform vec3 dirLights[maxLights];
uniform vec3 pointLights[maxLights];

uniform vec3 dirLightsColors[maxLights];
uniform vec3 pointLightsColors[maxLights];

// textures
uniform sampler2D sky;
uniform sampler2D grass;

uniform vec3 grassMean;

uniform bool useTextures;

//// ---------------- ////
//// global variables ////
//// ---------------- ////

// stores the object color and color at each collision point
vec3[maxCastCount] totalColor;

// parameter determines how far the intersection is
float d = -1;

// index determines what object the ray has intersected with
int index = -1;

// how many times the ray has reflected
int casts = 0;

// starting point of ray being traced
vec3 rayOrigin = pos;

// direction of ray being traced
vec3 rayDir = normalize(dir + side * (gl_FragCoord.x * 2 - width) / float(height) + up * (gl_FragCoord.y * 2 - height) / float(height));

// end point of ray being traced
vec3 rayEnd = pos;

// the type of object intersected
int objectType = 0;

//// -------------------------------- ////
//// functions to compute pixel color ////
//// -------------------------------- ////

//// __ blending functions __ ////

// gets the vector with min at each pos
vec3 eltWiseMin(vec3 u, vec3 v) {
  return vec3(min(u.x, v.x), min(u.y, v.y), min(u.z, v.z));
}

//// __ methods for sphere __ ////

// method to get intersection of line and sphere
float getIntersectionSphere(vec3 origin, vec3 dir, int idx) {
  vec3 CO = origin - spherePositions[idx];
  float x = -(dot(dir, CO));
  float y = pow(dot(dir, CO), 2) - pow(length(CO), 2) + sphereRadii[idx] * sphereRadii[idx];

  if (y < 0.0) return -1;

  y = sqrt(y);
  float d1 = x - y;
  if (d1 >= 0.0) return d1;
  else return x + y;
}

// method to get the normal to sphere
vec3 getNormalSphere() {
  vec3 normal = normalize(rayEnd - spherePositions[index]);
  if (dot(normal, rayDir) < 0) return normal;
  return -normal;
}

//// __ methods for triangle __ ////

// method to get intersection of line and triangle
float getIntersectionTriangle(vec3 origin, vec3 dir, int idx) {

  vec3 crossProduct = cross(triP3[idx] - triP1[idx], triP2[idx] - triP1[idx]);
  vec3 normal = normalize(crossProduct);

  float den = dot(dir, normal);
  float s = 0;
  if (den != 0.0) {
  	s = dot(triP1[idx] - origin, normal) / den; 
  }
  else return -1;

  float A = length(crossProduct);
  vec3 x = origin + dir * s;

  float alpha = dot( cross(triP3[idx] - x, triP2[idx] - x), normal) / A;
  float beta = dot( cross(triP1[idx] - x, triP3[idx] - x), normal) / A;
  float gamma = 1 - alpha - beta;

  if (alpha >= 0 && beta >= 0 && gamma >= 0 && alpha <= 1 && beta <= 1 && gamma <= 1) {
    return s;
  }
  return -1;
}

// method to get the normal to triangle
vec3 getNormalTriangle() {
  vec3 normal = normalize( cross(triP3[index] - triP1[index], triP2[index] - triP1[index]) );
  if (dot(normal, rayDir) < 0) return normal;
  return -normal;
}

//// __ methods for floor __ ////

// method to get intersection of line and floor
float getIntersectionFloor(vec3 origin, vec3 dir) {
  if (dir.y == 0) return -1;
  return -origin.y / dir.y;
}

// method to get the normal to floor
vec3 getNormalFloor() {
  if (dot(vec3(0, 1, 0), rayDir) < 0) return vec3(0, 1, 0);
  return vec3(0, -1, 0);
}

// sqaure tile pattern
int squareTiles(float x, float y) {
  float s = 25; int r = 2;
  vec2 pos = vec2(int(x/s) % r, int(y/s) % r);
  int t = 0;

  if ((x < 0) ^ (y < 0)) t++;

  if (pos.x >= 1) t++;
  if (pos.y >= 1) t++;

  if (t % 2 == 0) return 1;
  return 0;
}

// concentric circles pattern
int concentricCircles(float x, float y) {
  if (int(sqrt(x*x + y*y) - time) % 51 > 25) return 1;
  return 0;
}

// get texture
vec3 getFloorColor(float x, float y) {
  float s = 25;

  vec2 pos = vec2(mod(x/s, 1.0), mod(y/s, 1.0));

  float k = 3000.0 / 11.0;

  float f = 1.0 / ( 1.0 + k*exp(-d/k) );

  return texture2D(grass, pos) * (1 - f) + grassMean * f;
}

//// __ methods for void __ ////

// get the sky texture
vec4 getSky() {
  float pi = 3.14159;

  float phi = acos(rayDir.y/length(rayDir));
  float theta = atan2(rayDir.z, rayDir.x);

  vec2 pos = vec2(0.5, 0.5) + 0.99 * vec2(cos(theta), sin(theta)) * phi/pi;

  return texture2D(sky, pos) * dot(normalize(dirLights[0]), vec3(0, -1, 0));
}

//// __ methods for all objects __ ////

// get normal to object at index
vec3 getNormal() {
  if (objectType == SPHERE_OBJ) return getNormalSphere();
  if (objectType == TRIANGLE_OBJ) return getNormalTriangle();
  if (objectType == FLOOR_OBJ) return getNormalFloor();
  return vec3(1, 0, 0);
}

// gets the intersection given the obj type
float getIntersection(vec3 origin, vec3 dir, int idx, int objType) {
  if (objType == SPHERE_OBJ) return getIntersectionSphere(origin, dir, idx);
  if (objType == TRIANGLE_OBJ) return getIntersectionTriangle(origin, dir, idx);
  if (objType == FLOOR_OBJ) return getIntersectionFloor(origin, dir);
  return -1;
}

// checks all objects of a certain type for closest intersection
void checkShape(vec3 origin, vec3 dir, int objType, int objCount) {
  for (int i = 0; i < objCount; i++) {
	float d2 = getIntersection(origin, dir, i, objType);
    if (d2 > 0 && (d2 < d || d < 0)) {
      d = d2;
      index = i;
	  objectType = objType;
    }
  }
}

// method to get closest intersection of a ray of light
int getClosestIntersection(vec3 origin, vec3 dir) {

  origin += dir * 0.01;

  // temp variables
  d = -1;
  index = -1;
  objectType = VOID_OBJ;

  // check spheres
  checkShape(origin, dir, SPHERE_OBJ, sphereCount);
  checkShape(origin, dir, TRIANGLE_OBJ, triangleCount);
  checkShape(origin, dir, FLOOR_OBJ, 1);

  return objectType;
}

// method to get if point in shade
bool getShadow(vec3 origin, vec3 dir, float dist) {

  origin += dir * 0.01;
  float d2 = -1;

  // check spheres
  for (int i = 0; i < sphereCount; i++) {
    d2 = getIntersectionSphere(origin, dir, i);
    if (d2 > 0 && dist < 0) return true;
	if (d2 > 0 && dist > 0 && d2 < dist) return true;
  }

  // check triangle
  for (int i = 0; i < triangleCount; i++) {
    d2 = getIntersectionTriangle(origin, dir, i);
    if (d2 > 0 && dist < 0) return true;
	if (d2 > 0 && dist > 0 && d2 < dist) return true;
  }

  // check floor
  d2 = getIntersectionFloor(origin, dir);
  if (d2 > 0 && dist < 0) return true;
  if (d2 > 0 && dist > 0 && d2 < dist) return true;

  return false;
}

//// -------------------------------------- ////
//// pseudo recursive ray tracing algorithm ////
//// -------------------------------------- ////

bool traceRay() {

  getClosestIntersection(rayOrigin, rayDir);
  rayEnd = rayOrigin + rayDir * d;
  
  vec3 normal = getNormal();

  // if ray goes into void
  if (objectType == VOID_OBJ) {
	totalColor[casts] = voidColor;
    if (useTextures) totalColor[casts] = getSky();
	return false;
  }

  vec3 objColor = 0;
  vec3 colorFromLight = 0;

  // set objColor to the color of the obj hit
  if (objectType == SPHERE_OBJ) objColor = sphereColors[index];
  if (objectType == TRIANGLE_OBJ) objColor = triangleColors[index];
  if (objectType == FLOOR_OBJ) {
	if (squareTiles(rayEnd.x, rayEnd.z) == 0) objColor = vec3(0, 0, 0);
	else objColor = vec3(0.7, 0.7, 0.7);
	if (useTextures) objColor = getFloorColor(rayEnd.x, rayEnd.z);
  }

  // get the light coming from directed lights
  for (int i = 0; i < dirLightCount; i++)
    if ( !getShadow(rayEnd, normalize(-dirLights[i]), -1) ) colorFromLight += eltWiseMin(objColor, dirLightsColors[i]) * max(0, dot(normal, normalize(-dirLights[i])));

  // get the light coming from point lights
  for (int i = 0; i < pointLightCount; i++) {
    vec3 diff = pointLights[i]-rayEnd;
	float mag = length(diff);
    if ( !getShadow(rayEnd, normalize(diff), mag) ) colorFromLight += eltWiseMin(objColor, pointLightsColors[i]) * max(0, dot(normal, normalize(diff)) * atten / (mag * mag));
  }

  // add up the direct color at the point
  totalColor[casts] = colorFromLight + objColor * diffuse;

  //if (objectType == FLOOR_OBJ) return false; // floor non reflective

  // update the direction and the origin of the ray
  rayDir = rayDir - normal * 2 * dot(normal , rayDir);
  rayOrigin = rayEnd;

  return ++casts < maxCastCount;
}

//// ---------------------------------- ////
//// loop to get the color of the pixel ////
//// ---------------------------------- ////

vec3 collapseRays() {

  // for each bounce of the ray
  for (int i = casts; i > 0; i--) {
    totalColor[i - 1] += totalColor[i] * reflectivity;
  }
  return totalColor[0];
}

//// --------------------------------- ////
//// main method to get color of pixel ////
//// --------------------------------- ////

void main() {

  // while the ray is reflecting
  while (casts < maxCastCount && traceRay()) {}

  // set the color of the pixel to color
  gl_FragColor = vec4(collapseRays(), 1);
}