/// <summary>
/// �^�C�g���V�[���̃T�E���h�N���X
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
		//�^�C�g���W���O���T�E���h�̏�����
		m_gingleSound = NewGO<SoundSource>();
		m_gingleSound->Init(L"Assets/sound/TitleGingle.wav");
		m_gingleSound->SetVolume(nsTitleSceneSound::GINGLE_SOUND_VOLUME);
		m_gingleSound->Play(false);	//�����V���b�g�Đ�

		return true;
	}

	TitleSceneSound::~TitleSceneSound()
	{

	}

	void TitleSceneSound::Update()
	{

	}

	//�I�����Đ����\�b�h
	void TitleSceneSound::PlaySelectSound()
	{
		//�I���T�E���h�̏�����
		m_selectSound = NewGO<SoundSource>();
		m_selectSound->Init(L"Assets/sound/Select.wav");
		m_selectSound->SetVolume(0.3f);
		//�����V���b�g�Đ�
		m_selectSound->Play(false);
	}
	//���艹�Đ����\�b�h
	void TitleSceneSound::PlayDecideSound()
	{
		//����T�E���h�̏�����
		m_decideSound = NewGO<SoundSource>();
		m_decideSound->Init(L"Assets/sound/Decide.wav");
		m_decideSound->SetVolume(0.3f);
		//�����V���b�g�Đ�
		m_decideSound->Play(false);
	}
}