/// <summary>
/// �Q�[���^�C���N���X
/// </summary>
#pragma once
#include "stdafx.h"

class GameTime
{
public:
	static GameTime& GetInstance()
	{
		static GameTime t;
		return t;
	}

	/// <summary>
		/// 1�t���[���̌o�ߎ��Ԃ��Œ艻�����܂��B
		/// </summary>
		/// <remark>
		/// �ǂ�Ȏ��ɂ�����g���̂��H
		/// �Ⴆ�΁A���S�����^�̃I�����C���}���`�v���C�ȂǁB
		/// ���S�����^�̃I�����C���Q�[���ł́A�e�N���C�A���g�ԂŃQ�[���̐i�s���x����v������K�v������܂��B
		/// �ł��̂ŁA�σt���[�����[�g�łȂ��A�Œ�t���[�����[�g�ŃQ�[�������܂��B
		/// ���̂悤�ȏꍇ�ɃQ�[�����Ԃ��Œ艻�����Ă��������B
		/// </remark>
		/// <param name="fixedFrameDeltaTime"></param>
	void EnableFixedFrameDeltaTime(float fixedFrameDeltaTime)
	{
		m_fixedFrameDeltaTime = fixedFrameDeltaTime;
		m_isFixedFrameDeltaTime = true;
	}
	/// <summary>
	/// 1�t���[���̌o�ߎ��Ԃ̌Œ艻���������܂��B
	/// </summary>
	void DisableFixedFrameDeltaTime()
	{
		m_isFixedFrameDeltaTime = false;
	}

	/*!
	 *@brief	1�t���[���̌o�ߎ��Ԃ��擾(�P�ʁE�b)�B
	 */
	const float GetFrameDeltaTime() const
	{
		//�Œ�FPS�ɂ���B�ς͗v�����B
		//return 1.0f / 30.0f;
		return m_frameDeltaTime;
	}
	void PushFrameDeltaTime(float deltaTime)
	{
		m_frameDeltaTimeQue.push_back(deltaTime);
		if (m_frameDeltaTimeQue.size() > 30.0f) {
			float totalTime = 0.0f;
			for (auto time : m_frameDeltaTimeQue) {
				totalTime += time;
			}
			//���ϒl���Ƃ�B
			m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
			m_frameDeltaTimeQue.pop_front();
		}
	}
	/// <summary>
	/// �v���J�n
	/// </summary>
	/// <remark>
	/// �{�֐��̓G���W�����ł̂ݎg�p���܂��B
	/// ���[�U�[�͎g�p���Ȃ��ł��������B
	/// </remark>
	//void BeginMeasurement()
	//{
	//	//�v���J�n�B
	//	m_sw.Start();
	//}
	static GameTime& GameTimeFunc() {

		return GameTime::GetInstance();
	}

	private:
	std::list<float> m_frameDeltaTimeQue;
	float		m_frameDeltaTime = 1.0f / 60.0f;		//1�t���[���̌o�ߎ��ԁB
	bool		m_isFixedFrameDeltaTime = false;		// 1�t���[���̌o�ߎ��Ԃ��Œ艻����B
	float		m_fixedFrameDeltaTime = 1.0f / 60.0f;	// �Œ�o�ߎ��ԁB
};

