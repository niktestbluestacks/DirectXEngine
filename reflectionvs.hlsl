// Globals

cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ReflectionBuffer {
    matrix reflectionMatrix;
};

// Typedefs

struct VertexInputType {
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};

PixelInputType ReflectionVertexShader(VertexInputType input) {
    PixelInputType output;
    
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input.tex;

    // Reflection path: world → reflected view → projection
    float4 worldPos = mul(input.position, worldMatrix);
    output.reflectionPosition = mul(worldPos, reflectionMatrix);
    output.reflectionPosition = mul(output.reflectionPosition, projectionMatrix);

    return output;
}