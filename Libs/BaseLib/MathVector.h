
//--------------------------------------------------------------------------------------
//	MathVector.h
//
//	ベクトル計算の計算クラス
//	XNAMATH のラッピングクラス群
//
//	Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
//	DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
//--------------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"

namespace basedx11{

	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix4X4;

	//--------------------------------------------------------------------------------------
	//	struct Vector2 : public XMFLOAT2;
	//	用途: 2次元XMVECTORのラッピング構造体
	//--------------------------------------------------------------------------------------
	struct Vector2 : public XMFLOAT2 {
		//--------------------------------------------------------------------------------------
		//	Vector2();
		//	用途: コンストラクタ
		//	戻り値: なし
		//	＊複数の初期化方法の定義
		//--------------------------------------------------------------------------------------
		Vector2() :XMFLOAT2() {
			x = 0;
			y = 0;
		}
		Vector2(const XMFLOAT2& v2) :XMFLOAT2(v2) {
		}
		Vector2(const XMVECTOR& other) :XMFLOAT2() {
			XMVECTOR temp = other;
			XMStoreFloat2(this, temp);
		}
		Vector2(float x, float y) :
			XMFLOAT2(x, y) {
		}
		Vector2(const Vector2& other) :XMFLOAT2() {
			x = other.x;
			y = other.y;
		}
		Vector2(wstring& x_str, wstring& y_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT2 temp = *this;
			XMVECTOR Vec = XMLoadFloat2(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
		}
		void Set(float xx, float yy) {
			x = xx;
			y = yy;
		}
		void Zero() {
			x = 0;
			y = 0;
		}
		//--------------------------------------------------------------------------------------
		//	多重定義
		//--------------------------------------------------------------------------------------
		//代入
		Vector2& operator=(const Vector2& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
			}
			return *this;
		}
		Vector2& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat2(this, temp);
			return *this;
		}
		//追加演算
		Vector2& operator+=(const Vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2& operator-=(const Vector2& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2& operator*=(float val) {
			x *= val;
			y *= val;
			return *this;
		}
		Vector2& operator/=(float val) {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector2::operator/=()"
					);
			}
			x /= val;
			y /= val;
			return *this;
		}
		//単項演算子
		Vector2 operator+ () const {
			return *this;
		}
		Vector2 operator- () const {
			Vector2 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//演算
		Vector2 operator+(const Vector2& other)const {
			Vector2 ret = *this;
			ret += other;
			return ret;
		}
		Vector2 operator-(const Vector2& other)const {
			Vector2 ret = *this;
			ret -= other;
			return ret;
		}
		Vector2 operator*(float val)const {
			Vector2 ret = *this;
			ret *= val;
			return ret;
		}
		Vector2 operator/(float val)const {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector4::operator/()"
					);
			}
			Vector2 ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](int i) {
			if (i > 1 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 1 || i < 0)",
					L"Vector2::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			}
		}
		const float& operator[](int i) const {
			if (i > 1 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 1 || i < 0)",
					L"Vector2::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			}
		}

		//比較
		//--------------------------------------------------------------------------------------
		//	bool Equal(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトルと等しいかどうかを検証する
		//	戻り値: 等しければtrue
		//--------------------------------------------------------------------------------------
		bool Equal(const Vector2& other)const {
			return XMVector2Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector2& other)const {
			return Equal(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool EqualInt(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトルと等しいかどうかを検証する。
		//	それぞれの要素は符号なし整数として扱う。
		//	戻り値: 等しければtrue
		//--------------------------------------------------------------------------------------
		bool EqualInt(const Vector2& other)const {
			return XMVector2EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		//	bool Greater(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトルより大きいかどうかを検証する。
		//	戻り値: 大きければtrue
		//--------------------------------------------------------------------------------------
		bool Greater(const Vector2& other)const {
			return XMVector2Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//より大きいか比較
		bool operator>(const Vector2& other)const {
			return Greater(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool GreaterOrEqual(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトル以上の大きさかどうかを検証する。
		//	戻り値: 以上の大きさならばtrue
		//--------------------------------------------------------------------------------------
		bool GreaterOrEqual(const Vector2& other)const {
			return XMVector2GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以上の大きさか比較
		bool operator >= (const Vector2& other)const {
			return GreaterOrEqual(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool IsInfinite() const;
		//	用途: thisの値のいずれかが正か負の無限大かどうかを検証する。
		//	戻り値: 値のいずれかが正か負の無限大ならばtrue
		//--------------------------------------------------------------------------------------
		bool IsInfinite() const {
			return XMVector2IsInfinite(XMVECTOR(*this));
		}
		//--------------------------------------------------------------------------------------
		//	bool IsIsNaN() const;
		//	用途: thisの値のいずれかが非数（無効値）かどうかを検証する。
		//	戻り値: 値のいずれかが非数（無効値）ならばtrue
		//--------------------------------------------------------------------------------------
		bool IsIsNaN() const {
			return XMVector2IsNaN(XMVECTOR(*this));
		}
		//--------------------------------------------------------------------------------------
		//	bool Less(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトルより小さいかどうかを検証する。
		//	戻り値: 小さければtrue
		//--------------------------------------------------------------------------------------
		bool Less(const Vector2& other)const {
			return XMVector2Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//より小さいか比較
		bool operator<(const Vector2& other)const {
			return Less(other);
		}
		//--------------------------------------------------------------------------------------
		//	bool LessOrEqual(
		//	const Vector2& other	//比較するベクトル
		//	) const;
		//	用途: thisがもう一つのベクトル以下の大きさかどうかを検証する。
		//	戻り値: 以下の大きさならばtrue
		//--------------------------------------------------------------------------------------
		bool LessOrEqual(const Vector2& other)const {
			return XMVector2LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以下の大きさか比較
		bool operator <= (const Vector2& other)const {
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector2& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		) const;
		用途: thisがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		bool NearEqual(const Vector2& other, float Epsilon)const {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector2NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector2& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqual(const Vector2& other)const {
			return XMVector2NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//比較
		bool operator!=(const Vector2& other)const {
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector2& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqualInt(const Vector2& other)const {
			return XMVector2NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector2& other	//もう一つのベクトル
		)const;
		用途: 正規化したthisともう一つのつの正規化したベクトルとの
		　　　ラジアン角を計算し返す
		   戻り値: ラジアン角
		   ＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		   ＊ベクトルのラジアン角計算では、正規化しない分速い
		   ＊thisは変更しない
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector2& other)const {
			return ((Vector2)XMVector2AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector2& other	//比較するベクトル
		)const;
		用途: 正規化されたthisと正規化されたもう一つのつのベクトル間のラジアン角を予測して返す。
		戻り値: ラジアン角
		＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		＊thisは変更しない
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector2& other)const {
			return ((Vector2)XMVector2AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector2& other	//比較するベクトル
		)const;
		用途: thisともう一つのつのベクトル間のラジアン角を計算する。
		戻り値: ラジアン角
		＊thisは変更しない
		***************************************************************************/
		float AngleBetweenVectors(const Vector2& other)const {
			return ((Vector2)XMVector2AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: thisをベクトルの長さを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax) {
			*this = XMVector2ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector2& LengthMinV,		//長さの最小値(ベクトル)
		const Vector2& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: thisを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLengthV(const Vector2& LengthMinV, const Vector2& LengthMaxV) {
			*this = XMVector2ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		float Cross(
		const Vector2& other		//相手のベクトル
		)const;
		用途: thisとの外積を返す
		戻り値: 外積
		＊thisは変更しない
		***************************************************************************/
		float Cross(const Vector2& other)const {
			return ((Vector2)XMVector2Cross(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float Dot(
		const Vector2& other	//相手のベクトル
		)const;
		用途: thisとの内積を返す
		戻り値: 内積
		＊thisは変更しない
		***************************************************************************/
		float Dot(const Vector2& other)const {
			return ((Vector2)XMVector2Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector2& other	//比較するベクトル
		)const;
		用途: thisのすべての要素が、設定された境界内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		＊thisは変更しない
		***************************************************************************/
		bool InBounds(const Vector2& other)const {
			return XMVector2InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		＊thisは変更しない
		***************************************************************************/
		float Length() const {
			return  ((Vector2)XMVector2Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		＊thisは変更しない
		***************************************************************************/
		float LengthEst() const {
			return ((Vector2)XMVector2LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		＊thisは変更しない
		***************************************************************************/
		float LengthSq() const {
			return ((Vector2)XMVector2LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		用途: 正規化する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Normalize() {
			*this = XMVector2Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		用途: 正規化の予想値を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void NormalizeEst() {
			*this = XMVector2NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		用途: thisに垂直なベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Orthogonal() {
			*this = XMVector2Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		用途: thisの長さの逆数を返す
		戻り値: thisの長さの逆数
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLength()const {
			return ((Vector2)XMVector2ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		用途: thisの長さの逆数の予測値を返す
		戻り値: thisの長さの逆数の予測値
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLengthEst()const {
			return ((Vector2)XMVector2ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector2& Normal
		);
		用途: thisに法線ベクトルによって反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Reflect(const Vector2& Normal) {
			*this = XMVector2Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector2& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Refract(const Vector2& Normal, float RefractionIndex) {
			*this = XMVector2Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector2& Normal,	//屈折させる法線ベクトル
		const Vector2& RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を設定する
		   戻り値: なし
		   ＊thisに結果を設定する
		   ***************************************************************************/
		void RefractV(const Vector2& Normal, const Vector2& RefractionIndex) {
			*this = XMVector2RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transform(const XMMATRIX& m) {
			*this = XMVector2Transform(XMVECTOR(*this), m);
		}
		/**************************************************************************
		void Transform(
		const Matrix4X4& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void Transform(const Matrix4X4& m);
	};

	/**************************************************************************
	struct Vector3 : public XMFLOAT3;
	用途: 3次元XMVECTORのラッピング構造体
	****************************************************************************/
	struct Vector3 : public XMFLOAT3 {
		/**************************************************************************
		Vector3();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Vector3() :XMFLOAT3() {
			x = 0;
			y = 0;
			z = 0;
		}
		Vector3(const XMFLOAT3& v3) :XMFLOAT3(v3) {
		}
		Vector3(const XMVECTOR& other) :XMFLOAT3() {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
		}
		Vector3(float x, float y, float z) :
			XMFLOAT3(x, y, z) {
		}
		Vector3(const Vector3& other) :XMFLOAT3() {
			x = other.x;
			y = other.y;
			z = other.z;
		}
		Vector3(wstring& x_str, wstring& y_str, wstring& z_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT3 temp = *this;
			XMVECTOR Vec = XMLoadFloat3(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
		}
		void Set(float xx, float yy, float zz) {
			x = xx;
			y = yy;
			z = zz;
		}
		void Zero() {
			x = 0;
			y = 0;
			z = 0;
		}
		/**************************************************************************
		多重定義
		***************************************************************************/
		//代入
		Vector3& operator=(const Vector3& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
			}
			return *this;
		}
		Vector3& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
			return *this;
		}
		//追加演算
		Vector3& operator+=(const Vector3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3& operator*=(float val) {
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}
		Vector3& operator/=(float val) {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector3::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}
		//単項演算子
		Vector3 operator+ () const {
			return *this;
		}
		Vector3 operator- () const {
			Vector3 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//演算
		Vector3 operator+(const Vector3& other)const {
			Vector3 ret = *this;
			ret += other;
			return ret;
		}
		Vector3 operator-(const Vector3& other)const {
			Vector3 ret = *this;
			ret -= other;
			return ret;
		}
		Vector3 operator*(float val)const {
			Vector3 ret = *this;
			ret *= val;
			return ret;
		}
		Vector3 operator/(float val)const {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector3::operator/()"
					);
			}
			Vector3 ret = *this;
			ret /= val;
			return ret;
		}
		float& operator[](int i) {
			if (i > 2 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 2 || i < 0)",
					L"Vector3::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			}
			return x;
		}
		const float& operator[](int i) const {
			if (i > 2 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 2 || i < 0)",
					L"Vector3::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			}
			return x;
		}
		//比較
		/**************************************************************************
		bool Equal(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		bool Equal(const Vector3& other)const {
			return XMVector3Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector3& other)const {
			return Equal(other);
		}
		/**************************************************************************
		bool EqualInt(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しいかどうかを検証する。
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しければtrue
		***************************************************************************/
		bool EqualInt(const Vector3& other)const {
			return XMVector3EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		bool Greater(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルより大きいかどうかを検証する。
		戻り値: 大きければtrue
		***************************************************************************/
		bool Greater(const Vector3& other)const {
			return XMVector3Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//より大きいか比較
		bool operator>(const Vector3& other)const {
			return Greater(other);
		}
		/**************************************************************************
		bool GreaterOrEqual(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトル以上の大きさかどうかを検証する。
		戻り値: 以上の大きさならばtrue
		***************************************************************************/
		bool GreaterOrEqual(const Vector3& other)const {
			return XMVector3GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以上の大きさか比較
		bool operator >= (const Vector3& other)const {
			return GreaterOrEqual(other);
		}
		/**************************************************************************
		bool IsInfinite() const;
		用途: thisの値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		bool IsInfinite() const {
			return XMVector3IsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsIsNaN() const;
		用途: thisの値のいずれかが非数（無効値）かどうかを検証する。
		戻り値: 値のいずれかが非数（無効値）ならばtrue
		***************************************************************************/
		bool IsIsNaN() const {
			return XMVector3IsNaN(XMVECTOR(*this));
		}
		/**************************************************************************
		bool Less(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルより小さいかどうかを検証する。
		戻り値: 小さければtrue
		***************************************************************************/
		bool Less(const Vector3& other)const {
			return XMVector3Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//より小さいか比較
		bool operator<(const Vector3& other)const {
			return Less(other);
		}
		/**************************************************************************
		bool LessOrEqual(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトル以下の大きさかどうかを検証する。
		戻り値: 以下の大きさならばtrue
		***************************************************************************/
		bool LessOrEqual(const Vector3& other)const {
			return XMVector3LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以下の大きさか比較
		bool operator <= (const Vector3& other)const {
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector3& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		) const;
		用途: thisがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		bool NearEqual(const Vector3& other, float Epsilon)const {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector3NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqual(const Vector3& other)const {
			return XMVector3NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//比較
		bool operator!=(const Vector3& other)const {
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqualInt(const Vector3& other)const {
			return XMVector3NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector3& other	//もう一つのベクトル
		) const;
		用途: 正規化したthisともう一つのつの正規化したベクトルとの
		　　　ラジアン角を計算設定する。
		   戻り値: ラジアン角
		   ＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		   ＊ベクトルのラジアン角計算では、正規化しない分速い
		   ＊thisは変更しない
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector3& other) const {
			return ((Vector3)XMVector3AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector3& other	//比較するベクトル
		)const;
		用途: 正規化されたthisと正規化されたもう一つのつのベクトル間のラジアン角を予測設定する。
		戻り値: ラジアン角
		＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		＊thisは変更しない
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector3& other)const {
			return ((Vector3)XMVector3AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisともう一つのつのベクトル間のラジアン角を計算する。
		戻り値: なし
		＊thisは変更しない
		***************************************************************************/
		float AngleBetweenVectors(const Vector3& other) const {
			return ((Vector3)XMVector3AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: thisをベクトルの長さを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax) {
			*this = XMVector3ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector3& LengthMinV,		//長さの最小値(ベクトル)
		const Vector3& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: thisを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLengthV(const Vector3& LengthMinV, const Vector3& LengthMaxV) {
			*this = XMVector3ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		void Cross(
		const Vector3& other,		//相手のベクトル１
		);
		用途: thisとの外積を計算しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Cross(const Vector3& other) {
			*this = (Vector3)XMVector3Cross(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Dot(
		const Vector3& other	//相手のベクトル
		)const;
		用途: thisとの内積を計算する
		戻り値: 内積
		＊thisは変更しない
		***************************************************************************/
		float Dot(const Vector3& other)const {
			return ((Vector3)XMVector3Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector3& other	//比較するベクトル
		) const;
		用途: thisのすべての要素が、設定された境界内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		＊thisは変更しない
		***************************************************************************/
		bool InBounds(const Vector3& other) const {
			return XMVector3InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		＊thisは変更しない
		***************************************************************************/
		float Length() const {
			return ((Vector3)XMVector3Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		＊thisは変更しない
		***************************************************************************/
		float LengthEst() const {
			return ((Vector3)XMVector3LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		＊thisは変更しない
		***************************************************************************/
		float LengthSq() const {
			return ((Vector3)XMVector3LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		用途: 正規化する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Normalize() {
			*this = XMVector3Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		用途: 正規化の予想値を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void NormalizeEst() {
			*this = XMVector3NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		用途: thisに垂直なベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Orthogonal() {
			*this = XMVector3Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		用途: thisの長さの逆数を返す
		戻り値: thisの長さの逆数
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLength()const {
			return ((Vector3)XMVector3ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		用途: thisの長さの逆数の予測値を返す
		戻り値: thisの長さの逆数の予測値
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLengthEst()const {
			return ((Vector3)XMVector3ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector3& Normal
		);
		用途: thisに法線ベクトルによって反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Reflect(const Vector3& Normal) {
			*this = XMVector3Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector3& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Refract(const Vector3& Normal, float RefractionIndex) {
			*this = XMVector3Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector3& Normal,	//屈折させる法線ベクトル
		const Vector3& RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を設定する
		   戻り値: なし
		   ＊thisに結果を設定する
		   ***************************************************************************/
		void RefractV(const Vector3& Normal, const Vector3& RefractionIndex) {
			*this = XMVector3RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transform(const XMMATRIX& m) {
			*this = XMVector3Transform(XMVECTOR(*this), m);
		}
		/**************************************************************************
		inline void Transform(
		const Matrix4X4& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void Transform(const Matrix4X4& m);
	};


	/**************************************************************************
	struct Vector4 : public XMFLOAT4;
	用途: 4次元XMVECTORのラッピング構造体
	****************************************************************************/
	struct Vector4 : public XMFLOAT4 {
		/**************************************************************************
		Vector4();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Vector4() :XMFLOAT4() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		Vector4(const XMFLOAT4& v4) :XMFLOAT4(v4) {
		}
		Vector4(const XMVECTOR& other) :XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		Vector4(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w) {
		}
		Vector4(const Vector4& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		Vector4(const Vector3& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
		}
		Vector4(const Vector3& other, float ww) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = ww;
		}
		Vector4(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		void Set(float xx, float yy, float zz, float ww) {
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}
		void Zero() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		/**************************************************************************
		多重定義
		***************************************************************************/
		//代入
		Vector4& operator=(const Vector4& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		Vector4& operator=(const Vector3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
			return *this;
		}
		Vector4& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//追加演算
		Vector4& operator+=(const Vector4& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		Vector4& operator-=(const Vector4& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		Vector4& operator*=(float val) {
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		Vector4& operator/=(float val) {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector4::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}
		//単項演算子
		Vector4 operator+ () const {
			return *this;
		}
		Vector4 operator- () const {
			Vector4 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//演算
		Vector4 operator+(const Vector4& other)const {
			Vector4 ret = *this;
			ret += other;
			return ret;
		}
		Vector4 operator-(const Vector4& other)const {
			Vector4 ret = *this;
			ret -= other;
			return ret;
		}
		Vector4 operator*(float val)const {
			Vector4 ret = *this;
			ret *= val;
			return ret;
		}
		Vector4 operator/(float val)const {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Vector4::operator/()"
					);
			}
			Vector4 ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](int i) {
			if (i > 3 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 3 || i < 0)",
					L"Vector4::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		const float& operator[](int i) const {
			if (i > 2 || i < 0) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 3 || i < 0)",
					L"Vector4::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}

		//比較
		/**************************************************************************
		bool Equal(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		bool Equal(const Vector4& other)const {
			return XMVector4Equal(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Vector4& other)const {
			return Equal(other);
		}
		/**************************************************************************
		bool EqualInt(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しいかどうかを検証する。
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しければtrue
		***************************************************************************/
		bool EqualInt(const Vector4& other)const {
			return XMVector4EqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		bool Greater(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルより大きいかどうかを検証する。
		戻り値: 大きければtrue
		***************************************************************************/
		bool Greater(const Vector4& other)const {
			return XMVector4Greater(XMVECTOR(*this), XMVECTOR(other));
		}
		//より大きいか比較
		bool operator>(const Vector4& other)const {
			return Greater(other);
		}
		/**************************************************************************
		bool GreaterOrEqual(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトル以上の大きさかどうかを検証する。
		戻り値: 以上の大きさならばtrue
		***************************************************************************/
		bool GreaterOrEqual(const Vector4& other)const {
			return XMVector4GreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以上の大きさか比較
		bool operator >= (const Vector4& other)const {
			return GreaterOrEqual(other);
		}
		/**************************************************************************
		bool IsInfinite() const;
		用途: thisの値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		bool IsInfinite() const {
			return XMVector4IsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsIsNaN() const;
		用途: thisの値のいずれかが非数（無効値）かどうかを検証する。
		戻り値: 値のいずれかが非数（無効値）ならばtrue
		***************************************************************************/
		bool IsIsNaN() const {
			return XMVector4IsNaN(XMVECTOR(*this));
		}
		/**************************************************************************
		bool Less(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルより小さいかどうかを検証する。
		戻り値: 小さければtrue
		***************************************************************************/
		bool Less(const Vector4& other)const {
			return XMVector4Less(XMVECTOR(*this), XMVECTOR(other));
		}
		//より小さいか比較
		bool operator<(const Vector4& other)const {
			return Less(other);
		}
		/**************************************************************************
		bool LessOrEqual(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトル以下の大きさかどうかを検証する。
		戻り値: 以下の大きさならばtrue
		***************************************************************************/
		bool LessOrEqual(const Vector4& other)const {
			return XMVector4LessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//以下の大きさか比較
		bool operator <= (const Vector4& other)const {
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool NearEqual(
		const Vector4& other,	//比較するベクトル
		float Epsilon			//比較する最大許容差
		) const;
		用途: thisがもう一つのベクトルが近い大きさかどうかを検証する。
		戻り値: 近い大きさならばtrue
		***************************************************************************/
		bool NearEqual(const Vector4& other, float Epsilon)const {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector4NearEqual(XMVECTOR(*this), XMVECTOR(other), VecEp);
		}
		/**************************************************************************
		bool NotEqual(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqual(const Vector4& other)const {
			return XMVector4NotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		//比較
		bool operator!=(const Vector4& other)const {
			return NotEqual(other);
		}
		/**************************************************************************
		bool NotEqualInt(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのベクトルと等しくないかどうかを検証する
		それぞれの要素は符号なし整数として扱う。
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqualInt(const Vector4& other)const {
			return XMVector4NotEqualInt(XMVECTOR(*this), XMVECTOR(other));
		}
		//ジオメトリ
		/**************************************************************************
		float AngleBetweenNormals(
		const Vector4& other	//もう一つのベクトル
		)const;
		用途: 正規化したthisともう一つのつの正規化したベクトルとの
		　　　ラジアン角を返す。
		   戻り値: ラジアン角
		   ＊thisは変更しない
		   ＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		   ＊ベクトルのラジアン角計算では、正規化しない分速い
		   ***************************************************************************/
		float AngleBetweenNormals(const Vector4& other)const {
			return ((Vector4)XMVector4AngleBetweenNormals(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenNormalsEst(
		const Vector4& other	//比較するベクトル
		)const;
		用途: 正規化されたthisと正規化されたもう一つのつのベクトル間のラジアン角を返す
		戻り値: ラジアン角
		＊thisは変更しない
		＊正規化はしないので、thisおよびotherはあらかじめ正規化しておく
		＊ベクトルのラジアン角計算では、正規化しない分速い
		***************************************************************************/
		float AngleBetweenNormalsEst(const Vector4& other)const {
			return ((Vector4)XMVector4AngleBetweenNormalsEst(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float AngleBetweenVectors(
		const Vector4& other	//比較するベクトル
		)const;
		用途: thisともう一つのつのベクトル間のラジアン角。
		戻り値: ラジアン角
		＊thisは変更しない
		***************************************************************************/
		float AngleBetweenVectors(const Vector4& other)const {
			return ((Vector4)XMVector4AngleBetweenVectors(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void ClampLength(
		float LengthMin,	//長さの最小値
		float LengthMax		//長さの最大値
		);
		用途: thisをベクトルの長さを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLength(float LengthMin, float LengthMax) {
			*this = XMVector4ClampLength(XMVECTOR(*this), LengthMin, LengthMax);
		}
		/**************************************************************************
		void ClampLengthV(
		const Vector4& LengthMinV,		//長さの最小値(ベクトル)
		const Vector4& LengthMaxV		//長さの最大値(ベクトル)
		);
		用途: thisを指定した大きさにクランプする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void ClampLengthV(const Vector4& LengthMinV, const Vector4& LengthMaxV) {
			*this = XMVector4ClampLengthV(XMVECTOR(*this), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		/**************************************************************************
		void Cross(
		const Vector4& other1,		//相手のベクトル１
		const Vector4& other2		//相手のベクトル２
		);
		用途: thisとの外積を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Cross(const Vector4& other1, const Vector4& other2) {
			*this = XMVector4Cross(XMVECTOR(*this), XMVECTOR(other1), XMVECTOR(other2));
		}
		/**************************************************************************
		float Dot(
		const Vector4& other	//相手のベクトル
		)const;
		用途: thisとの内積を計算する
		戻り値: 内積
		＊thisは変更しない
		***************************************************************************/
		float Dot(const Vector4& other)const {
			return ((Vector4)XMVector4Dot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool InBounds(
		const Vector4& other	//比較するベクトル
		) const;
		用途: thisのすべての要素が、設定された境界内にある場合はtrueを返す
		戻り値: 境界内にある場合はtrue
		＊thisは変更しない
		***************************************************************************/
		bool InBounds(const Vector4& other) const {
			return XMVector4InBounds(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		float Length() const;
		用途: ベクトルの長さを返す
		戻り値: ベクトルの長さ
		＊thisは変更しない
		***************************************************************************/
		float Length() const {
			return ((Vector4)XMVector4Length(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthEst() const;
		用途: ベクトルの長さを予想して返す
		戻り値: ベクトルの長さの予想
		＊thisは変更しない
		***************************************************************************/
		float LengthEst() const {
			return ((Vector4)XMVector4LengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		用途: ベクトルの長さの2乗を返す
		戻り値: ベクトルの長さの2乗
		＊thisは変更しない
		***************************************************************************/
		float LengthSq() const {
			return ((Vector4)XMVector4LengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Normalize();
		用途: 正規化する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Normalize() {
			*this = XMVector4Normalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		用途: 正規化の予想値を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void NormalizeEst() {
			*this = XMVector4NormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		void Orthogonal();
		用途: thisに垂直なベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Orthogonal() {
			*this = XMVector4Orthogonal(XMVECTOR(*this));
		}
		/**************************************************************************
		float ReciprocalLength()const;
		用途: thisの長さの逆数を返す
		戻り値: thisの長さの逆数
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLength()const {
			return ((Vector4)XMVector4ReciprocalLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float ReciprocalLengthEst()const;
		用途: thisの長さの逆数の予測値を返す
		戻り値: thisの長さの逆数の予測値
		＊thisは変更しない
		***************************************************************************/
		float ReciprocalLengthEst()const {
			return ((Vector4)XMVector4ReciprocalLengthEst(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Reflect(
		const Vector4& Normal
		);
		用途: thisに法線ベクトルによって反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Reflect(const Vector4& Normal) {
			*this = XMVector4Reflect(XMVECTOR(*this), XMVECTOR(Normal));
		}
		/**************************************************************************
		void Refract(
		const Vector4& Normal,	//屈折させる法線ベクトル
		float RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Refract(const Vector4& Normal, float RefractionIndex) {
			*this = XMVector4Refract(XMVECTOR(*this), XMVECTOR(Normal), RefractionIndex);
		}
		/**************************************************************************
		void RefractV(
		const Vector4& Normal,	//屈折させる法線ベクトル
		const Vector4& RefractionIndex	//屈折率
		);
		用途: thisに法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）
		　　　で反射させたベクトル(入射ベクトル)を設定する
		   戻り値: なし
		   ＊thisに結果を設定する
		   ***************************************************************************/
		void RefractV(const Vector4& Normal, const Vector4& RefractionIndex) {
			*this = XMVector4RefractV(XMVECTOR(*this), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//トランスフォーム
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transform(const XMMATRIX& m) {
			*this = XMVector4Transform(XMVECTOR(*this), m);
		}
		/**************************************************************************
		void Transform(
		const Matrix4X4& m	//トランスフォームする行列
		);
		用途: thisをmによってトランスフォームされたベクトルを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void Transform(const Matrix4X4& m);
	};

	//--------------------------------------------------------------------------------------
	//	struct XMVector : public XMFLOAT4;
	//	用途: 1次元XMVECTORのラッピング構造体
	//	＊各要素の操作など、基本的なXM関数群
	//--------------------------------------------------------------------------------------
	struct XMVector : public XMFLOAT4 {
		/**************************************************************************
		XMVector();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		XMVector() :XMFLOAT4() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		XMVector(const XMFLOAT4& v4) :XMFLOAT4(v4) {
		}
		XMVector(const XMVECTOR& other) :XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		XMVector(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w) {
		}
		XMVector(const XMVector& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		XMVector(const Vector4& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		XMVector(const Vector3& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
		}
		XMVector(const Vector3& other, float ww) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = ww;
		}
		XMVector(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		void Set(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			x = (float)_wtof(x_str.c_str());
			y = (float)_wtof(y_str.c_str());
			z = (float)_wtof(z_str.c_str());
			w = (float)_wtof(w_str.c_str());
		}
		void Set(float xx, float yy, float zz, float ww) {
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}
		void Zero() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		/**************************************************************************
		多重定義
		***************************************************************************/
		//代入
		XMVector& operator=(const XMVector& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		XMVector& operator=(const Vector3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = 0;
			return *this;
		}
		XMVector& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//追加演算
		XMVector& operator+=(const XMVector& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		XMVector& operator-=(const XMVector& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		XMVector& operator*=(float val) {
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		XMVector& operator/=(float val) {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"VectorBase::operator/=()"
					);
			}
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}
		//単項演算子
		XMVector operator+ () const {
			return *this;
		}
		XMVector operator- () const {
			XMVector ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//演算
		XMVector operator+(const XMVector& other)const {
			XMVector ret = *this;
			ret += other;
			return ret;
		}
		XMVector operator-(const XMVector& other)const {
			XMVector ret = *this;
			ret -= other;
			return ret;
		}
		XMVector operator*(float val)const {
			XMVector ret = *this;
			ret *= val;
			return ret;
		}
		XMVector operator/(float val)const {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"VectorBase::operator/()"
					);
			}
			XMVector ret = *this;
			ret /= val;
			return ret;
		}

		float& operator[](size_t i) {
			if (i > 3) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 3)",
					L"XMVector::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		float operator[](size_t i) const {
			if (i > 3) {
				throw BaseException(
					L"インデックスが範囲外です",
					L"if(i > 3)",
					L"XMVector::operator[]()"
					);
			}
			switch (i) {
			case 0:
				return x;
				break;
			case 1:
				return y;
				break;
			case 2:
				return z;
				break;
			case 3:
				return w;
				break;
			}
		}
		//算術
		/**************************************************************************
		void Abs();
		用途: 各要素を絶対値に設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Abs() {
			*this = XMVectorAbs(XMVECTOR(*this));
		}
		/**************************************************************************
		void Add(
		const XMVector& other	//計算相手
		);
		用途: 各要素にotherの各要素を加算する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Add(const XMVector& other) {
			*this = XMVectorAdd(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void AddAngles(
		const XMVector& other	//計算相手
		);
		用途: 各要素にotherの各要素角度加算する
		戻り値: なし
		＊thisに結果を設定する
		＊thisとotherは、-XM_PI以上XM_PI未満である必要がある
		***************************************************************************/
		void AddAngles(const XMVector& other) {
			*this = XMVectorAddAngles(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void Ceiling(
		const XMVector& other	//計算相手
		);
		用途: 各要素の上限を計算し、設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Ceiling(const XMVector& other) {
			*this = XMVectorCeiling(XMVECTOR(other));
		}


	};
}
//end of namespace basedx11.
