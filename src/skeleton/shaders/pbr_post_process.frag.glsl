#version 450

layout (location = 0) in vec2 v_tex;

layout (location = 0) out vec4 o_color;

layout (binding = 0) uniform sampler2D u_frame_buffer;

void main() {
  o_color = texture(u_frame_buffer, v_tex);
}
