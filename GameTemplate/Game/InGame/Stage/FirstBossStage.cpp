/// <summary>
/// 最初のボスステージクラス
/// </summary>
#include "stdafx.h"
#include "FirstBossStage.h"
#include "../Enemy/FirstBoss.h"
#include "../GameScene.h"
#include "../../Result/GameClearScene.h"
#include "../../Graphics/Fade.h"

namespace nsKabutoubatu
{
	bool FirstBossStage::Start()
	{
		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,0);

		//ゲームシーンのインスタンスを検索
		m_gameScene = FindGO< GameScene>(nsStdafx::GAMESCENE_NAME);
		//ゲームBGMを止める
		m_gameScene->GameBGMStop();

		//ステージモデルの初期化
		m_stageModel = NewGO<SkinModelRender>();
		m_stageModel->SetShadowCaster(false);	//シャドウは落とさない
		m_stageModel->SetOutline(false);		//輪郭線をつけない
		m_stageModel->Init("FirstBossStage");
		//当たり判定を適応
		m_physicsStaticObject.CreateFromModel(*m_stageModel->GetModel(), m_stageModel->GetModel()->GetWorldMatrix());
		m_pos = { 0.0f,-10000.0f,0.0f };
		m_stageModel->SetPosition(m_pos);
		//当たり判定を更新
		m_physicsStaticObject.Update(m_pos, m_rot);

		//ボス敵
		m_firstBoss = NewGO<FirstBoss>();
		m_firstBoss->SetPlayerNum(m_playerNum);

		return true;
	}

	FirstBossStage::~FirstBossStage()
	{
		//ステージモデルを削除
		DeleteGO(m_stageModel);
		//ボス敵を削除
		DeleteGO(m_firstBoss);
	}

	void FirstBossStage::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
		}

		if (m_firstBossBGM == nullptr)
		{
			//ファーストボスBGMの初期化
			m_firstBossBGM = NewGO<SoundSource>();
			m_firstBossBGM->Init(L"Assets/sound/FirstBossBGM.wav");
			m_firstBossBGM->SetVolume(0.2f);
			//ファーストボスBGMを再生
			m_firstBossBGM->Play(true);	//ループ再生
		}
		//ボスのHPが0になったら、
		if (m_gameClearScene == nullptr && m_firstBoss->GetGameClearFlg())
		{
			//ゲームクリアクラスを生成
			m_gameClearScene = NewGO<GameClearScene>();
			m_gameClearScene->SetPlayerNum(m_playerNum);
			//BGMの音量を消す
			m_firstBossBGM->SetVolume(0.0f);
		}
	}
}