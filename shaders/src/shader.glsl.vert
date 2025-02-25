#version 460

layout(set=1, binding=0) uniform UBO {
	mat4 mvp;
};

void main() {
	if (gl_VertexIndex == 0) {
		gl_Position = vec4(-0.5, -0.5, 0, 1);
	} else  if (gl_VertexIndex == 1) {
		gl_Position = vec4(0, 0.5, 0, 1);
	} else if (gl_VertexIndex == 2) {
		gl_Position = vec4(0.5, -0.5, 0, 1);
	}
	gl_Position = mvp * gl_Position;
}