/// <summary>
/// タイトルシーンのサウンドクラス
/// </summary>
#include "stdafx.h"
#include "TitleSceneSound.h"
#include "TitleScene.h"

namespace nsKabutoubatu
{
	namespace nsTitleSceneSound
	{
		const float GINGLE_SOUND_VOLUME = 0.5f;
	}

	bool TitleSceneSound::Start()
	{
		//タイトルジングルサウンドの初期化
		m_gingleSound = NewGO<SoundSource>();
		m_gingleSound->Init(L"Assets/sound/TitleGingle.wav");
		m_gingleSound->SetVolume(nsTitleSceneSound::GINGLE_SOUND_VOLUME);
		m_gingleSound->Play(false);	//ワンショット再生

		return true;
	}

	TitleSceneSound::~TitleSceneSound()
	{

	}

	void TitleSceneSound::Update()
	{

	}

	//選択音再生メソッド
	void TitleSceneSound::PlaySelectSound()
	{
		//選択サウンドの初期化
		m_selectSound = NewGO<SoundSource>();
		m_selectSound->Init(L"Assets/sound/Select.wav");
		m_selectSound->SetVolume(0.3f);
		//ワンショット再生
		m_selectSound->Play(false);
	}
	//決定音再生メソッド
	void TitleSceneSound::PlayDecideSound()
	{
		//決定サウンドの初期化
		m_decideSound = NewGO<SoundSource>();
		m_decideSound->Init(L"Assets/sound/Decide.wav");
		m_decideSound->SetVolume(0.3f);
		//ワンショット再生
		m_decideSound->Play(false);
	}
}