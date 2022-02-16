#include "stdafx.h"
#include "system/system.h"
#include "../Game/Title/TitleScene.h"
#include "StopWatch.h"
#include "../Game/Graphics/RenderingEngine.h"
#include "../Game/InGame/Camera/PlayerCamera.h"
#include "StopWatch.h"
#include "../Game/InGame/Pause.h"
#include "Utilities/GameUpdateStop.h"
#include "../Game/InGame/Online/Online.h"
#include "ResourceBankManager.h"

/// <summary>
/// 標準入出力コンソールを初期化
/// </summary>
void InitStandardIOConsole();

/// <summary>
/// モデル事前ロード
/// </summary>
void PreLoad();

//////////////////////////////////////
// ウィンドウプログラムのメイン関数。
//////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//FPS値表示のOnOffはここで設定。
	bool m_fpsValueOn = true;

	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("ふたりで!! かぶとうばつ"));


	// 標準入出力コンソールの初期化
	//コマンドプロンプトを起動
	if (m_fpsValueOn)
	{
		InitStandardIOConsole();
	}

	//////////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	//フィジクスワールドのインスタンスを作成する。
	PhysicsWorld::CreateInstance();
	//リソースマネージャのインスタンスを作成
	ResourceBankManager::CreateInstance();
	//プリロード
	PreLoad();
	//エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();
	//サウンドエンジンのインスタンスを作成する。
	SoundEngine::CreateInstance();
	SoundEngine::GetInstance()->Init();

	//オンライン通信エンジン
	nsKabutoubatu::Online* online = NewGO<nsKabutoubatu::Online>(nsStdafx::PRIORITY_0, nsStdafx::ONLINE_NAME);

	//ライト
	nsKabutoubatu::Light* light = NewGO<nsKabutoubatu::Light>(nsStdafx::PRIORITY_0,nsStdafx::LIGHT_NAME);
	//ディレクションライトをセット
	light->SetDirectionLightData();
	//半球ライトをセット
	light->SetHemiSphereLightData();

	//レンダリングエンジン
	nsKabutoubatu::RenderingEngine* renderingEngine = NewGO<nsKabutoubatu::RenderingEngine>(nsStdafx::PRIORITY_0, nsStdafx::RENDERINGENGINE_NAME);
	//ZPrepass用のレンダリングターゲットを初期化
	renderingEngine->Init();

	//ポーズ
	nsKabutoubatu::Pause* pause = NewGO<nsKabutoubatu::Pause>(nsStdafx::PRIORITY_0,nsStdafx::PAUSE_NAME);

	//ゲーム時間の進行をストップさせるクラス
	nsKabutoubatu::GameUpdateStop* gameUpdateStop = NewGO<nsKabutoubatu::GameUpdateStop>(nsStdafx::PRIORITY_0,nsStdafx::GAMEUPDATESTOP_NAME);
	enum enMoveTarget
	{
		enUI,
		enCameraMove
	};

	//ストップウォッチを生成する
	StopWatch stopWatch;

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//タイトルシーン
	NewGO<nsKabutoubatu::TitleScene>(nsStdafx::PRIORITY_0,nsStdafx::TITLESCENE_NAME);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//ストップウォッチの計測開始
		stopWatch.Start();

		//レンダリング開始。
		g_engine->BeginFrame();

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		//ポーズ中でないときは通常更新処理を実行
		if(!pause->GetIsPause() && !gameUpdateStop->GetIsStop())
		{
			//ゲームオブジェクトマネージャーの更新。
			GameObjectManager::GetInstance()->ExecuteUpdate();
		}
		//ポーズ中の時はポーズ専用の更新処理を実行
		else if(pause->GetIsPause())
		{
			//ゲームオブジェクトマネージャーのポーズ更新。
			GameObjectManager::GetInstance()->ExecutePauseUpdate();
		}
		//ゲームの進行を止めてUIの動きのみを動かす。
		else if (gameUpdateStop->GetIsStop())
		{
			switch (gameUpdateStop->GetMoveTarget())
			{
			//UIのみを動かす
			case enUI:
				//ゲームオブジェクトマネージャーのUI演出更新。
				GameObjectManager::GetInstance()->ExecuteUiUpdate();

				break;

			//カメラ移動のみを動かす
			case enCameraMove:
				//カメラアニメーションを実装する時は更新関数を増やす。

				break;
			}
		}

		//物理ワールドの更新。
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		//エフェクトエンジンの更新。
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//レンダリングエンジンを実行
		renderingEngine->Execute(renderContext);

		//////////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////////
		g_engine->EndFrame();

		//スピンロックを行う。
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = 16 - (int)stopWatch.GetElapsedMillisecond();
		} while (restTime > 0);

		//ストップウォッチの計測終了
		stopWatch.Stop();

		//FPS値をデバック表示
		if (m_fpsValueOn)
		{
			printf("fps = %0.2f\n\n", 1.0f / stopWatch.GetElapsed());
		}

		//デルタタイムをストップウォッチの計測時間から、計算する
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}::FreeConsole();

	return 0;
}

/// <summary>
/// 標準入出力コンソールを初期化
/// </summary>
void InitStandardIOConsole()
{
	::AllocConsole();               // コマンドプロンプトが表示される
	freopen("CON", "w", stdout);    // 標準出力の割り当て

	auto fhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc;

	rc.Top = 0;
	rc.Left = 0;
	rc.Bottom = 30;
	rc.Right = 30;
	::SetConsoleWindowInfo(fhandle, TRUE, &rc);
}

/// <summary>
/// モデル事前ロード
/// </summary>
void PreLoad()
{
	//プレイヤー
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/Player1.tkm");	//1P
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/Player2.tkm");	//2P
	//敵
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_mole.tkm");		//モグラ
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_needleSlime.tkm");	//トゲスライム
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_slime.tkm");		//スライム
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/enemy_firstBoss.tkm");	//最初のボス
	//ステージ
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/sea.tkm");							//海
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_floor.tkm");			//床
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_hitAcce.tkm");		//当たり判定のある装飾品
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstStage_map1_dontHitAcce.tkm");	//当たり判定のない装飾品
	//建物
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/House.tkm");				//宿屋
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/HouseRed.tkm");			//アイテムショップ
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstBossHouse.tkm");	//最初のボスの建物
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/SalesPerson.tkm");		//店員
	//建物のドア
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/House_Door.tkm");			//宿屋のドア
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/HouseRed_Door.tkm");			//アイテムショップのドア
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/FirstBossHouse_Door.tkm");	//最初のボスの建物のドア
	//ドアの暗闇
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/DarknessEntrance.tkm");					//宿屋とアイテムのドアの暗闇
	nsKabutoubatu::SkinModelRender::PreLoadModel("Assets/modelData/DarknessEntrance_FirstBossHouse.tkm");	//最初のボスの建物のドアの暗闇
}