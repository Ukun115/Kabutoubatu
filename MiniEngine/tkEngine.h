#pragma once

#include "HID/GamePad.h"
#include "../GameTemplate/Game/GameTime.h"
#include "../MiniEngine/time/FPSLimitter.h"

class TkmFile;
class Shader;
class GraphicsEngine;

class TkEngine {
public:

	/// <summary>
		/// フレームレートモード
		/// </summary>
	enum EnFrameRateMode {
		enFrameRateMode_Fix,		// 固定フレームレートモード。
		enFrameRateMode_Variable,	// 可変フレームレートモード。
	};
	/// <summary>
	/// フレームレートに関する情報
	/// </summary>
	struct FrameRateInfo {
		EnFrameRateMode frameRateMode;	// フレームレートモード
		int maxFPS;						// 最大FPS
	};

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

	const int GetRate()const {
		return m_rate;
	}

	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}

	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}

	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
	/// <summary>
		/// 現在のフレームレートに関する情報を取得
		/// </summary>
		/// <returns></returns>
	const FrameRateInfo& GetFrameRateInfo() const
	{
		return m_frameRateInfo;
	}
	/// <summary>
	/// フレームレートモードを設定する。
	/// </summary>
	/// <param name="frameRateMode">EnFrameRateModeを参照</param>
	/// <param name="maxFPS">最大FPS</param>
	void SetFrameRateMode(EnFrameRateMode frameRateMode, int maxFPS)
	{
		if (frameRateMode == enFrameRateMode_Fix) {
			// 固定フレームレート
			// 1フレームの経過時間の値を固定にする。
			m_gameTime.EnableFixedFrameDeltaTime(1.0f / maxFPS);
		}
		else {
			// 可変フレームレート
			m_gameTime.DisableFixedFrameDeltaTime();
		}
		m_fpsLimitter.SetMaxFPS(maxFPS);
		// フレームレートに関する情報を記憶。
		m_frameRateInfo.frameRateMode = frameRateMode;
		m_frameRateInfo.maxFPS = maxFPS;
	}

private:
	TResourceBank<TkmFile> m_tkmFileBank;
	TResourceBank<Shader> m_shaderBank;

	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	GameTime m_gameTime;							//ゲームタイム。
	int m_rate = 0;
	FrameRateInfo m_frameRateInfo = { enFrameRateMode_Variable , 60 };
	nsKabutoubatu::FPSLimitter m_fpsLimitter;						// FPSに制限をかける処理。
};

extern TkEngine* g_engine;	//TKエンジン。
extern GameTime* g_gameTime;

//デバッグ機能を簡易的にアクセスするための名前空間
namespace dbg {

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
	/*!
	* @brief	コリジョンのデバッグ表示を有効にする。
	*@param[in]	colliObj	コリジョンオブジェクト。
	*/
	static inline void SetDrawPhysicsCollisionEnable()
	{
		PhysicsWorld().SetDebugDrawMode(1);
	}
	/*!
	* @brief	コリジョンのデバッグ表示を有効にする。
	*@param[in]	colliObj	コリジョンオブジェクト。
	*/
	static inline void SetDrawPhysicsCollisionDisable()
	{
		PhysicsWorld().SetDebugDrawMode(0);
	}
	/*!
	* @brief	ベクトルのデバッグ出力。
	*/
	static inline void DrawVector(Vector3 vector, Vector3 origin, const char* name = "no name")
	{
		prefab::CVectorRender::SRenderData renderData = { name, vector, origin };
		Engine().GetVectorRender()->AddVector(renderData);
	}
	/*!
	* @brief	ベクトルのデバッグ描画を有効にする。
	*/
	static inline void SetDrawVectorEnable()
	{
		Engine().GetVectorRender()->SetRenderFlag(true);
	}
	/*!
	* @brief	ベクトルのデバッグ描画を無効にする。
	*/
	static inline void SetDrawVectorDisable()
	{
		Engine().GetVectorRender()->SetRenderFlag(false);
	}
	static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
	{
		Engine().GetVectorRender()->SetRenderVectorNameList(list);
	}
#else

	static inline void SetDrawPhysicsCollisionEnable()
	{
	}

	static inline void SetDrawPhysicsCollisionDisable()
	{
	}

	static inline void DrawVector(Vector3 vector, Vector3 origin, const char* name = "no name")
	{
	}

	static inline void SetDrawVectorEnable()
	{
	}

	static inline void SetDrawVectorDisable()
	{
	}
	static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
	{
	}
#endif

}
