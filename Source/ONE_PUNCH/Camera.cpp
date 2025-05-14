#include "Player.h"
#include "Camera.h"
#include <DxLib.h>
#include "Geometry.h"
#include <EffekseerForDXLib.h>

namespace
{
	constexpr float kInitCameraNear = 60;
	constexpr float kInitCameraFar = 10000;
	constexpr float kCameraRotateSensitivity = 1.0f;
	constexpr float kLerpStrength = 0.1f;
	constexpr float kFovDegrees = 80.0f * Geometry::kDegToRad;
	// �����낹��ő�l
	constexpr float kCameraVRotPlusThreshold = 0.3f;
	// ���グ����ő�l
	constexpr float kCameraVRotMinusThreshold = 1.0f;

	const Vector3 kInitTargetToCamera = { 0, 300, -900 };
}

Camera::Camera() :
	m_targetToCamera(kInitTargetToCamera)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	SetCameraNearFar(kInitCameraNear, kInitCameraFar);
	SetupCamera_Perspective(kFovDegrees);
}

void Camera::Update()
{
	// Effekseer�̃J�����Ɠ����H����
	Effekseer_Sync3DSetting();

	m_lerpedTargetPos.LerpMyself(m_targetPos, kLerpStrength);
	// DxLib�̃J�����ɔ��f
	SetCameraPositionAndTarget_UpVecY(m_lerpedTargetPos + m_targetToCamera, m_lerpedTargetPos);
}

void Camera::Draw_Debug() const
{
	Vector3 cameraPos = GetCameraPosition();
	DrawFormatString(300, 0, 0xffffff, "�J�����̌��݈ʒuX:%f,Y:%f,Z:%f", cameraPos.x, cameraPos.y, cameraPos.z);
	DrawFormatString(300, 15, 0xffffff, "�J�����̌��݉�]V:%f,H:%f,T:%f", GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());

	// ��]������ɓ����
	const MATRIX camMat = MTranspose(GetCameraViewMatrix());
	const Vector3 rightAxis = VTransformSR({ 1,0,0 }, camMat);
	DrawLine3D(m_targetPos + rightAxis,  m_targetPos + rightAxis*200,0xffff00);
	const Vector3 strPos = ConvWorldPosToScreenPos(m_targetPos + rightAxis * 200);
	DrawStringF(strPos.x, strPos.y, "���J�����̐�����]��", 0xffffff);
}

void Camera::SetTargetPos(const Vector3& targetPos)
{
	m_targetPos = targetPos;
}

void Camera::RotateCameraUpVecY(const float rad)
{
	m_targetToCamera = VTransformSR(m_targetToCamera, MGetRotY(rad));
}

void Camera::RotateCameraV(const float rad)
{
	// �܂���]������ɓ����
	// �]�u�s�������Ă���͉̂�]�������t�ɂ��邽��
	// �t�s����y���̂�
	// �Ȃ�ŋt���������̂��͕������
	const MATRIX camMat = MTranspose(GetCameraViewMatrix());
	// �J�����ɂƂ��Ă�X���@���擾���Ă���
	const Vector3 rightAxis = {camMat.m[0][0],camMat.m[0][1],camMat.m[0][2]};

	// ���ȏ�}�Ȋp�x�ɂȂ�Ȃ��悤�ɂ���
	// ��]���悤�Ƃ��Ă���������A�p�x�����ɖ߂������Ȃ狖��
	const Vector3 unitTargetToCamera   = m_targetToCamera.GetNormalize();
	const Vector3 unitTargetToCameraXZ = {unitTargetToCamera.x, 0, unitTargetToCamera.z};
	const float dot = unitTargetToCamera.Dot(unitTargetToCameraXZ);

	if (dot < kCameraVRotPlusThreshold && rad > 0 && m_targetToCamera.y > 0) return;
	if (dot < kCameraVRotMinusThreshold && rad < 0 && m_targetToCamera.y < 0) return;

	// ��]���ɂ����ĉ�]
	m_targetToCamera = VTransform(m_targetToCamera, MGetRotAxis(rightAxis, rad));
}

Vector3 Camera::GetCameraDir() const
{
	return (-m_targetToCamera).GetNormalize();
}

Vector3 Camera::GetCameraDirXZ() const
{
	const Vector3 temp = GetCameraDir();
	return {temp.x, 0, temp.z};
}

Vector3 Camera::RotateVecToCameraDirXZ(const Vector3& vec, const Vector3& vec2)
{
	const MATRIX camMat = MGetRotVec2(vec2, GetCameraDirXZ());
	return VTransformSR(vec, camMat);
}