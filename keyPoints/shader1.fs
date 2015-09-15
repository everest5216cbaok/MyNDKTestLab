precision mediump float;

varying highp vec2 TexCoord0;
varying highp vec3 position;

uniform sampler2D gSampler;
uniform sampler2D gSampler2;

uniform highp mat3 originMatrix;
uniform highp mat3 targetMatrix;
uniform highp float originWeight;

void main()
{
    highp vec3 originPosition = vec3(position.xy, 1.0) * originMatrix;
    highp vec3 targetPosition = vec3(position.xy, 1.0) * targetMatrix;

    originPosition.y = 1.0 - originPosition.y;
    targetPosition.y = 1.0 - targetPosition.y;

    highp vec4 color1 = texture2D(gSampler, originPosition.xy);
    highp vec4 color2 = texture2D(gSampler2, targetPosition.xy);
    gl_FragColor = color1 * originWeight + color2 * (1.0 - originWeight);
    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}