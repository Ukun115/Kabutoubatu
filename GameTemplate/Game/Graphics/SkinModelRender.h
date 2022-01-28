/// <summary>
/// モデル描画クラス
/// </summary>
#pragma once

/// <summary>
/// 前方宣言
/// </summary>
class Model;	//モデル

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Light;			//ライト
	class DepthShadow;		//デプスシャドウ
	class RenderingEngine;	//レンダリングエンジン
	class Silhouette;		//シルエット描画
	class MiniMap;			//ミニマップ
	class PlayerCamera;		//プレイヤーカメラ
	class SkyCube;			//スカイキューブ

	/// <summary>
	/// 構造体
	/// </summary>

	//モデルのシェーダーに渡す情報
	struct stModelMode
	{
		//プレイヤーのモード
		int playerMode = 2;	//初期値はその他の２番

		//輪郭線をつけるモデルかどうかのフラグ
		int outline = false;

		//シルエットをつけるかどうかのフラグ
		int silhouetteFlg = false;

		//ダメージ受けた時の赤色にチカっとするフラグ
		int damageColorFlg = false;
	};

	class SkinModelRender : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//輪郭線の色
		enum enOutLineColor
		{
			enRed,		//赤色
			enBlue,		//青色
			enBlack		//黒色
		};

		/// <summary>
		/// クラスのポインタ
		/// </summary>
		Light* m_light = nullptr;					//ライト
		DepthShadow* m_depthShadow = nullptr;		//デプスシャドウ
		RenderingEngine* m_zPrepass = nullptr;				//ZPrepass
		Silhouette* m_silhouette = nullptr;			//シルエット
		MiniMap* m_miniMap = nullptr;				//ミニマップ
		SkyCube* m_skyCube = nullptr;				//スカイキューブ
		PlayerCamera* m_playerCamera = nullptr;		//プレイヤーカメラ
		AnimationClip* m_animationClips = nullptr;	//アニメーションクリップ。

		ModelInitData m_modelInitData;				//モデルのデータ
		ModelInitData m_shadowModelInitData;		//シャドウモデルのデータ
		ModelInitData m_zPrepassModelInitData;		//ZPrepassモデルのデータ
		ModelInitData m_silhouetteModelInitData;	//シルエットモデルのデータ
		ModelInitData m_miniMapModelInitData;		//ミニマップモデルのデータ
		Model m_model;				//モデル
		Model m_shadowModel;		//シャドウのモデル
		Model m_zPrepassModel;		//ZPrepassのモデル
		Model m_silhouetteModel;	//シルエットのモデル
		Model m_miniMapModel;		//ミニマップのモデル
		Skeleton m_skeleton;		//スケルトン
		Animation m_animation;		//アニメーション
		stModelMode m_modelMode;
		//スケルトンありのモデルの上方向。通常時はYアップ。
		//スケルトンなしのモデルの上方向は通常時Zアップ。
		EnModelUpAxis m_skinModelUpAxis = enModelUpAxisY;

		int	m_animationClipsNum = 0;				//アニメーションクリップの数。
		Vector3    m_pos = Vector3::Zero;			//位置
		Quaternion m_rot = Quaternion::Identity;	//回転
		Vector3    m_sca = Vector3::One;			//拡大率
		const char* m_tksFilePath = nullptr;					//スケルトンのファイルパス
		char m_filePathM[256];						//モデルのファイルパス
		char m_filePathS[256];						//スケルトンのファイルパス
		//シャドウキャスターかどうか(シャドウを落とす物体かどうか)のフラグ
		bool m_shadowCaster = false;
		bool m_silhouetteFlg = false;
		float m_animationSpeed = 1.0f;
		//火の玉モードかどうかフラグ
		bool m_fireBallFlg = false;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

	public:
		static void PreLoadModel(const char* tkmFilePath);

		//モデルを初期化するメソッド
		void Init(const char* tkmFilePath, const char* tksFilePath = nullptr, AnimationClip* animationClips = nullptr, int animationClipsNum = 0);

		//モデルの描画メソッド
		void ModelRender(RenderContext& rc)override final;

		//シャドウの描画メソッド
		void ShadowRender(RenderContext& rc)override final;

		//輪郭線の描画メソッド
		void ZPrepassRender(RenderContext& rc)override final;

		//シルエットモデルの描画メソッド
		void SilhouetteRender(RenderContext& rc)override final;

		//ミニマップの描画メソッド
		void MiniMapRender(RenderContext& rc)override final;

		/*!
		* @brief	アニメーションを再生。
		*@param[in]	clipNo	アニメーションクリップの番号。コンストラクタに渡したanimClipListの並びとなる。
		*@param[in]	interpolateTime		補完時間(単位：秒)	補完時間が長いほどゆっくり次のアニメーションに切り替わる
		*/
		void PlayAnimation(int animNo, float interpolateTime = 0.1f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// セッター
		/// </summary>
		//モデルの位置を設定する関数
		void SetPosition(const Vector3& pos) { m_pos = pos; }

		//モデルの回転を設定する関数
		void SetRotation(const Quaternion& rot) { m_rot = rot; }

		//モデルの拡大率を設定する関数
		void SetScale(const Vector3& sca) { m_sca = sca; }

		//シャドウをつけるかどうかのフラグをセットするメソッド
		void SetShadowCaster(const bool shadowCaster) { m_shadowCaster = shadowCaster; }

		//シルエットをつけるかどうかのフラグをセットするメソッド
		void SetSilhouette(const bool silhouetteFlg) { m_modelMode.silhouetteFlg = silhouetteFlg; }

		//輪郭線をつけるかどうかのフラグをセットするメソッド
		void SetOutline(const bool outline) { m_modelMode.outline = outline; }

		//１P２Pその他を判別する番号をセットするメソッド
		void SetPlayerMode(const int playerMode) { m_modelMode.playerMode = playerMode; }

		//スケルトンありのモデルの上方向を指定するメソッド
		void SetModelUpAxis(const EnModelUpAxis modelUpAxis) { m_skinModelUpAxis = modelUpAxis; }

		void SetAnimationSpeed(const float animationSpeed) { m_animationSpeed = animationSpeed; };

		void SetDameageRed(const bool flg) { m_modelMode.damageColorFlg = flg; };

		void SetFireBall(const bool flg) { m_fireBallFlg = flg; };

		/// <summary>
		/// モデルの位置を取得
		/// </summary>
		/// <returns>モデルの位置</returns>
		Vector3 GetPosition()const { return m_pos; }

		/// <summary>
		/// 回転クォータニオンを取得
		/// </summary>
		/// <returns>回転クォータニオン</returns>
		Quaternion GetRotation()const { return m_rot; }

		/// <summary>
		/// モデルのアドレスを取得
		/// </summary>
		/// <returns>モデルのアドレス</returns>
		Model* GetModel() { return &m_model; }

		/// <summary>
		/// スケルトンのアドレスを取得
		/// </summary>
		/// <returns>スケルトン</returns>
		Skeleton* GetSkeleton() { return &m_skeleton; }

		/// <summary>
		/// アニメーション再生中かを取得
		/// </summary>
		/// <returns>アニメーション再生中かどうか</returns>
		bool IsPlaying() { return m_animation.IsPlaying(); };

		/// <summary>
		/// 火の玉モードかどうかを取得
		/// </summary>
		/// <returns>火の玉モードかどうか</returns>
		bool GetFireBall()const { return m_fireBallFlg; };
	};
}