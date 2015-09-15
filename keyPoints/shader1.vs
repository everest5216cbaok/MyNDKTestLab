attribute mediump vec3 Position;
attribute mediump vec2 TexCoord;

varying highp vec2 TexCoord0;
varying highp vec3 position;

uniform highp mat4 gWVP;

void main()
{
    gl_Position = vec4(Position, 1.0);
    position = vec3(Position.xy, 1.0);
    TexCoord0 = TexCoord;
    position.xy = (position.xy + 1.0) / 2.0;
}