/*!
 * @brief	エフェクト
 */

#include "stdafx.h"
#include "Effect.h"



Effect::Effect()
{
}
Effect::~Effect()
{
	//再生中のエフェクトを停止する。
	EffectEngine::GetInstance()->Stop(m_handle);
}


void Effect::Init(const char16_t* filePath)
{
	/*char m_filePath[256];
	sprintf(m_filePath, "Assets/effect/efk/%s.efk", filePath);*/
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);

	//読み込めていなかったらエラーメッセージを出す。
	if (m_effect == nullptr)
	{
		MessageBoxA(nullptr, "エフェクトが正しく読み込まれませんでした。", "エラー", MB_OK);
	}

}
void Effect::Play()
{
	//再生中のエフェクトを停止する。
	EffectEngine::GetInstance()->Stop(m_handle);
	//新しく再生。
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}
void Effect::Stop()
{
	//再生中のエフェクトを停止する。
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