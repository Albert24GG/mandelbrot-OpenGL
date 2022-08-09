#version 330 core

out vec4 FragColor;
in vec4 gl_FragCoord;

uniform vec4 color;
uniform vec2 windowResolution;
uniform float time;
uniform vec4 coordRange;
uniform vec2 off;
uniform float zoom;
 
const int maxIterations = 5000;

precision highp float;

// vec2(x, y) are the coordinates -> x + y * i is the complex representation
vec2 squareImaginary(vec2 number){
	
	return vec2(
		number.x * number.x - number.y * number.y,
		2 * number.x * number.y
	);

}


float iterateMandelbrot(vec2 coords){
	vec2 z = vec2(0, 0);

	for(int i=1; i<maxIterations; ++i){
		z = squareImaginary(z) + coords;
		
		// Verify the absolute value of z
		if(length(z) > 2){
			return float(i) / 10;
		}
	}
	return 0.0;
}

/*
//vec4 coordinateRange represents (Xmin,Xmax,Ymin,Ymax) we want from our window
vec2 getCoordinatesFromScreen(vec2 fragCoords, vec2 windowResolution, vec4 coordinateRange){
	vec2 normalizedCoordinates = fragCoords.xy / windowResolution; // relative position of the fragment, from 0 to 1

	float horizontalSize = coordinateRange.y - coordinateRange.x; // Size of horizontal part of the window
	float verticalSize = coordinateRange.w - coordinateRange.z; // Size of vertical part of the window
	
	// Multiply by size and add initial offset position to get the normalized position
	normalizedCoordinates.x = normalizedCoordinates.x * horizontalSize + coordinateRange.x; 
	normalizedCoordinates.y = normalizedCoordinates.y * verticalSize + coordinateRange.z;

	return normalizedCoordinates;
}
*/

vec2 fragNormalizeCoords(vec2 fragCoords, vec2 initialAxisLen){
	return vec2(
		 (fragCoords.x / windowResolution.x - 0.5) * (initialAxisLen.x / zoom) + off.x,
		 (fragCoords.y / windowResolution.y - 0.5) * (initialAxisLen.y / zoom) + off.y
	);
}

void main(){
	float aspectRatio = windowResolution.x / windowResolution.y;
	
	vec2 fragNormalizedCoords = fragNormalizeCoords(gl_FragCoord.xy, vec2(4 * aspectRatio, 4));

	float shade = iterateMandelbrot(fragNormalizedCoords) * 0.25;
	FragColor = vec4(shade);
	//if(fragNormalizedCoords.x < 0 && fragNormalizedCoords.y < 0)
	//	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
