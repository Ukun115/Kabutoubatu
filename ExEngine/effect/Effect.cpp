/*!
 * @brief	�G�t�F�N�g
 */

#include "stdafx.h"
#include "Effect.h"



Effect::Effect()
{
}
Effect::~Effect()
{
	//�Đ����̃G�t�F�N�g���~����B
	EffectEngine::GetInstance()->Stop(m_handle);
}


void Effect::Init(const char16_t* filePath)
{
	/*char m_filePath[256];
	sprintf(m_filePath, "Assets/effect/efk/%s.efk", filePath);*/
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);

	//�ǂݍ��߂Ă��Ȃ�������G���[���b�Z�[�W���o���B
	if (m_effect == nullptr)
	{
		MessageBoxA(nullptr, "�G�t�F�N�g���������ǂݍ��܂�܂���ł����B", "�G���[", MB_OK);
	}

}
void Effect::Play()
{
	//�Đ����̃G�t�F�N�g���~����B
	EffectEngine::GetInstance()->Stop(m_handle);
	//�V�����Đ��B
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}
void Effect::Stop()
{
	//�Đ����̃G�t�F�N�g���~����B
	EffectEngine::GetInstance()->Stop(m_handle);
}
void Effect::Update()
{
	EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
		m_handle,
		m_position,
		m_rotation,
		m_scale
	);
}