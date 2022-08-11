#version 400 core

out vec4 FragColor;
in vec4 gl_FragCoord;

uniform uvec2 windowResolution;
uniform float time;
uniform dvec2 off;
uniform double zoom;
 
const int maxIterations = 1000;


dvec2 squareImaginary(dvec2 number){
	
	return dvec2(
		number.x * number.x - number.y * number.y,
		2 * number.x * number.y
	);

}


// dvec2(x, y) are the coordinates -> x + y * i is the complex representation 
float iterateMandelbrot(dvec2 coords){
	dvec2 z1 = dvec2(0);
	dvec2 z2 = dvec2(0);
	int iteration = 0;
	
	while(z2.x + z2.y <= 4 && iteration < maxIterations){
		z1.y = 2 * z1.x * z1.y + coords.y;
		z1.x = z2.x - z2.y + coords.x;
		z2 = z1 * z1;
		++iteration;
	}
	iteration = int(iteration != maxIterations) * iteration;
	return float(iteration) / 10;
}


dvec2 fragNormalizeCoords(dvec2 fragCoords, dvec2 initialAxisLen){
	return dvec2(
		 (fragCoords.x / windowResolution.x - 0.5) * (initialAxisLen.x / zoom) + off.x,
		 (fragCoords.y / windowResolution.y - 0.5) * (initialAxisLen.y / zoom) + off.y
	);
}


void main(){
	float aspectRatio = 1.0 * windowResolution.x / windowResolution.y;
	
	dvec2 fragNormalizedCoords = fragNormalizeCoords(gl_FragCoord.xy, dvec2(4 * aspectRatio, 4));

	float shade = iterateMandelbrot(fragNormalizedCoords) * 0.25;
	FragColor = vec4(shade);
}
