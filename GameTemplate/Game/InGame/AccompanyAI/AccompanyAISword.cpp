/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)の剣クラス
/// </summary>
#include "stdafx.h"
#include "AccompanyAISword.h"
#include "AccompanyAI.h"

namespace nsKabutoubatu
{
	bool AccompanyAISword::Start()
	{
		//同行AI(おとも)のインスタンスを検索
		m_accompaanyAi = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);

		//モデルを初期化
		m_model = NewGO<SkinModelRender>();
		m_model->SetShadowCaster(true);	//影を落とす
		m_model->SetOutline(false);		//輪郭線をつけない
		m_model->SetSilhouette(true);		//青色シルエットをつける
		m_model->SetPlayerMode(1);
		//Zアップに変更
		m_model->SetModelUpAxis(enModelUpAxisZ);
		m_model->Init("AccompanyAI_Sword");

		//右手ボーンを検索
		m_rightHandBoneId = m_accompaanyAi->GetSkeleton()->FindBoneID(L"mixamorig:RightHand");
		// 検索したボーンIDを使って、ボーンを取得する。
		m_rightHandBone = m_accompaanyAi->GetSkeleton()->GetBone(m_rightHandBoneId);

		return true;
	}

	AccompanyAISword::~AccompanyAISword()
	{
		//モデルを削除
		DeleteGO(m_model);
	}

	void AccompanyAISword::Update()
	{
		if (m_accompaanyAi->GetMode() == enAttackMode)
		{
			m_model->Activate();
		}
		else
		{
			m_model->Deactivate();
		}

		//ボーンのワールド座標・回転・拡大率を更新
		for (int boneNum = 0; boneNum < m_accompaanyAi->GetSkeleton()->GetNumBones(); boneNum++)
		{
			m_accompaanyAi->GetSkeleton()->GetBone(boneNum)->CalcWorldTRS(m_pos, m_rot, m_sca);
		}
		//取得したボーンのワールド座標を取得
		m_pos = m_rightHandBone->GetPosition();
		//取得したボーンの回転を取得
		m_rot = m_rightHandBone->GetRotation();

		//位置更新
		m_model->SetPosition(m_pos);
		//回転更新
		m_model->SetRotation(m_rot);
	}
}