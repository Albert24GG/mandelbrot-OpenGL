#version 460 core

out vec4 FragColor;
in vec4 gl_FragCoord;

uniform uvec2 windowResolution;
uniform dvec2 off;
uniform double zoom;
uniform uint maxIterations;


// dvec2(x, y) are the coordinates -> x + y * i is the complex representation 
int iterateMandelbrot(dvec2 coords){
	dvec2 z1 = dvec2(0);
	dvec2 z2 = dvec2(0);
	int iteration = 0;
	while(dot(z1, z1) <= 4 && iteration < maxIterations){
		z1.y = 2 * z1.x * z1.y + coords.y;
		z1.x = z2.x - z2.y + coords.x;
		z2 = z1 * z1;
		++iteration;
	}
	return iteration;
}


dvec2 fragNormalizeCoords(dvec2 fragCoords, dvec2 initialAxisLen){
	return dvec2(
		 (fragCoords.x / windowResolution.x - 0.5) * (initialAxisLen.x / zoom) + off.x,
		 (fragCoords.y / windowResolution.y - 0.5) * (initialAxisLen.y / zoom) + off.y
	);
}


vec4 map_to_color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}


void main(){
	
	float aspectRatio = float(windowResolution.x) / windowResolution.y;
	
	dvec2 fragNormalizedCoords = fragNormalizeCoords(gl_FragCoord.xy, dvec2(4 * aspectRatio, 4));

	int iterations = iterateMandelbrot(fragNormalizedCoords);
	
	float ratio = float(iterations) / maxIterations;
	
	FragColor = map_to_color(ratio);
}
