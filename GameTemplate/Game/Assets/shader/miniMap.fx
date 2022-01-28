/*!
 * @brief	ミニマップシェーダー
 */
 ///////////////////////////////////////////////
 //定数
 ///////////////////////////////////////////////

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn
{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};

//ピクセルシェーダーへの入力。
struct SPSIn
{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos 	: TEXCOORD1;
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線

	float depthInView	: TEXCOORD3;	// カメラ空間でのZ値

	float4 posInProj	: TEXCOORD4;	//頂点の正規化スクリーン座標系の座標
};

//ピクセルシェーダーからの出力構造体
struct SPSOut
{
	float4 color : SV_Target0;	// レンダリングターゲット0に描きこむ
	float depth  : SV_Target1;	// レンダリングターゲット1に描きこむ
};

//ディレクションライト
struct DirectionLight
{
	float3 direction;	//方向
	float3 color;		//色
};

//ポイントライト
struct PointLight
{
	float3 position;	//位置
	float3 color;		//色
	float  Range;		//影響範囲
};

//スポットライト
struct SpotLight
{
	float3 position;	//位置
	float3 color;		//色
	float  Range;		//射出範囲
	float3 direction;	//射出方向
	float  angle;		//射出角度
};

//半球ライト
struct HemiSphereLight
{
	float3 groundColor;		//地面色（照り返しのライト）
	float3 skyColor;		//天球色
	float3 groundNormal;	//地面の法線
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ライトの定数バッファ
cbuffer LightCb : register(b1)
{
	DirectionLight  directionLight;				//ディレクションライト
	PointLight	    pointLight[12];				//ポイントライト
	SpotLight	    spotLight;					//スポットライト
	HemiSphereLight hemiSphereLight;			//半球ライト

	float3 eyePos;	//視点の位置
};

////////////////////////////////////////////////
//関数宣言
////////////////////////////////////////////////
//拡散反射光の計算
float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
//鏡面反射光の計算
float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
//距離による影響率の計算
float CalculateImpactRate(float3 ligPos, float ligRange, float3 worldPos);
//リムライトの計算
float3 CalculateRimlight(float3 lightDirection, float3 lightColor, float3 normal, float normalInViewZ);

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
sampler g_sampler : register(s0);							//サンプラステート。

Texture2D<float4> g_albedoMap	: register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap	: register(t1);				//法線マップマップ
Texture2D<float4> g_specularMap : register(t2);				//スぺキュラマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);		//ボーン行列。
Texture2D<float4> g_toonMap		: register(t4);				//トゥーンシェーダーテクスチャー

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

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	//スキンありメッシュ
	if( hasSkin )
	{
		//スキン行列を計算
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	//スキン無しメッシュ
	else
	{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);

	//頂点シェーダーでカメラ空間でのZ値を設定
	psIn.depthInView = psIn.pos.z;

	psIn.pos = mul(mProj, psIn.pos);
	//法線を回転
	psIn.normal = mul(m, vsIn.normal);
	psIn.uv = vsIn.uv;
	//カメラ空間の法線を求める
	psIn.normalInView = mul(mView,psIn.normal);
	//頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
	psIn.posInProj = psIn.pos;
	psIn.posInProj.xy /= psIn.posInProj.w;

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
SPSIn VSSkinMain( SVSIn vsIn )
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
SPSOut PSMain(SPSIn psIn) : SV_Target0
{
	//法線カラーを返すデバック。
	//return float4(psIn.normal,1.0f);


	////////////////////////////////////ライティング/////////////////////////////////////////////////////////

	//ディレクションライト
	//ディレクションライトのフォン拡散反射を計算
	float3 diffDirection = CalculateLambertDiffuse(
		directionLight.direction,
		directionLight.color,
		psIn.normal
	);
	//ディレクションライトのフォン鏡面反射を計算
	float3 specDirection = CalculatePhoneSpecular(
		directionLight.direction,
		directionLight.color,
		psIn.worldPos,
		psIn.normal
	);
	//ディレクションライトのライティングをまとめる
	float3 finalDirectionLig = diffDirection + specDirection;



	//ポイントライト

	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 ligDir = psIn.worldPos - pointLight[0].position;
	//正規化する。
	ligDir = normalize(ligDir);

	//減衰なしのランバート拡散反射光を計算
	float3 diffPoint = CalculateLambertDiffuse(
		ligDir,
		pointLight[0].color,
		psIn.normal
	);
	//減衰なしのフォン鏡面反射光を計算
	float3 specPoint = CalculatePhoneSpecular(
		ligDir,
		pointLight[0].color,
		psIn.worldPos,
		psIn.normal
	);

	float pAffect;	//影響率
	//距離による影響率を計算
	pAffect = CalculateImpactRate(pointLight[0].position, pointLight[0].Range, psIn.worldPos);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	diffPoint *= pAffect;
	specPoint *= pAffect;
	//ポイントライトのライティングをまとめる
	float3 finalPointLig = diffPoint + specPoint;



	//スポットライト用

	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 sLigDir = psIn.worldPos - spotLight.position;
	//正規化する。
	sLigDir = normalize(sLigDir);

	//減衰なしのランバート拡散反射光を計算する。
	float3 diffSpot = CalculateLambertDiffuse(
		sLigDir,
		spotLight.color,
		psIn.normal
	);

	//減衰なしのフォン鏡面反射光を計算する。
	float3 specSpot = CalculatePhoneSpecular(
		sLigDir,
		spotLight.color,
		psIn.worldPos,
		psIn.normal
	);

	//距離による影響率を計算する。
	float sAffect = CalculateImpactRate(spotLight.position, spotLight.Range, psIn.worldPos);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	diffSpot *= sAffect;
	specSpot *= sAffect;


	//入射光と射出方向の角度を求める。
	//dot()を利用して内積を求める。
	float sAngle = dot(sLigDir, spotLight.direction);
	//dot()で求めた値をacosに渡して角度を求める。
	sAngle = acos(sAngle);

	float aAffect;
	//角度による影響率を求める。
	//角度に比例して小さくなっていく影響率を計算する。
	aAffect = 1.0f - 1.0f / spotLight.angle * sAngle;
	//影響力がマイナスにならないように補正をかける。
	aAffect = max(0.0f, aAffect);
	//影響の仕方を指数関数的にする。
	aAffect = pow(aAffect, 3.0f);
	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	diffSpot *= aAffect;
	specSpot *= aAffect;
	//スポットライトのライティングをまとめる
	float3 finalSpotLig = diffSpot + specSpot;



	//リムライト

	float3 dirLim = CalculateRimlight(
		directionLight.direction,
		directionLight.color,
		psIn.normal,
		psIn.normalInView.z
	);
	float3 spotLim = CalculateRimlight(
		spotLight.direction,
		spotLight.color,
		psIn.normal,
		psIn.normalInView.z
	);
	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	dirLim *= aAffect;
	spotLim *= aAffect;
	//リムライトのライティングをまとめる
	float3 finalLimLig = dirLim + spotLim;



	//半球ライト

	//サーフェイスの法線と地面の法線との内積を計算する。
	float hLigT = dot(psIn.normal, hemiSphereLight.groundNormal);
	//内積の結果を0~1の範囲に変換する。
	hLigT = (hLigT + 1.0f) / 2.0f;
	//地面色と天球色を補完率hLigTで線形補完する。
	float3 hemiLight = lerp(hemiSphereLight.groundColor, hemiSphereLight.skyColor, hLigT);


	//環境光(簡易)
	//これを加えて一律にライトの明るさを底上げ
	const float3 ambientLig = { 0.3f,0.3f,0.3f };


	//拡散反射光・鏡面反射光・リムライト・半球ライト・環境光を加算して最終的な光を求める。
	float3 lig = finalDirectionLig + finalPointLig + finalSpotLig + finalLimLig + hemiLight + ambientLig;
	//モデルのテクスチャから色をフェッチする
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);
	//テクスチャカラーに求めた光を乗算して最終出力カラーを求める。
	float4 finalColor;
	finalColor.xyz = albedoColor.xyz * lig;
	//完全に初期化するためにwの値も設定
	finalColor.w = 1.0f;
	////////////////////////////////////ライティングEND///////////////////////////////////////////////////////////



	///////////////////////////トゥーンシェーダー///////////////////////////////
	//ハーフランバート拡散照明によるライティング計算
	float p = dot(psIn.normal, directionLight.direction);

	p *= -1.0f;

	// 内積の値を0以上の値にする
	p = max(0.0f, p);
	p = p * 0.5f + 0.5f;
	p = p * p;

	//計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_toonMap.Sample(g_sampler, float2(p, 0.0f));

	//最終カラーにトゥーンカラーをかける
	finalColor *= toonColor;
	///////////////////////////トゥーンシェーダーEND////////////////////////////////



	//ピクセルシェーダーからカラーとZ値を出力する
	SPSOut psOut;
	psOut.color = finalColor;
	// カメラ空間での深度値を設定
	psOut.depth = psIn.depthInView;


	//最終カラーを返す
	return psOut;
}


//関数

//拡散反射光の計算
float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する。
	float t = dot(normal,lightDirection);

