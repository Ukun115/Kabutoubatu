///////////////////////////////////////
// 構造体。
///////////////////////////////////////

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//モデルの法線ベクトル。
	float3 tangent  : TANGENT;		//接ベクトル
	float3 biNormal : BINORMAL;		//従法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
// ピクセルシェーダーへの入力
struct SPSIn
{
	float4 pos : SV_POSITION;       //座標。
	float3 normal : NORMAL;         //法線。
	float3 tangent  : TANGENT;      //接ベクトル。
	float3 biNormal : BINORMAL;     //従ベクトル。
	float2 uv : TEXCOORD0;          //UV座標。
	float3 worldPos : TEXCOORD1;    // ワールド座標
	float4 posInLVP		: TEXCOORD2;
};


//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer SkyCubeCb : register(b1)
{
    float luminance;	// 明るさ。
};

///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
TextureCube<float4> g_skyCubeMap : register(t10);	//スカイキューブマップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

	return skinning;
}

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normal.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	//ピクセルシェーダに渡すデータ
	SPSIn psIn;

	//ワールド行列
	float4x4 m;

	if (hasSkin)
	{
		//スキンあり
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else
	{
		//スキンなし
		m = mWorld;
	}

	//オブジェクト座標をワールド座標に変換
	psIn.pos = mul(m, vsIn.pos);
	//ワールド座標を保持しておく
	psIn.worldPos = psIn.pos;
	float4 worldPos = psIn.pos;
	//ワールド座標をビュー座標に変換
	psIn.pos = mul(mView, psIn.pos);
	//ビュー座標をプロジェクション座標に変換
	psIn.pos = mul(mProj, psIn.pos);

	//頂点法線
	psIn.normal = normalize(mul(m, vsIn.normal));
	//接ベクトル
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	//従法線
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	//UV
	psIn.uv = vsIn.uv;

	return psIn;
}
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float4 albedoColor;
	float3 normal = normalize(psIn.normal);
	//albedoColor = g_skyCubeMap.Sample(g_sampler, psIn.normal);
	albedoColor = g_skyCubeMap.Sample(g_sampler, normal * -1.0f) * luminance;
	return albedoColor;
}
