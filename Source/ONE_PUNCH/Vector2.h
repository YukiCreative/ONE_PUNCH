#pragma once

struct Vector3;

struct Vector2
{
public:
	float x, y;
public:
	Vector2();
	Vector2(const float _x, const float _y);

	explicit operator Vector3();

	void    operator+=(const Vector2& other);
	Vector2 operator+ (const Vector2& other) const;
	void    operator-=(const Vector2& other);
	Vector2 operator- (const Vector2& other) const;
	void    operator*=(const float other);
	Vector2 operator* (const float other) const;
	void    operator/=(const float other);
	Vector2 operator/ (const float other) const;

	// {0,0}
	static const Vector2  Zero();
	// {0,1}
	static const Vector2    Up();
	// {0,-1}
	static const Vector2  Down();
	// {1,0}
	static const Vector2 Right();
	// {-1,0}
	static const Vector2  Left();

	Vector2 GetNormalize() const;
	// �����𐳋K��
	void Normalize();
	float SqrMagnitude() const;
	float Magnitude() const;
	float Dot(const Vector2& other) const;
	// ��������_
	Vector2 Lerp(const Vector2& targetPos, const float time) const;
	void LerpMyself(const Vector2& targetPos, const float time);
	// X���������n���Ƃ����p�x
	// �[���x�N�g����0�x�ɂȂ�͎̂d�l
	// ��F{1,1}=45��
	float Angle() const;
	// �����ɓ����ꂽ�x�N�g���Ƃ̂Ȃ��p���o���܂�
	float RerativeAngle(const Vector2& other) const;
};