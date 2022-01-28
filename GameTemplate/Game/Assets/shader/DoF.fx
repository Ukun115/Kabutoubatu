/*!
 * @brief 被写界深度
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;       // MVP行列
    float4 mulColor;    // 乗算カラー
};

//シーンにDoFをかけるかどうかフラグを持ってくるための定数バッファ
cbuffer LightCb : register(b1)
{
    int getCanDoF;
}

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};


//頂点シェーダー
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

//ボケ画像と深度テクスチャにアクセスするための変数を追加
Texture2D<float4> bokeTexture : register(t0);  // ボケ画像
Texture2D<float4> depthTexture : register(t1); // 深度テクスチャ

sampler Sampler : register(s0);     //サンプラーステート

/////////////////////////////////////////////////////////
// ボケ画像書き込み用
/////////////////////////////////////////////////////////

float4 PSMain(PSInput In) : SV_Target0
{
    //ボケ画像書き込み用のピクセルシェーダーを実装
    //カメラ空間での深度値をサンプリング
    float depth = depthTexture.Sample(Sampler, In.uv);

    //カメラ空間での深度値が3000.0f以下ならボケ画像を書き込まないようにピクセルキル
    clip(depth - 3000.0f);

    // ボケ画像をサンプリング
    float4 boke = bokeTexture.Sample(Sampler, In.uv);

    //深度値から不透明度を計算する
    //深度値3000.0fからボケが始まり、深度値5000.0fで最大のボケ具合になる
    //つまり、深度値5000で不透明度が1になる
    bool canDoF = getCanDoF;
    if (canDoF)
    {
        boke.a = min(1.0f, (depth - 3000.0f) / 5000.0f);
    }
    else
    {
        boke.a = 0.0f;
    }

    //ボケ画像を出力
    return boke;
}
