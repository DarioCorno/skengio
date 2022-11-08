#version 330 core
uniform sampler2D depthTexture; // depth buffer
uniform sampler2D diffuseTexture; // render buffer
uniform float nearPlane;
uniform float farPlane;
uniform vec3 camViewPos;
uniform vec3 camTargetPos;
uniform float camFOV;

//deeply inspired by https://gist.github.com/slarson/6c3be61a2f7dd85cb7ef8df28276976a

in vec2 TexCoords; // uv coords
in vec2 pos;
out vec4 color;

#define PI 3.1415926

float znear = nearPlane; // camera clipping near plane
float zfar = farPlane; // camera clipping far plane
vec2 iResolution = textureSize(diffuseTexture, 0); //vec2(1024, 768); // screen resolution
float focalLength = camFOV; // camera focal length
float focalDepth = distance(camViewPos,camTargetPos); // camera focal depth
float fstop = 0.7; // camera fstop
float dithering = 0.0001; // amount of dithering
float maxblur = 2.0; // maximum amount of blur
float threshold = 2.0; // highlight threshold;
float gain = 0.0; // highlight gain;
float bias = 0.0; // bokeh edge bias
float fringe = 0.0; // bokeh chromatic aberration / fringing,

vec2 vUv = TexCoords;

// constants TODO should be const-qualified
vec2 texel = vec2(1.0/iResolution.x,1.0/iResolution.y);
float dbsize = 1.25; // depth blur size
const float CoC = 0.03; //circle of confusion size in mm (35mm film = 0.03mm)
const int rings = 3;
const int samples = 4;
const int maxringsamples = rings * samples;


// generating noise / pattern texture for dithering
vec2 rand(vec2 coord) {

	float noiseX = ((fract(1.0-coord.s*(iResolution.x/2.0))*0.25)+(fract(coord.t*(iResolution.y/2.0))*0.75))*2.0-1.0;
	float noiseY = ((fract(1.0-coord.s*(iResolution.x/2.0))*0.75)+(fract(coord.t*(iResolution.y/2.0))*0.25))*2.0-1.0;

	// if (noise) {
	// 	noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
	// 	noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
	// }

	return vec2(noiseX,noiseY);
}

// Depth buffer blur
// calculate the depth from a given set of coordinates
float bdepth(vec2 coords) {
	float d = 0.0, kernel[9];
	vec2 offset[9], wh = vec2(texel.x, texel.y) * dbsize;

	offset[0] = vec2(-wh.x,-wh.y);
	offset[1] = vec2( 0.0, -wh.y);
	offset[2] = vec2( wh.x -wh.y);

	offset[3] = vec2(-wh.x,  0.0);
	offset[4] = vec2( 0.0,   0.0);
	offset[5] = vec2( wh.x,  0.0);

	offset[6] = vec2(-wh.x, wh.y);
	offset[7] = vec2( 0.0,  wh.y);
	offset[8] = vec2( wh.x, wh.y);

	kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;
	kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;
	kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;


	for( int i=0; i<9; i++ ) {
		float tmp = texture2D(depthTexture, coords + offset[i]).r;
		d += tmp * kernel[i];
	}

	return d;
}

// processing the sample
vec3 calcColor(vec2 coords,float blur) {
	vec3 col = vec3(0.0);

	// read from the render buffer at an offset
	col.r = texture2D(diffuseTexture,coords + vec2(0.0,1.0)*texel*fringe*blur).r;
	col.g = texture2D(diffuseTexture,coords + vec2(-0.866,-0.5)*texel*fringe*blur).g;
	col.b = texture2D(diffuseTexture,coords + vec2(0.866,-0.5)*texel*fringe*blur).b;

	vec3 lumcoeff = vec3(0.299,0.587,0.114); // arbitrary numbers???
	float lum = dot(col.rgb, lumcoeff);
	float thresh = max((lum-threshold)*gain, 0.0);
	return col+mix(vec3(0.0),col,thresh*blur);
}

float gather(float i, float j, int ringsamples, inout vec3 col, float w, float h, float blur) {
	float rings2 = float(rings);
	float step = PI*2.0 / float(ringsamples);
	float pw = cos(j*step)*i;
	float ph = sin(j*step)*i;
	float p = 1.0;
	col += calcColor(vUv.xy + vec2(pw*w,ph*h), blur) * mix(1.0, i/rings2, bias) * p;
	return 1.0 * mix(1.0, i /rings2, bias) * p;
}

float linearize(float depth) {
	return -zfar * znear / (depth * (zfar - znear) - zfar);
}

void main(void)
{
	float depth = linearize(bdepth(vUv.xy));

	float f = focalLength; // focal length in mm,
	float d = focalDepth*1000.0; // focal plane in mm,
	float o = depth*1000.0; // depth in mm,

	float a = (o*f)/(o-f);
	float b = (d*f)/(d-f);
	float c = (d-f)/(d*fstop*CoC);

	float blur = clamp(abs(a-b)*c,0.0,1.0);

	// calculation of pattern for dithering
	vec2 noise = rand(vUv.xy)*dithering*blur;

	// getting blur x and y step factor
	float w = (1.0/iResolution.x)*blur*maxblur+noise.x;
	float h = (1.0/iResolution.y)*blur*maxblur+noise.y;

	// calculation of final color,
	vec3 col = texture2D(diffuseTexture, vUv.xy).rgb;

	if ( blur >= 0.05 ) {
		float s = 1.0;
		int ringsamples;

		for (int i = 1; i <= rings; i++) {
			ringsamples = i * samples;

			for (int j = 0 ; j < maxringsamples ; j++) {
				if (j >= ringsamples) break;
				s += gather(float(i), float(j), ringsamples, col, w, h, blur);
			}
		}
		col /= s; //divide by sample count
	}

	color = vec4(col,1.0);
}