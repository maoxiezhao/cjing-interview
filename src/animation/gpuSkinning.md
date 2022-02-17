# GPU skinning
待补全...
```glsl
#include "..\hf\global.hlsli"

struct BonePose
{
    float4 bonePose0;
    float4 bonePose1;
    float4 bonePose2;
};
STRUCTUREDBUFFER(BonePoseBuffer, BonePose, SKINNING_SBSLOT_BONE_POSE_BUFFER);

RAWBUFFER(vertexBufferPosNormal, SKINNING_SLOT_VERTEX_POS);
RAWBUFFER(vertexBufferBoneIndexWeight, SKINNING_SLOT_VERTEX_BONE_INDEX_WEIGHT);
RWRAWBUFFER(streamoutBufferPos, 0);

inline void Skinning(inout float3 pos, inout float3 nor, in float4 boneIndices, in float4 boneWeights)
{
    // m = inverseBindM * inverseArmatureM * nodeTransM
    // 顶点*inverseBindM首先得到绑定空间下的顶点
    // (inverseArmatureM * nodeTransM)会得到顶点的局部空间变化
    // 在对顶点所有受影响的骨骼做加权求和
    
    float4 newPos = 0.0f;
    float3 newNor = 0.0f;
    float weightSum = 0.0f;
    [loop]
    for (int i = 0; (i < 4) && (weightSum < 1.0f); i++)
    {
        BonePose bonePose = BonePoseBuffer[(uint) boneIndices[i]];
        float4x4 m =
        {
            bonePose.bonePose0,
            bonePose.bonePose1,
            bonePose.bonePose2,
            float4(0, 0, 0, 1)
        };
        
        newPos += mul(m, float4(pos.xyz, 1.0f)) * boneWeights[i];
        newNor += mul((float3x3)m, nor) * boneWeights[i];
        
        weightSum += boneWeights[i];
    }
    
    pos.xyz = newPos.xyz;
    nor = normalize(newNor);
}

static const uint stridePosNor = 16; // float4
static const uint strideBoneWeight = 16;

float4 GetBoneIndexs(uint2 value)
{
    float4 boneIndexs = 0.0f;
    boneIndexs.x = (float) ((value.x >> 0) & 0xffff);
    boneIndexs.y = (float) ((value.x >> 16) & 0xffff);
    boneIndexs.z = (float) ((value.y >> 0) & 0xffff);
    boneIndexs.w = (float) ((value.y >> 16) & 0xffff);
    return boneIndexs;
}

float4 GetBoneWeights(uint2 value)
{
    float4 boneWeights = 0.0f;
    boneWeights.x = (float) ((value.x >> 0)  & 0xffff) / 65535.0f;
    boneWeights.y = (float) ((value.x >> 16) & 0xffff) / 65535.0f;
    boneWeights.z = (float) ((value.y >> 0)  & 0xffff) / 65535.0f;
    boneWeights.w = (float) ((value.y >> 16) & 0xffff) / 65535.0f;
    return boneWeights;
}

[numthreads(SHADER_SKINNING_BLOCKSIZE, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    const uint posNormalAddress = DTid.x * stridePosNor;
    const uint bondWeightAddress = DTid.x * strideBoneWeight;
    
    // load position and normal
    uint4 posNormal = vertexBufferPosNormal.Load4(posNormalAddress);
    float3 pos = asfloat(posNormal.xyz);
    float3 nor = GetNormalFromNumber(posNormal.w);

    // load bone indices and weights
    uint4 boneIndicesWeight = vertexBufferBoneIndexWeight.Load4(bondWeightAddress);
    float4 boneIndices = GetBoneIndexs(boneIndicesWeight.xy);
    float4 bondWeights = GetBoneWeights(boneIndicesWeight.zw);
    
    Skinning(pos, nor, boneIndices, bondWeights);
    
    // 转换成uint4写入rawbuffer
    posNormal.xyz = asuint(pos.xyz);
    posNormal.w = SetNormalIntoNumber(nor);
    streamoutBufferPos.Store4(posNormalAddress, posNormal);
}
```