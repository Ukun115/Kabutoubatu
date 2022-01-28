/// <summary>
/// フェードクラス
/// </summary>
#include "stdafx.h"
#include "Fade.h"

namespace nsKabutoubatu
{
	namespace nsFade
	{
		//フェード画像の縦横サイズ
		const float FADE_IMAGE_SIZE = 1500.0f;
		//フェードするスピード
		const float FADE_SPEED = 0.03f;
		//真っ白
		const float ALL_WHITE = 1.0f;
		//R
		const float INIT_R = 1.0f;
		//G
		const float INIT_G = 1.0f;
		//B
		const float INIT_B = 1.0f;

		const char* const FADE_COLOR[2] = {"Fade_Black", "Fade_White"};
	}

	void Fade::Init(const float alphaValue, const int nowState, const int fadeColorState)
	{
		//α値を設定
		m_alphaValue = alphaValue;
		//フェードの状態を設定
		m_nowState = nowState;
		//フェードの色のステートを設定
		m_fadeColorState = fadeColorState;

		//フェード画像を初期化
		m_fadeSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_10, nullptr);
		m_fadeSprite->Init(nsFade::FADE_COLOR[m_fadeColorState], nsFade::FADE_IMAGE_SIZE, nsFade::FADE_IMAGE_SIZE);
		//アルファ値を設定
		m_fadeSprite->SetMulColor(0.0f, 0.0f, 0.0f, m_alphaValue);
	}

	Fade::~Fade()
	{
		//フェード画像を削除
		DeleteGO(m_fadeSprite);
	}

	void Fade::Update()
	{
		//状態に応じて変化
		switch (m_nowState)
		{
		//フェードイン中
		case StateIn:
			m_alphaValue -= nsFade::FADE_SPEED;
			//完全にフェードインしたら、
			if (m_alphaValue <= 0.0f) {
				//フェードを削除する
				DeleteGO(this);
			}
			break;
		//フェードアウト中
		case StateOut:
			m_alphaValue += nsFade::FADE_SPEED;
			//完全に真っ白になったら待機させる
			if (m_alphaValue >= nsFade::ALL_WHITE) {
				//待機中にする
				m_nowState = StateWait;
			}
			break;
		}
		//アルファ値を更新
		m_fadeSprite->SetMulColor(nsFade::INIT_R, nsFade::INIT_G, nsFade::INIT_B, m_alphaValue);
	}

	void Fade::UiUpdate()
	{
		//状態に応じて変化
		switch (m_nowState)
		{
			//フェードイン中
		case StateIn:
			m_alphaValue -= nsFade::FADE_SPEED;
			//完全にフェードインしたら、
			if (m_alphaValue <= 0.0f) {
				//フェードを削除する
				DeleteGO(this);
			}
			break;
			//フェードアウト中
		case StateOut:
			m_alphaValue += nsFade::FADE_SPEED;
			//完全に真っ白になったら待機させる
			if (m_alphaValue >= nsFade::ALL_WHITE) {
				//待機中にする
				m_nowState = StateWait;
			}
			break;
		}
		//アルファ値を更新
		m_fadeSprite->SetMulColor(nsFade::INIT_R, nsFade::INIT_G, nsFade::INIT_B, m_alphaValue);
	}
}