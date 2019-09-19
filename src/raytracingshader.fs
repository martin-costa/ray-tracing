
//// ------------------ ////
//// constant variables ////
//// ------------------ ////

const int maxObjects = 130;
const int maxCastCount = 6;

// the types of scene objects [VOID: -1, SPHERE: 0, FLOOR: 1, TRIANGLE: 2]
const int VOID_OBJ = -1, SPHERE_OBJ = 0, FLOOR_OBJ = 1, TRIANGLE_OBJ = 2;

//// ----------------- ////
//// uniform variables ////
//// ----------------- ////

// width and height
uniform int width;
uniform int height;

uniform float reflectivity = 0.6;
uniform float diffuse = 0.2;

// camera position and orientation
uniform vec3 pos;

uniform vec3 dir;
uniform vec3 up;
uniform vec3 side;

// the data for the scene objects

// color of the void
uniform vec3 voidColor = vec3(0, 0, 0.07);

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

//// ---------------- ////
//// global variables ////
//// ---------------- ////

// stores the object color and color at each collision point
vec3[maxCastCount] colorOfObject;
vec3[maxCastCount] colorFromLight;
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
vec3 rayDir = normalize(dir + side * (gl_FragCoord.x - float(width) / 2.0) * 2.0 / float(height) + up * (gl_FragCoord.y - float(height) / 2.0) * 2.0 / float(height));

// end point of ray being traced
vec3 rayEnd = pos;

// the type of object intersected
int objectType = 0;

//// -------------------------------- ////
//// functions to compute pixel color ////
//// -------------------------------- ////

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
  return normalize(rayEnd - spherePositions[index]);
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
  return -origin.y / dir.y;
}

// method to get the normal to sphere
vec3 getNormalFloor() {
  return vec3(0, 1, 0);
}

//// __ methods for all objects __ ////

// get normal to object at index
vec3 getNormal() {
  if (objectType == SPHERE_OBJ) return getNormalSphere();
  if (objectType == TRIANGLE_OBJ) return getNormalTriangle();
  if (objectType == FLOOR_OBJ) return getNormalFloor();

  return vec3(1, 0, 0);
}

// method to get closest intersection of a ray of light
int getClosestIntersection(vec3 origin, vec3 dir) {

  origin += dir * 0.01;

  // temp variables
  d = -1;
  index = -1;
  objectType = VOID_OBJ;

  // check spheres
  for (int i = 0; i < sphereCount; i++) {
	float d2 = getIntersectionSphere(origin, dir, i);
    if (d2 > 0 && (d2 < d || d < 0)) {
      d = d2;
      index = i;
	  objectType = SPHERE_OBJ;
    }
  }

  // check triangle
  for (int i = 0; i < triangleCount; i++) {
	float d2 = getIntersectionTriangle(origin, dir, i);
    if (d2 > 0 && (d2 < d || d < 0)) {
      d = d2;
      index = i;
	  objectType = TRIANGLE_OBJ;
    }
  }

  // check floor
  float d2 = getIntersectionFloor(origin, dir);
  if (d2 > 0 && (d2 < d || d < 0)) {
    d = d2;
	objectType = FLOOR_OBJ;
  }

  return objectType;
}

// method to get if point in shade
bool getShadow(vec3 origin, vec3 dir) {
  return getClosestIntersection(origin, dir) != VOID_OBJ;
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
	return false;
  }

  // if a sphere is hit by the ray
  else if (objectType == SPHERE_OBJ) {
    colorFromLight[casts] = sphereColors[index] * max(0, dot(normal, normalize(vec3(0, 1, 0))));
	colorOfObject[casts] = sphereColors[index];
  }

  // if a triangle is hit by the ray
  else if (objectType == TRIANGLE_OBJ) {
    colorFromLight[casts] = triangleColors[index] * max(0, dot(normal, normalize(vec3(0, 1, 0))));
	colorOfObject[casts] = triangleColors[index];
  }
  
  // if the floor is hit by the ray
  else if (objectType == FLOOR_OBJ) {
    vec2 tile = vec2(int(rayEnd.x) % 51, int(rayEnd.z) % 51);
    int t = 0;

    if (tile.x >= 25) t++;
    if (tile.y >= 25) t++;
    if (t % 2 == 0) {
      colorFromLight[casts] = vec3(0.05, 0.05, 0.05);
	  colorOfObject[casts] = vec3(0, 0, 0);
	}
    else {
      colorFromLight[casts] = vec3(0.75, 0.75, 0.75);
	  colorOfObject[casts] = vec3(1, 1, 1);
	}
  }

  // get if object is in the shade
  if ( getShadow(rayEnd, normalize( vec3(0, pos.y, 0) )) ) colorFromLight[casts] = vec3(0, 0, 0);

  // add up the direct color at the point
  totalColor[casts] = colorFromLight[casts] + colorOfObject[casts] * diffuse;

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
  for (int i = casts - 1; i > 0; i--) {
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