	//内積の結果に-1を乗算する。
	t *= -1.0f;

	//内積の結果が0以下なら0にする。
	t = max(0.0f, t);

	//拡散反射光を求める。
	return lightColor * t;
}

//鏡面反射光の計算
float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	//反射ベクトルを求める。
	float3 refVec = reflect(lightDirection,normal);

	//光が当たったサーフェイスから視点に伸びるベクトルを求める。
	float3 toEye = eyePos - worldPos;
	//正規化する。
	toEye = normalize(toEye);

	//鏡面反射の強さを求める。
	//dot関数を利用してrefVecとtoEyeの内積を求める。
	float t = dot(refVec,toEye);
	//内積の結果はマイナスになるので、マイナスの場合は0にする。
	t = max(0.0f, t);

	//鏡面反射の強さを絞る。
	t = pow(t,1.5f);

	//鏡面反射光を求める。
	return directionLight.color * t;
}

//影響率の計算
float CalculateImpactRate(float3 ligPos, float ligRange, float3 worldPos)
{
	//距離による影響率を計算する。
	//ポイントライトとの距離を計算する。
	float3 lDistance = length(worldPos - ligPos);

	//影響率は距離に比例して小さくなっていく。
	float lAffect = 1.0f - 1.0f / ligRange * lDistance;

	//影響力がマイナスにならないように補正をかける。
	lAffect = max(0.0, lAffect);

	//影響を指数関数的にする。
	lAffect = pow(lAffect,1.5f);

	return lAffect;
}

//リムライトの計算
float3 CalculateRimlight(float3 lightDirection, float3 lightColor, float3 normal, float normalInViewZ)
{
	//サーフェイスの法線と光の入射方向に依存するリムの強さを求める。
	float power1 = 1.0f - max(0.0f,dot(lightDirection,normal));

	//サーフェイスの法線と視線の方向に依存するリムの強さを求める。
	float power2 = 1.0f - max(0.0f,normalInViewZ * -1.0f);

	//最終的なリムの強さを求める。
	float limPower = power1* power2;

	//pow()を使用して、強さの変化を指数関数的にする。
	limPower = pow(limPower,0.5f);

	//最終的な反射光にリムライトの反射光を合算する。
	//まずはリムライトのカラーを計算する。
	float3 limColor = limPower * lightColor;

	return limColor;
}