#version 330 core

// TODO: uniforms w/ customizability
const vec4 c_no_data = vec4(0.15f, 0.15f, 0.15f, 1.0f); // dark grey
const vec4 c_min = vec4(0.0f, 0.0f, 0.0f, 1.0f);        // black
const vec4 c_max = vec4(1.0f, 1.0f, 1.0f, 1.0f);        // white

in VS_OUT {
    vec2 uv;
} i;

uniform vec2 u_resolution;
uniform bool u_ld_enabled;
uniform bool u_st_enabled;
uniform bool u_show_grid_blocks;
uniform vec3 u_block_size;
uniform vec3 u_max_block_index;
uniform float u_grid_block_opacity;
uniform sampler2D u_data; // GL_FLOAT => r = ld, g = st, b = euuugh

void main() {
    vec2 position = gl_FragCoord.xy / u_resolution; // global position [x, y] of the current fragment

    // lots of branching here but this shader doesn't need to be terribly efficient
    float st = u_st_enabled ? texture2D(u_data, position).r : 0.0f;
    float ld = u_ld_enabled ? texture2D(u_data, position).g : 0.0f;
    float mix_factor = (ld + st) / (0.5f * (u_block_size.x + u_block_size.y)) / max(1.0f, max(u_max_block_index.x + 1, u_max_block_index.y + 1));
    vec4 c_out = mix(c_min, c_max, mix_factor);

    if (u_show_grid_blocks) {
        // block the current fragment belongs to
        vec4 block = vec4(floor(gl_FragCoord.x / u_block_size.x), floor(gl_FragCoord.y / u_block_size.y), 0.5f, 1.0f);
        // mix block color onto out color
        vec4 block_color = vec4(block.x / (u_max_block_index.x + 1), block.y / (u_max_block_index.y + 1), block.z, 1.0f);
        c_out = mix(c_out, block_color, u_grid_block_opacity);
    }

    // deprecated shmeprecated
    gl_FragColor = vec4(ld == 0.0f && st == 0.0f ? c_no_data.xyz : c_out.xyz, 1.0f);
}