#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float size = 2200;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in Vertex {
    vec2 texCoord;
} IN [];

out Vertex {
    vec2 texCoord;
} OUT ;

const vec2 tc     [4] =
  vec2 [] ( vec2 (0.0, 0.0),
            vec2 (1.0, 0.0),
            vec2 (1.0, 1.0),
            vec2 (0.0, 1.0) );

void main () {
    

    for (int i = 0; i < gl_in.length (); ++i) {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += size ;
        gl_Position.y += size;
        OUT.texCoord   = vec2 (0.0, 0.0);
        EmitVertex   ();

        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= size;
        gl_Position.y += size;
        OUT.texCoord   =  vec2 (1.0, 0.0);
        EmitVertex   ();

        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += size;
        gl_Position.y -= size;
        OUT.texCoord   =  vec2 (0.0, 1.0);
        EmitVertex   ();

        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= size ;
        gl_Position.y -= size;
        OUT.texCoord   = vec2 (1.0, 1.0);
        EmitVertex   ();
    }
    EndPrimitive ();
}