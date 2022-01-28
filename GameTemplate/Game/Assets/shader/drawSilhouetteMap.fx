/*!
 * @brief シルエットマップ描画用のシェーダー
 */


// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//モデルのモードの定数バッファ
cbuffer ModelMode : register(b1)
{
    int  playerMode;	//プレイヤーのモード(0は赤。１は青。２は黒)

    int outLineFlg;	//輪郭線をつけるかどうかのフラグ

    int silhouetteFlg;//シルエットをつけるかどうかのフラグ
}

 //スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
    int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos          : POSITION;     //モデルの頂点座標
    float3 normal       : NORMAL;       //法線
    float2 uv           : TEXCOORD0;    //UV座標
    SSkinVSIn skinVert;		            //スキン用のデータ。
};
// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos          : SV_POSITION;   //スクリーン空間でのピクセルの座標
    float3 normal       : NORMAL;        //法線
    float2 uv           : TEXCOORD0;     //uv座標
    float3 worldPos		: TEXCOORD1;     //ワールド座標
    float3 normalInView : TEXCOORD2;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo              : register(t0);     // アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	    //ボーン行列
sampler g_sampler                       : register(s0);     // サンプラーステート

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

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    //スキンありメッシュの時、
    if (hasSkin)
    {
        //スキン行列を計算
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    //スキン無しメッシュの時、
    else
    {
        m = mWorld;
    }
    psIn.pos = mul(m, vsIn.pos);

    psIn.worldPos = psIn.pos;

    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = mul(m, vsIn.normal);
    psIn.normal = normalize(psIn.normal);

    psIn.normalInView = mul(mView, psIn.normal);

    psIn.uv = vsIn.uv;

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// シルエットマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //シルエットを落としているプレイヤーの番号をmodelシェーダーに渡す
    int m_player = 0.0f;
    switch (playerMode)
    {
    case 0:
        m_player = 0.0f;
        break;
    case 1:
        m_player = 1.0f;
        break;
    case 2:
        m_player = 2.0f;
        break;
    }
    //シルエットマップ描画用のピクセルシェーダーを実装
    return float4(psIn.pos.z, m_player, 0.0f, 1.0f);
}
