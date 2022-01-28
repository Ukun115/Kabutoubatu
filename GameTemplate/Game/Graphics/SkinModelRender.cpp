/// <summary>
/// モデル描画クラス
/// </summary>

#include "stdafx.h"
#include "Light.h"
#include "DepthShadow.h"
#include "RenderingEngine.h"
#include "Silhouette.h"
#include "MiniMap.h"
#include "../InGame/Camera/PlayerCamera.h"
#include "SkyCube.h"
#include "../ResourceBankManager.h"

namespace nsKabutoubatu
{
	void SkinModelRender::Update()
	{
		//モデルの位置、回転、拡大を更新
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		//スケルトンを更新
		if (m_tksFilePath != nullptr)
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		//アニメーションを更新
		if (m_animationClips != nullptr)
		{
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		//シャドウの位置、回転、拡大を更新
		if (m_shadowCaster)
		{
			m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//輪郭線の位置、回転、拡大を更新
		if (m_modelMode.outline)
		{
			m_zPrepassModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//シルエットの位置、回転、拡大を更新
		if (m_modelMode.silhouetteFlg)
		{
			m_silhouetteModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//ミニマップの位置、回転、拡大を更新
		m_miniMapModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	void SkinModelRender::UiUpdate()
	{
		//モデルの位置、回転、拡大を更新
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		//スケルトンを更新
		if (m_tksFilePath != nullptr)
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		//アニメーションを更新
		if (m_animationClips != nullptr)
		{
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		//シャドウの位置、回転、拡大を更新
		if (m_shadowCaster)
		{
			m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//輪郭線の位置、回転、拡大を更新
		if (m_modelMode.outline)
		{
			m_zPrepassModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//シルエットの位置、回転、拡大を更新
		if (m_modelMode.silhouetteFlg)
		{
			m_silhouetteModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		}
		//ミニマップの位置、回転、拡大を更新
		m_miniMapModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	//モデルを初期化するメソッド
	void SkinModelRender::Init(const char* tkmFilePath,const char* tksFilePath,AnimationClip* animationClips, int animationClipsNum)
	{
		//インスタンスを探す
		m_light = FindGO<Light>(nsStdafx::LIGHT_NAME);
		m_depthShadow = FindGO<DepthShadow>(nsStdafx::DEPTHSHADOW_NAME);
		m_zPrepass = FindGO<RenderingEngine>(nsStdafx::RENDERINGENGINE_NAME);
		m_silhouette = FindGO<Silhouette>(nsStdafx::SILHOUETTE_NAME);
		m_miniMap = FindGO<MiniMap>(nsStdafx::MINIMAP_NAME);
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);
		m_skyCube = FindGO<SkyCube>(nsStdafx::SKYCUBE_NAME);

		//ファイル名をファイルパスにする
		sprintf(m_filePathM, "Assets/modelData/%s.tkm", tkmFilePath);
		//tkmファイルのファイルパスを設定する
		m_modelInitData.m_tkmFilePath = m_filePathM;
		//使用するシェーダーファイルパスを設定する
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_modelInitData.m_vsEntryPointFunc = "VSMain";
		//シェーダーのピクセルシェーダーのエントリー関数名の指定
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		/*モデルの初期化情報を定数バッファとしてディスクリプタヒープに
		  登録するためにモデルの初期化情報として渡す					*/
		m_modelInitData.m_expandConstantBuffer[0] = m_light->GetLightAdoress();
		m_modelInitData.m_expandConstantBufferSize[0] = sizeof(m_light->GetLight());
		//シェーダーにモデルのモードを渡す
		m_modelInitData.m_expandConstantBuffer[1] = &m_modelMode;
		m_modelInitData.m_expandConstantBufferSize[1] = sizeof(m_modelMode);
		/*モデルの初期化情報を定数バッファとしてディスクリプタヒープに
		  登録するためにモデルの初期化情報として渡す					*/
		m_modelInitData.m_expandConstantBuffer[2] = m_playerCamera->GetCameraAdoress();
		m_modelInitData.m_expandConstantBufferSize[2] = sizeof(m_playerCamera->GetCamera());
		//シャドウマップを拡張SRVに設定する。
		m_modelInitData.m_expandShaderResourceView[0] = &m_depthShadow->GetShadowMapTexture();
		//モデルのみの被写界深度マップを拡張SRVに設定する。
		m_modelInitData.m_expandShaderResourceView[1] = &m_zPrepass->GetZPrepassDepthTexture();
		//シルエットマップを拡張SRVに設定する。
		m_modelInitData.m_expandShaderResourceView[2] = &m_silhouette->GetSilhouetteMapTexture();
		////スカイキューブマップを拡張SRVに設定する。
		//m_modelInitData.m_expandShaderResourceView[3] = &m_skyCube->GetSkyCubeMapTexture();



		///////////////////////////////ミニマップ////////////////////////////////////
		//tkmファイルのファイルパスを設定する
		m_miniMapModelInitData.m_tkmFilePath = m_filePathM;
		//使用するシェーダーファイルパスを設定する
		m_miniMapModelInitData.m_fxFilePath = "Assets/shader/miniMap.fx";
		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_miniMapModelInitData.m_vsEntryPointFunc = "VSMain";
		//シェーダーのピクセルシェーダーのエントリー関数名の指定
		m_miniMapModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		/*モデルの初期化情報を定数バッファとしてディスクリプタヒープに
		  登録するためにモデルの初期化情報として渡す					*/
		m_miniMapModelInitData.m_expandConstantBuffer[0] = m_light->GetLightAdoress();
		m_miniMapModelInitData.m_expandConstantBufferSize[0] = sizeof(m_light->GetLight());
		//シャドウモデルを初期化
		m_miniMapModel.Init(m_miniMapModelInitData);
		///////////////////////////////ミニマップEND//////////////////////////////////



		/////////////////////////////////////スケルトン////////////////////////////////////////////
		if (tksFilePath != nullptr)
		{
			//ファイル名を保存
			m_tksFilePath = tksFilePath;
			//ファイル名をファイルパスにする
			sprintf(m_filePathS, "Assets/modelData/%s.tks", tksFilePath);
			m_skeleton.Init(m_filePathS);
			//モデルにスケルトンのデータを渡す
			m_modelInitData.m_skeleton = &m_skeleton;
			//シャドウモデルにスケルトンのデータを渡す
			if (m_shadowCaster)
			{
				m_shadowModelInitData.m_skeleton = &m_skeleton;
			}
			//輪郭線モデルにスケルトンのデータを渡す
			if (m_modelMode.outline)
			{
				m_zPrepassModelInitData.m_skeleton = &m_skeleton;
			}
			//シルエットモデルにスケルトンのデータを渡す
			if (m_modelMode.silhouetteFlg)
			{
				m_silhouetteModelInitData.m_skeleton = &m_skeleton;
			}
		}
		/////////////////////////////////////スケルトンEND////////////////////////////////////////////



		///////////////////////////////アニメーション////////////////////////////////////
		//アニメーションを初期化
		if (animationClips != nullptr)
		{
			//アニメーションクリップを保存
			m_animationClips = animationClips;
			//アニメーションクリップ数を保存
			m_animationClipsNum = animationClipsNum;
			//アニメーションを初期化
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipsNum);
			//モデルの上方向を指定する。
			m_modelInitData.m_modelUpAxis = m_skinModelUpAxis;
		}
		///////////////////////////////アニメーションEND/////////////////////////////////



		//////////////////////////////////シャドウ/////////////////////////////////////
		if (m_shadowCaster)
		{
			m_shadowModelInitData.m_tkmFilePath = m_filePathM;
			//ファイル名をファイルパスにする
			m_shadowModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
			//シェーダーの頂点シェーダーのエントリー関数名の指定
			m_shadowModelInitData.m_vsEntryPointFunc = "VSMain";
			//シェーダーのピクセルシェーダーのエントリー関数名の指定
			m_shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//シェーダーにモデルのモードを渡す
			m_shadowModelInitData.m_expandConstantBuffer[0] = &m_modelMode;
			m_shadowModelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelMode);
			//シャドウモデルを初期化
			m_shadowModel.Init(m_shadowModelInitData);
		}
		////////////////////////////////////シャドウEND//////////////////////////////////



		//////////////////////////ZPrepass///////////////////////////////////////
		if (m_modelMode.outline)
		{
			m_zPrepassModelInitData.m_tkmFilePath = m_filePathM;
			//ファイル名をファイルパスにする
			m_zPrepassModelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
			//シェーダーの頂点シェーダーのエントリー関数名の指定
			m_zPrepassModelInitData.m_vsEntryPointFunc = "VSMain";
			//シェーダーのピクセルシェーダーのエントリー関数名の指定
			m_zPrepassModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			m_zPrepassModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;
			//ZPrepassモデルを初期化
			m_zPrepassModel.Init(m_zPrepassModelInitData);
		}
		//////////////////////////ZPrepassEND///////////////////////////////////////



		////////////////////////////////シルエット////////////////////////////////
		if (m_modelMode.silhouetteFlg)
		{
			//シャドウの逆をやってやるとできる
			//シャドウはライトカメラだったが、シルエットはカメラを使う

			m_silhouetteModelInitData.m_tkmFilePath = m_filePathM;
			//ファイル名をファイルパスにする
			m_silhouetteModelInitData.m_fxFilePath = "Assets/shader/drawSilhouetteMap.fx";
			//シェーダーの頂点シェーダーのエントリー関数名の指定
			m_silhouetteModelInitData.m_vsEntryPointFunc = "VSMain";
			//シェーダーのピクセルシェーダーのエントリー関数名の指定
			m_silhouetteModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//シェーダーにモデルのモードを渡す
			m_silhouetteModelInitData.m_expandConstantBuffer[0] = &m_modelMode;
			m_silhouetteModelInitData.m_expandConstantBufferSize[0] = sizeof(m_modelMode);
			//シルエットモデルを初期化
			m_silhouetteModel.Init(m_silhouetteModelInitData);
		}
		///////////////////////////////シルエットEND//////////////////////////////////



		//初期化情報を使ってモデル表示処理を初期化する
		m_model.Init(m_modelInitData);
	}


	//描画関数

	//モデルの描画メソッド
	void SkinModelRender::ModelRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		//モデルのドローコールを実行し、表示する
		m_model.Draw(rc);
	}
	//シャドウの描画メソッド
	void SkinModelRender::ShadowRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_shadowCaster)
		{
			return;
		}

		//シャドウ
		m_shadowModel.Draw(rc, *m_light->GetLightCamera());
	}
	//輪郭線の描画メソッド
	void SkinModelRender::ZPrepassRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_modelMode.outline)
		{
			return;
		}

		//輪郭線モデル
		m_zPrepassModel.Draw(rc);
	}
	//シルエットモデルの描画メソッド
	void SkinModelRender::SilhouetteRender(RenderContext& rc)
	{
		if (m_fireBallFlg)
		{
			return;
		}

		if (!m_modelMode.silhouetteFlg)
		{
			return;
		}

		//シルエットモデル
		//カメラ視点からのシルエットマップ
		m_silhouetteModel.Draw(rc);
	}

	//ミニマップの描画メソッド
	void SkinModelRender::MiniMapRender(RenderContext& rc)
	{
		//真上からのミニマップ
		m_miniMapModel.Draw(rc ,*m_miniMap->GetMiniMapCamera());
	}

	void SkinModelRender::PreLoadModel(const char* tkmFilePath)
	{
		//プリロードはまだできない
		TkmFile* tkmFile = ResourceBankManager::GetInstance()->GetTkmFileFromBank(tkmFilePath);

		//未登録
		if (tkmFile == nullptr)
		{
			//登録
			tkmFile = new TkmFile;
			tkmFile->Load(tkmFilePath);
			ResourceBankManager::GetInstance()->RegistTkmFileToBank(tkmFilePath, tkmFile);
		}
	}
}