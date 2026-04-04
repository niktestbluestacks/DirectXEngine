// Defines

#define NUM_LIGHTS 4

// Globals

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightColorbuffer {
    float4 diffuseColor[NUM_LIGHTS];
};

// Typedefs

struct PixelInputType {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightPos[NUM_LIGHTS] : TEXCOORD1;
};

// Pixel Shader

float4 LightPixelShader(PixelInputType input) : SV_TARGET {
    float4 textureColor;
    float lightIntensity[NUM_LIGHTS];
    float4 colorArray[NUM_LIGHTS];
    float4 colorSum;
    float4 color;
    int i;
    
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    for (i = 0; i < NUM_LIGHTS; ++i) {
        lightIntensity[i] = saturate(dot(input.normal, input.lightPos[i]));
        colorArray[i] = diffuseColor[i] * lightIntensity[i];
    }
    
    colorSum = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    for (i = 0; i < NUM_LIGHTS; ++i) {
        colorSum.r += colorArray[i].r;
        colorSum.g += colorArray[i].g;
        colorSum.b += colorArray[i].b;
    }

    color = saturate(colorSum) * textureColor;
    
    return color;
}