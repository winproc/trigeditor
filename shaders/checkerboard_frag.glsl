#version 450
out vec4 fragColor;

in vec2 fragCoord;

void main() {
	const float scale = 1.0;
	vec2 scaled_fragCoord = floor((fragCoord*100)/scale);
	vec3 col = mod(scaled_fragCoord.xxx + scaled_fragCoord.y, 2.0); 
	fragColor = vec4(col, 1.0);
}