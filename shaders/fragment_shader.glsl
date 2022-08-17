#version 460 core

out vec4 FragColor;
in vec4 gl_FragCoord;

uniform uvec2 windowResolution;
uniform float time;
uniform dvec2 off;
uniform double zoom;
uniform unsigned int maxIterations;


dvec2 squareImaginary(dvec2 number){
	
	return dvec2(
		number.x * number.x - number.y * number.y,
		2 * number.x * number.y
	);

}


// dvec2(x, y) are the coordinates -> x + y * i is the complex representation 
int iterateMandelbrot(dvec2 coords){
	dvec2 z1 = dvec2(0);
	dvec2 z2 = dvec2(0);
	int iteration = 0;
	while(z2.x + z2.y <= 4 && iteration < maxIterations){
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


vec3 getColor(int iterations){
	if(iterations == 0 || iterations == maxIterations) 
		return vec3(0);
	switch(iterations % 16){
		case 0:
			return vec3(66, 30, 15);
			break;
		case 1:
			return vec3(25, 7, 26);
			break;
		case 2:
			return vec3(9, 1, 47);
			break;
		case 3:
			return vec3(4, 4, 73);
			break;
		case 4:
			return vec3(0, 7, 100);
			break;
		case 5:
			return vec3(12, 44, 138);
			break;
		case 6:
			return vec3(24, 82, 177);
			break;
		case 7:
			return vec3(57, 125, 209);
			break;
		case 8:
			return vec3(134, 181, 229);
			break;
		case 9:
			return vec3(211, 236, 248);
			break;
		case 10:
			return vec3(241, 233, 191);
			break;
		case 11:
			return vec3(248, 201, 95);
			break;
		case 12:
			return vec3(255, 170, 0);
			break;
		case 13:
			return vec3(204, 128, 0);
			break;
		case 14:
			return vec3(153, 87, 0);
			break;
		case 15:
			return vec3(106, 52, 3);
			break;
	}
}


void main(){
	
	float aspectRatio = 1.0 * windowResolution.x / windowResolution.y;
	
	dvec2 fragNormalizedCoords = fragNormalizeCoords(gl_FragCoord.xy, dvec2(4 * aspectRatio, 4));

	int iterations = iterateMandelbrot(fragNormalizedCoords);
	
	double t = double(iterations) / double(maxIterations);
	//FragColor = vec4(getColor(iterations) / 255, 1.0);
	FragColor = map_to_color(float(t));
}
