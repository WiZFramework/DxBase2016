
/*!
@file MathExt.h
@brief 行列、クオータニオン、カラーなどの計算クラス<br />
XNAMATH のラッピングクラス群

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

//--------------------------------------------------------------------------------------
// MathExt.h
//
// 行列、クオータニオン、カラーなどの計算クラス
// XNAMATH のラッピングクラス群
//
// Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
// DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
//--------------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"

namespace basedx11{

	struct Matrix4X4;

	/**************************************************************************
	struct Plane : public XMFLOAT4;
	用途: Plane（平面）構造体
	****************************************************************************/
	struct Plane : public XMFLOAT4 {
		/**************************************************************************
		Plane();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Plane() :XMFLOAT4() {}
		Plane(const XMFLOAT4& v4) :XMFLOAT4() {
			x = v4.x;
			y = v4.y;
			z = v4.z;
			w = v4.w;
		}
		Plane(const Plane& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		Plane(const XMVECTOR& other) :XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		Plane(const Vector3& Point, const Vector3& Normal) {
			*this = (Plane)XMPlaneFromPointNormal(XMVECTOR(Point), XMVECTOR(Normal));
		}
		Plane(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) {
			*this = (Plane)XMPlaneFromPoints(XMVECTOR(Point1), XMVECTOR(Point2), XMVECTOR(Point3));
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
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
		Plane& operator=(const Plane& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		Plane& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		/**************************************************************************
		float Dot(
		const Vector4& other	//計算するVector4
		)const;
		用途: thisとotherの内積を返す
		戻り値: 内積
		＊thisは変更しない
		***************************************************************************/
		float Dot(const Vector4& other) {
			return ((Vector4)XMPlaneDot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float DotCoord(
		const Vector3& other	//相手のVector3
		) const;
		用途: thisとVector3の内積を返す
		戻り値: 内積
		＊thisは変更しない
		＊あるポイントから平面までの符号付き距離を求めるのに便利である
		***************************************************************************/
		float DotCoord(const Vector3& other) {
			return  ((Vector4)XMPlaneDotCoord(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		float DotNormal(
		const Vector3& other	//相手のVector3
		) const;
		用途:　thisの法線ベクトルと 3D ベクトルの間の内積を計算し返す
		戻り値: 内積
		＊thisは変更しない
		＊平面の法線ベクトルと別の法線ベクトルとの角度を算出するときに役立つ
		***************************************************************************/
		float DotNormal(const Vector3& other)const {
			return ((Vector4)XMPlaneDotNormal(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		bool Equal(
		const Plane& other	//相手のPlane
		)const;
		用途:　thisと相手の平面が同じかどうかを調べる
		戻り値: 同じならtrue
		＊thisは変更しない
		***************************************************************************/
		bool Equal(const Plane& other)const {
			return XMPlaneEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void FromPointNormal(
		const Vector3& Point,	//平面上の点
		const Vector3& Normal	//法線
		);
		用途:　点と法線から平面を作成して、thisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void FromPointNormal(const Vector3& Point, const Vector3& Normal) {
			*this = (Plane)XMPlaneFromPointNormal(XMVECTOR(Point), XMVECTOR(Normal));
		}
		/**************************************************************************
		void FromPoints(
		const Vector3& Point1,	//頂点1
		const Vector3& Point2,	//頂点2
		const Vector3& Point3	//頂点3
		);
		用途:　３点から平面を作成して、thisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void FromPoints(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) {
			*this = (Plane)XMPlaneFromPoints(XMVECTOR(Point1), XMVECTOR(Point2), XMVECTOR(Point3));
		}
		/**************************************************************************
		Vector3 IntersectLine(
		const Vector3& LinePoint1,	//頂点1
		const Vector3& LinePoint2	//頂点2
		)const;
		用途:　thisと２つの頂点が作成する直線の交差する頂点を返す
		戻り値: thisと交差する３Ｄベクトル
		＊thisは変更しない
		＊平面と直線が平行の場合は、戻る各値はQNaN（非数） になるので、
		　Vector3::IsNaN()などでチェックする
		 ***************************************************************************/
		Vector3 IntersectLine(const Vector3& LinePoint1, const Vector3& LinePoint2)const {
			return (Vector3)XMPlaneIntersectLine(XMVECTOR(*this),
				XMVECTOR(LinePoint1), XMVECTOR(LinePoint2));
		}
		/**************************************************************************
		void IntersectPlane(
		Vector3& RetLinePoint1,	//返される頂点1
		Vector3& RetLinePoint2,	//返される頂点2
		const Plane& other		//thisと比べられるもう一つの面
		)const;
		用途:　thisともう一つの面が交差する2つの頂点を計算する
		戻り値: なし
		＊thisは変更しない
		＊RetLinePoint1とRetLinePoint2に交差する頂点を返す
		＊2つの平面が平行の場合は、戻る各値はQNaN（非数） になるので、
		　Vector3::IsNaN()などでチェックする
		 ***************************************************************************/
		void IntersectPlane(Vector3& RetLinePoint1, Vector3& RetLinePoint2,
			const Plane& other)const {
			XMVECTOR Temp1, Temp2;
			XMPlaneIntersectPlane(&Temp1, &Temp2, XMVECTOR(*this), XMVECTOR(other));
			RetLinePoint1 = (Vector3)Temp1;
			RetLinePoint2 = (Vector3)Temp2;
		}
		/**************************************************************************
		bool IsInfinite()const;
		用途:　thisの要素のいずれかが正または負の無限大かどうかを調べる
		戻り値: 正または負の無限大ならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsInfinite()const {
			return XMPlaneIsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsNaN()const;
		用途:　thisの要素のいずれかが NaNかどうかを調べる
		戻り値: NaNならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsNaN()const {
			return XMPlaneIsNaN(XMVECTOR(*this));
		}
		/**************************************************************************
		bool NearEqual(
		const Plane& other		//thisと比べられるもう一つの面
		const Vector4& Epsilon	//各要素の許容範囲
		)const;
		用途:　thisともう一つの平面がほぼ等しいかどうか調べる
		戻り値: ほぼ等しければtrue
		＊thisは変更しない
		***************************************************************************/
		bool NearEqual(const Plane& other, const Vector4& Epsilon)const {
			return XMPlaneNearEqual(XMVECTOR(*this), XMVECTOR(other), Epsilon);
		}
		/**************************************************************************
		void Normalize();
		用途:thisを正規化する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Normalize() {
			*this = (Plane)XMPlaneNormalize(XMVECTOR(*this));
		}
		/**************************************************************************
		void NormalizeEst();
		用途:thisをx,y,zの係数が単位の法線ベクトルを形成するように、平面の係数を予測
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void NormalizeEst() {
			*this = (Plane)XMPlaneNormalizeEst(XMVECTOR(*this));
		}
		/**************************************************************************
		bool NotEqual(
		const Plane& other	//相手のPlane
		)const;
		用途:　thisと相手の平面が等しくないかどうかを調べる
		戻り値: 等しくないならtrue
		＊thisは変更しない
		***************************************************************************/
		bool NotEqual(const Plane& other)const {
			return XMPlaneNotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void Transform(
		const XMMATRIX& m	//トランスフォームする行列
		);
		用途:thisをmでトランスフォームする
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transform(const XMMATRIX& m) {
			*this = (Plane)XMPlaneTransform(XMVECTOR(*this), m);
		}

		//　XMPlaneTransformStream関数は、ラッピング関数は用意しない
		//　理由：パラメータに配列を渡す必要があり、ラッピングによって、最適化が損なわれる
	};


	/**************************************************************************
	struct Color4 : public XMFLOAT4;
	用途: r、g、b、aの色を定義する構造体
	****************************************************************************/
	struct Color4 : public XMFLOAT4 {
		/**************************************************************************
		Color4();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Color4() :XMFLOAT4() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		Color4(const XMFLOAT4& v4) :XMFLOAT4(v4) {
		}
		Color4(const XMVECTOR& other) :XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		Color4(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w) {
		}
		Color4(const Color4& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
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
		Color4& operator=(const Color4& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		Color4& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//追加演算
		Color4& operator+=(const Color4& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		Color4& operator-=(const Color4& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		Color4& operator*=(float val) {
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		Color4& operator/=(float val) {
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
		Color4 operator+ () const {
			return *this;
		}
		Color4 operator- () const {
			Color4 ret = *this;
			ret *= -1.0f;
			return ret;
		}
		//演算
		Color4 operator+(const Color4& other)const {
			Color4 ret = *this;
			ret += other;
			return ret;
		}
		Color4 operator-(const Color4& other)const {
			Color4 ret = *this;
			ret -= other;
			return ret;
		}
		Color4 operator*(float val)const {
			Color4 ret = *this;
			ret *= val;
			return ret;
		}
		Color4 operator/(float val)const {
			if (val == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(val == 0.0f)",
					L"Color4::operator/()"
					);
			}
			Color4 ret = *this;
			ret /= val;
			return ret;
		}

		//個別アクセス
		float GetR() const {
			return x;
		}
		void SetR(float r) {
			x = r;
		}
		float GetG() const {
			return y;
		}
		void SetG(float g) {
			y = g;
		}
		float GetB() const {
			return z;
		}
		void SetB(float b) {
			z = b;
		}
		float GetA() const {
			return w;
		}
		void SetA(float a) {
			w = a;
		}
		void SetRGBA(float r, float g, float b, float a) {
			x = r;
			y = g;
			z = b;
			w = a;
		}
		/**************************************************************************
		bool Equal(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラーと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		bool Equal(const Color4& other)const {
			return XMColorEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Color4& other)const {
			return Equal(other);
		}
		/**************************************************************************
		bool NotEqual(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラーと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqual(const Color4& other)const {
			return XMColorNotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator!=(const Color4& other)const {
			return NotEqual(other);
		}

		/**************************************************************************
		bool Greater(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラーより大きいかどうかを検証する
		戻り値: 大きければtrue
		***************************************************************************/
		bool Greater(const Color4& other)const {
			return XMColorGreater(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator>(const Color4& other)const {
			return Greater(other);
		}
		/**************************************************************************
		bool GreaterOrEqual(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラー以上かどうかを検証する
		戻り値: 以上であればtrue
		＊thisは変更しない
		***************************************************************************/
		bool GreaterOrEqual(const Color4& other)const {
			return XMColorGreaterOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator>=(const Color4& other)const {
			return GreaterOrEqual(other);
		}
		/**************************************************************************
		bool LessOrEqual(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラー以下かどうかを検証する
		戻り値: 以下であればtrue
		＊thisは変更しない
		***************************************************************************/
		bool LessOrEqual(const Color4& other)const {
			return XMColorLessOrEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator<=(const Color4& other)const {
			return LessOrEqual(other);
		}
		/**************************************************************************
		bool Less(
		const Color4& other	//比較するベクトル
		) const;
		用途: thisがもう一つのカラーより小さいかどうかを検証する
		戻り値: より小さければtrue
		＊thisは変更しない
		***************************************************************************/
		bool Less(const Color4& other)const {
			return XMColorLess(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator<(const Color4& other)const {
			return Less(other);
		}
		/**************************************************************************
		bool IsInfinite()const;
		用途: thisのカラーのいずれかの成分が正または負の無限大かどうかをテストする
		戻り値: thisのカラーのいずれかの成分が正または負の無限大ならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsInfinite()const {
			return XMColorIsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsNaN();
		用途: thisのカラーのいずれかの成分がNaNかどうかをテストする
		戻り値: thisのカラーのいずれかの成分がNaNならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsNaN()const {
			return XMColorIsNaN(XMVECTOR(*this));
		}
		//操作
		/**************************************************************************
		void AdjustRange();
		用途: thisのカラーを0.0から1.0の範囲に調整しthisに設定する
		戻り値: なし
		＊範囲外の要素は0.0か1.0に設定される
		＊thisに結果を設定する
		***************************************************************************/
		void AdjustRange() {
			if (this->x <= 0) {
				this->x = 0;
			}
			if (this->y <= 0) {
				this->y = 0;
			}
			if (this->z <= 0) {
				this->z = 0;
			}
			if (this->w <= 0) {
				this->w = 0;
			}
			if (this->x >= 1.0f) {
				this->x = 1.0f;
			}
			if (this->y >= 1.0f) {
				this->y = 1.0f;
			}
			if (this->z >= 1.0f) {
				this->z = 1.0f;
			}
			if (this->w >= 1.0f) {
				this->w = 1.0f;
			}
		}
		/**************************************************************************
		void AdjustContrast(
		float Contrast		//コントラスト範囲
		);
		用途: thisのカラーのコントラストを調節する
		戻り値: なし
		＊カラーは0.0から1.0の範囲に調整され、計算される
		＊thisに結果を設定する
		***************************************************************************/
		void AdjustContrast(float Contrast) {
			AdjustRange();
			*this = (Color4)XMColorAdjustContrast(XMVECTOR(*this), Contrast);
		}
		/**************************************************************************
		void AdjustSaturation(
		float Saturation		//彩度値
		);
		用途: カラーの彩度を調節しthisに設定する
		戻り値: なし
		＊カラーは0.0から1.0の範囲に調整され、計算される
		＊彩度値が0の場合はグレースケール、1.0の場合はもとの色になる
		＊thisに結果を設定する
		***************************************************************************/
		void AdjustSaturation(float Saturation) {
			AdjustRange();
			*this = (Color4)XMColorAdjustSaturation(XMVECTOR(*this), Saturation);
		}
		/**************************************************************************
		void Modulate(
		const Color4& other	//カラー1
		);
		用途: thisにもう一つのカラーの対応する成分を乗算してブレンドしたものをthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Modulate(const Color4& other) {
			AdjustRange();
			*this = (Color4)XMColorModulate(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void Negative();
		用途: thisのカラーの負の RGB カラー値を求めthisに設定する
		戻り値: なし
		＊カラーは0.0から1.0の範囲に調整され、計算される
		＊thisに結果を設定する
		***************************************************************************/
		void Negative() {
			AdjustRange();
			*this = (Color4)XMColorNegative(XMVECTOR(*this));
		}
		/**************************************************************************
		void GrayColor();
		用途: thisのカラーをグレーに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void GrayColor() {
			*this = Color4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		/**************************************************************************
		void WhiteColor();
		用途: thisのカラーを白に設定する
		戻り値: なし
		＊カラーは白のカラー値（1.0,1.0,1.0,1.0）になる
		＊thisに結果を設定する
		***************************************************************************/
		void WhiteColor() {
			*this = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		}


	};

	/**************************************************************************
	struct Quaternion : public XMFLOAT4;
	用途: クオータニオン構造体
	****************************************************************************/
	struct Quaternion : public XMFLOAT4 {
		/**************************************************************************
		Quaternion();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Quaternion() :XMFLOAT4() {
			Identity();
		}
		Quaternion(const XMFLOAT4& v4) :XMFLOAT4(v4) {
		}
		Quaternion(const XMVECTOR& other) :XMFLOAT4() {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
		}
		Quaternion(float x, float y, float z, float w) :
			XMFLOAT4(x, y, z, w) {
		}
		Quaternion(const Quaternion& other) :XMFLOAT4() {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		//回転スカラから作成
		Quaternion(float Pitch, float Yaw, float Roll) :XMFLOAT4() {
			RotationRollPitchYaw(Pitch, Yaw, Roll);
		}
		//回転軸と角度から作成
		Quaternion(const Vector3& Vec, float Angle) :XMFLOAT4() {
			RotationAxis(Vec, Angle);
		}
		//回転行列から作成
		Quaternion(const XMMATRIX& m) :XMFLOAT4() {
			RotationMatrix(m);
		}
		inline Quaternion(const Matrix4X4& m);

		operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		void Set(float xx, float yy, float zz, float ww) {
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}
		/**************************************************************************
		多重定義
		***************************************************************************/
		//代入
		Quaternion& operator=(const Quaternion& other) {
			//自己代入の抑制
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}
		Quaternion& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4(this, temp);
			return *this;
		}
		//追加演算
		Quaternion& operator*=(const Quaternion& other) {
			*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this), XMVECTOR(other));
			return *this;
		}
		//演算
		Quaternion operator*(const Quaternion& other)const {
			Quaternion temp(*this);
			temp *= other;
			return temp;
		}
		//比較
		/**************************************************************************
		bool Equal(
		const Quaternion& other	//比較するクオータニオン
		) const;
		用途: thisがもう一つのクオータニオンと等しいかどうかを検証する
		戻り値: 等しければtrue
		***************************************************************************/
		bool Equal(const Quaternion& other)const {
			return XMQuaternionEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator==(const Quaternion& other)const {
			return Equal(other);
		}
		/**************************************************************************
		bool NotEqual(
		const Quaternion& other	//比較するクオータニオン
		) const;
		用途: thisがもう一つのクオータニオンと等しくないかどうかを検証する
		戻り値: 等しくなければtrue
		***************************************************************************/
		bool NotEqual(const Quaternion& other)const {
			return XMQuaternionNotEqual(XMVECTOR(*this), XMVECTOR(other));
		}
		bool operator!=(const Quaternion& other)const {
			return NotEqual(other);
		}
		//検証
		/**************************************************************************
		bool IsIdentity() const;
		用途: thisが単位クオータニオン (0, 0, 0, 1)かどうかを検証する。
		戻り値: クオータニオン (0, 0, 0, 1)ならばtrue
		***************************************************************************/
		bool IsIdentity() const {
			return XMQuaternionIsIdentity(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsInfinite() const;
		用途: thisの値のいずれかが正か負の無限大かどうかを検証する。
		戻り値: 値のいずれかが正か負の無限大ならばtrue
		***************************************************************************/
		bool IsInfinite() const {
			return XMQuaternionIsInfinite(XMVECTOR(*this));
		}
		/**************************************************************************
		bool IsNaN() const;
		用途: 値のいずれかがNaNかどうかを検証する。
		戻り値: 値のいずれかがNaNならばtrue
		***************************************************************************/
		bool IsNaN() const {
			return XMQuaternionIsNaN(XMVECTOR(*this));
		}
		//操作
		/**************************************************************************
		void AddRotation(
		const Vector3& other	//追加回転する回転行列
		);
		用途: 回転行列を追加回転する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void AddRotation(const Vector3& other) {
			Quaternion Span;
			Span.RotationRollPitchYawFromVector(other);
			*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this), XMVECTOR(Span));
		}
		/**************************************************************************
		Vector3 GetRotation();
		用途: クオータニオンから回転ベクトルを返す
		戻り値: 回転ベクトル
		***************************************************************************/
		inline Vector3 GetRotation()const;
		/**************************************************************************
		void BaryCentric(
		const Quaternion& Q1,	//三角形の 2 つ目のクオータニオン
		const Quaternion& Q2,	//三角形の 3 つ目のクオータニオン
		float f,	//加重係数
		float g		//加重係数
		);
		用途: 三角形の1つ目のクオータニオンthisに、
		　　　指定されたクオータニオンを使用して、重心座標のポイントを設定する
		   戻り値: なし
		   ＊thisに結果を設定する
		   ***************************************************************************/
		void BaryCentric(const Quaternion& Q1, const Quaternion& Q2, float f, float g) {
			*this = (Quaternion)XMQuaternionBaryCentric(XMVECTOR(*this), XMVECTOR(Q1), XMVECTOR(Q2), f, g);
		}
		/**************************************************************************
		void BaryCentricV(
		const Quaternion& Q1,	//三角形の 2 つ目のクオータニオン
		const Quaternion& Q2,	//三角形の 3 つ目のクオータニオン
		const Vector4& f,	//加重係数
		const Vector4& g		//加重係数
		);
		用途: 三角形の1つ目のクオータニオンthisに、
		　　　指定されたクオータニオンとベクトルを使用して、重心座標のポイントを設定する
		   戻り値: なし
		   ＊ベクトルfとgはそれぞれ要素が同じ値にする
		   ＊thisに結果を設定する
		   ***************************************************************************/
		void BaryCentricV(const Quaternion& Q1, const Quaternion& Q2,
			const Vector4& f, const Vector4& g) {
			*this = (Quaternion)XMQuaternionBaryCentricV(XMVECTOR(*this), XMVECTOR(Q1), XMVECTOR(Q2), XMVECTOR(f), XMVECTOR(g));
		}
		/**************************************************************************
		void Quaternion Conjugate();
		用途: thisの共役を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Conjugate() {
			*this = (Quaternion)XMQuaternionConjugate(XMVECTOR(*this));
		}
		/**************************************************************************
		float Quaternion Dot(
		const Quaternion& other	//もう一つのクオータニオン
		) const;
		用途: thisともう一つのクオータニオンとの内積を計算して返す
		戻り値: 内積
		＊thisは変更しない
		***************************************************************************/
		float Dot(const Quaternion& other)const {
			return ((Vector4)XMQuaternionDot(XMVECTOR(*this), XMVECTOR(other))).x;
		}
		/**************************************************************************
		void Exp();
		用途: thisの指数をthisに設定する
		戻り値: なし
		＊入力のw要素は無視される
		＊thisに結果を設定する
		***************************************************************************/
		void Exp() {
			*this = (Quaternion)XMQuaternionExp(XMVECTOR(*this));
		}
		/**************************************************************************
		void Identity();
		用途: thisに単位クオータニオンを設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Identity() {
			*this = (Quaternion)XMQuaternionIdentity();
		}
		/**************************************************************************
		void Inverse();
		用途: thisを反転する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Inverse() {
			*this = (Quaternion)XMQuaternionInverse(XMVECTOR(*this));
		}
		/**************************************************************************
		float Length() const;
		用途: thisの長さを返す
		戻り値: thisの長さ
		＊thisは変更しない
		***************************************************************************/
		float Length() const {
			return ((Vector4)XMQuaternionLength(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		float LengthSq() const;
		用途: thisの長さの2乗を返す
		戻り値: thisの長さの2乗
		＊thisは変更しない
		***************************************************************************/
		float LengthSq() const {
			return  ((Vector4)XMQuaternionLengthSq(XMVECTOR(*this))).x;
		}
		/**************************************************************************
		void Ln();
		用途: thisの自然対数をthisに設定する
		戻り値: なし
		＊thisは正規化しておく
		***************************************************************************/
		void Ln() {
			*this = (Quaternion)XMQuaternionLn(XMVECTOR(*this));
		}
		/**************************************************************************
		void Multiply(
		const Quaternion& other	//相手のクオータニオン
		);
		用途: thisを相手のクオータニオンと乗算する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Multiply(const Quaternion& other) {
			*this = (Quaternion)XMQuaternionMultiply(XMVECTOR(*this), XMVECTOR(other));
		}
		/**************************************************************************
		void Normalize();
		用途: thisを正規化する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Normalize() {
			*this = (Quaternion)XMQuaternionNormalize(XMVECTOR(*this));
		}

		/**************************************************************************
		void NormalizeEst();
		用途: thisに正規化の予測値を設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void NormalizeEst() {
			*this = (Quaternion)XMQuaternionNormalizeEst(XMVECTOR(*this));
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
		void RotationAxis(
		const Vector3& Vec,		//回転軸を表す３Ｄベクトル
		float Angle				//回転角度（時計回りラジアン）
		);
		用途: 回転軸と角度からクオータニオンを作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationAxis(const Vector3& Vec, float Angle) {
			*this = XMQuaternionRotationAxis(XMVECTOR(Vec), Angle);
		}
		/**************************************************************************
		void RotationMatrix(
		const XMMATRIX& m				//回転行列
		);
		用途: 回転行列からクオータニオンをthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationMatrix(const XMMATRIX& m) {
			*this = (Quaternion)XMQuaternionRotationMatrix(m);
		}
		/**************************************************************************
		inline void RotationMatrix(
		const Matrix4X4& m				//回転行列
		);
		用途: 回転行列からクオータニオンをthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void RotationMatrix(const Matrix4X4& m);

		/**************************************************************************
		void RotationNormal(
		const Vector3& Vec,		//回転軸を表す正規化された３Ｄベクトル
		float Angle				//回転角度（時計回りラジアン）
		);
		用途: 正規化された回転軸と角度からクオータニオンを作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationNormal(const Vector3& Vec, float Angle) {
			*this = (Quaternion)XMQuaternionRotationNormal(XMVECTOR(Vec), Angle);
		}
		/**************************************************************************
		void RotationRollPitchYaw(
		float Pitch,		//X軸回転（ラジアン）
		float Yaw,			//Y軸回転（ラジアン）
		float Roll			//Z軸回転（ラジアン）
		);
		用途: 各軸の回転からクオータニオンを作成しthisに設定する
		戻り値: なし
		＊回転はRollPitchYaw（Z、Y、X）の順で行われる
		＊thisに結果を設定する
		***************************************************************************/
		void RotationRollPitchYaw(float Pitch, float Yaw, float Roll) {
			*this = (Quaternion)XMQuaternionRotationRollPitchYaw(Pitch, Yaw, Roll);
		}
		/**************************************************************************
		void RotationRollPitchYawFromVector(
		const Vector3& Vec		//回転軸を表す３Ｄベクトル
		);
		用途: 3D回転ベクトルからクオータニオンを作成しthisに設定する
		戻り値: なし
		＊回転はRollPitchYaw（Z、Y、X）の順で行われる
		＊thisに結果を設定する
		***************************************************************************/
		void RotationRollPitchYawFromVector(const Vector3& Vec) {
			*this = (Quaternion)XMQuaternionRotationRollPitchYawFromVector(XMVECTOR(Vec));
		}
		/**************************************************************************
		void Slerp(
		const Quaternion& Qt0,		//補間元（正規化されたクオータニオン）
		const Quaternion& Qt1,		//補間先（正規化されたクオータニオン）
		float t						//補間係数（0.0fから1.0fの間を指定）
		);
		用途: 2つのクオータニオンから係数で補間されたクオータニオンを作成しthisに設定する
		戻り値: なし
		＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
		＊thisに結果を設定する
		***************************************************************************/
		void Slerp(const Quaternion& Qt0, const Quaternion& Qt1, float t) {
			*this = XMQuaternionSlerp(XMVECTOR(Qt0), XMVECTOR(Qt1), t);
		}
		/**************************************************************************
		void Slerp(
		const Quaternion& other,	//補間先（正規化されたクオータニオン）
		float t						//補間係数（0.0fから1.0fの間を指定）
		);
		用途: thisともう一つのクオータニオンから係数で補間されたクオータニオンを作成しthisに設定する
		戻り値: なし
		＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
		＊thisに結果を設定する
		***************************************************************************/
		void Slerp(const Quaternion& other, float t) {
			Normalize();
			*this = XMQuaternionSlerp(XMVECTOR(*this), XMVECTOR(other), t);
		}
		/**************************************************************************
		void SlerpV(
		const Quaternion& Qt0,		//補間元（正規化されたクオータニオン）
		const Quaternion& Qt1,		//補間先（正規化されたクオータニオン）
		const Vector4& Vec			//補間係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
		);
		用途: 2つのクオータニオンから係数で補間されたクオータニオンを作成しthisに設定する
		戻り値: なし
		＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
		＊thisに結果を設定する
		***************************************************************************/
		void SlerpV(const Quaternion& Qt0, const Quaternion& Qt1, const Vector4& Vec) {
			*this = XMQuaternionSlerpV(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Vec));
		}
		/**************************************************************************
		void SlerpV(
		const Quaternion& other,		//補間先（正規化されたクオータニオン）
		const Vector4& Vec			//補間係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
		);
		用途: thisともう一つのクオータニオンから係数で補間されたクオータニオンを作成しthisに設定する
		戻り値: なし
		＊係数が0の場合はQ0、1.0ならQ1と同じものを返す
		＊thisに結果を設定する
		***************************************************************************/
		void SlerpV(const Quaternion& other, const Vector4& Vec) {
			*this = XMQuaternionSlerpV(XMVECTOR(*this), XMVECTOR(other), XMVECTOR(Vec));
		}
		/**************************************************************************
		void Squad(
		const Quaternion& Qt0,		//正規化されたクオータニオン１
		const Quaternion& Qt1,		//正規化されたクオータニオン２
		const Quaternion& Qt2,		//正規化されたクオータニオン３
		const Quaternion& Qt3,		//正規化されたクオータニオン４
		float t						//補間制御係数（0.0fから1.0fの間を指定）
		);
		用途: 球状平方補間を使用して、4 つの単位クオータニオン間を補間しthisに設定する
		戻り値: なし
		＊Q1、Q2、Q3はQuaternion::GetSquadSetup関数で作成する
		＊thisに結果を設定する
		***************************************************************************/
		void Squad(const Quaternion& Qt0, const Quaternion& Qt1, const Quaternion& Qt2, const Quaternion& Qt3, float t) {
			*this = XMQuaternionSquad(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Qt2), XMVECTOR(Qt3), t);
		}
		/**************************************************************************
		void SquadV(
		const Quaternion& Qt0,		//正規化されたクオータニオン１
		const Quaternion& Qt1,		//正規化されたクオータニオン２
		const Quaternion& Qt2,		//正規化されたクオータニオン３
		const Quaternion& Qt3,		//正規化されたクオータニオン４
		const Vector4& Vec			//補間制御係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
		);
		用途: 球状平方補間を使用して、4 つの単位クオータニオン間を補間しthisに設定する
		戻り値: なし
		＊Q1、Q2、Q3はQuaternion::GetSquadSetup関数で作成する
		＊thisに結果を設定する
		***************************************************************************/
		void SquadV(const Quaternion& Qt0, const Quaternion& Qt1, const Quaternion& Qt2, const Quaternion& Qt3, const Vector4& Vec) {
			*this = XMQuaternionSquadV(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Qt2), XMVECTOR(Qt3), XMVECTOR(Vec));
		}
		/**************************************************************************
		void ToAxisAngle(
		Vector3& rVec,			//回転軸を得る３Ｄベクトル
		float& rAngle			//回転角度を得る変数
		)const;
		用途: thisクオータニオンから、回転軸と回転角度を得る
		戻り値: なし
		＊pVecには回転軸ベクトル、pAngleには角度が設定される
		***************************************************************************/
		void ToAxisAngle(Vector3& rVec, float& rAngle)const {
			XMVECTOR Vec(rVec);
			XMQuaternionToAxisAngle(&Vec, &rAngle, XMVECTOR(*this));
			rVec = Vec;
		}

		/**************************************************************************
		void Billboard(
		const Vector3& Line			//計算元となる線
		)t;
		用途: Lineに対してビルボード角度をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void Billboard(const Vector3& Line);

		/**************************************************************************
		void Facing(
		const Vector3& Line			//計算元となる線(オブジェクトからカメラ位置を引いたもの)
		)t;
		用途: Lineに対してフェイシング角度をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void Facing(const Vector3& Line);

		/**************************************************************************
		void FacingY(
		const Vector3& Line			//計算元となる線(オブジェクトからカメラ位置を引いたもの)
		)t;
		用途: Lineに対してY軸フェイシング角度をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		inline void FacingY(const Vector3& Line);


	};

	/**************************************************************************
	struct Matrix4X4 : public XMFLOAT4X4;
	用途:　行列を表す構造体
	****************************************************************************/
	struct Matrix4X4 : public XMFLOAT4X4 {
		/**************************************************************************
		Matrix4X4();
		用途: コンストラクタ
		戻り値: なし
		＊複数の初期化方法の定義
		***************************************************************************/
		Matrix4X4() :XMFLOAT4X4() {
			Identity();
		}
		Matrix4X4(const XMMATRIX& other) :
			XMFLOAT4X4() {
			XMMATRIX temp = other;
			XMStoreFloat4x4(this, temp);
		}
		Matrix4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) :
			XMFLOAT4X4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33) {}
		Matrix4X4(const XMFLOAT4X4& m) :
			XMFLOAT4X4(m._11, m._12, m._13, m._14,
				m._21, m._22, m._23, m._24,
				m._31, m._32, m._33, m._34,
				m._41, m._42, m._43, m._44) {}
		Matrix4X4(const Matrix4X4& other) :
			XMFLOAT4X4() {
			for (UINT i = 0; i <4; i++) {
				m[i][0] = other.m[i][0];
				m[i][1] = other.m[i][1];
				m[i][2] = other.m[i][2];
				m[i][3] = other.m[i][3];
			}
		}
		Matrix4X4(const Vector3& Scaling,
			const Quaternion& RotationQuaternion,
			const Vector3& Translation) :
			XMFLOAT4X4() {
			DefTransformation(Scaling, RotationQuaternion, Translation);
		}
		Matrix4X4(const Vector3& Scaling,
			const Vector3& Rotation,
			const Vector3& Translation) :
			XMFLOAT4X4() {
			DefTransformation(Scaling, Rotation, Translation);
		}
		operator XMMATRIX() const {
			Matrix4X4 temp = *this;
			XMMATRIX m = XMLoadFloat4x4(&temp);
			return m;
		}
		/**************************************************************************
		多重定義
		***************************************************************************/
		//代入
		Matrix4X4& operator=(const Matrix4X4& other) {
			//自己代入の抑制
			if (this != &other) {
				for (UINT i = 0; i <4; i++) {
					m[i][0] = other.m[i][0];
					m[i][1] = other.m[i][1];
					m[i][2] = other.m[i][2];
					m[i][3] = other.m[i][3];
				}
			}
			return *this;
		}
		Matrix4X4& operator=(const XMMATRIX& other) {
			XMMATRIX temp = other;
			XMStoreFloat4x4(this, temp);
			return *this;
		}
		//追加演算
		Matrix4X4& operator+=(const Matrix4X4& other) {
			XMMATRIX temp = XMMATRIX(*this);
			temp += XMMATRIX(other);
			*this = Matrix4X4(temp);
			return *this;
		}
		Matrix4X4& operator-=(const Matrix4X4& other) {
			XMMATRIX temp = XMMATRIX(*this);
			temp -= XMMATRIX(other);
			*this = Matrix4X4(temp);
			return *this;
		}
		Matrix4X4& operator*=(const Matrix4X4& other) {
			XMMATRIX temp = XMMATRIX(*this);
			temp *= XMMATRIX(other);
			*this = Matrix4X4(temp);
			return *this;
		}
		Matrix4X4& operator*=(float f) {
			XMMATRIX temp = XMMATRIX(*this);
			temp *= f;
			*this = Matrix4X4(temp);
			return *this;
		}
		Matrix4X4& operator/=(float f) {
			if (f == 0.0f) {
				//0除算
				throw BaseException(
					L"0除算です",
					L"if(f == 0.0f)",
					L"Matrix4X4::operator/=()"
					);
			}
			XMMATRIX temp = XMMATRIX(*this);
			temp /= f;
			*this = Matrix4X4(temp);
			return *this;
		}
		Matrix4X4 operator+ (const Matrix4X4& other) const {
			Matrix4X4 temp(*this);
			temp += other;
			return temp;
		}
		Matrix4X4 operator- (const Matrix4X4& other) const {
			Matrix4X4 temp(*this);
			temp -= other;
			return temp;
		}
		Matrix4X4 operator* (const Matrix4X4& other) const {
			Matrix4X4 temp(*this);
			temp *= other;
			return temp;
		}
		Matrix4X4 operator* (float f) const {
			Matrix4X4 temp(*this);
			temp *= f;
			return temp;
		}
		Matrix4X4 operator/ (float f) const {
			Matrix4X4 temp(*this);
			temp /= f;
			return temp;
		}
		//個別アクセス
		float Get(UINT Row, UINT Col) {
			if (Row > 3 || Col > 3) {
				throw BaseException(
					L"行列の範囲外です",
					L"if(Row > 3 || Col > 3)",
					L"Matrix4X4::Get()"
					);
			}
			return m[Row][Col];
		}
		void Set(UINT Row, UINT Col, float f) {
			if (Row > 3 || Col > 3) {
				throw BaseException(
					L"行列の範囲外です",
					L"if(Row > 3 || Col > 3)",
					L"Matrix4X4::Set()"
					);
			}
			m[Row][Col] = f;
		}
		/**************************************************************************
		Vector3 PosInMatrixSt()const;
		用途: this行列内の位置情報を取得する(直接行列から取り出す
		戻り値: 位置ベクトル
		＊thisは変更しない
		***************************************************************************/
		Vector3 PosInMatrixSt()const {
			Vector3 Vec;
			Vec.x = _41;
			Vec.y = _42;
			Vec.z = _43;
			return Vec;
		}
		/**************************************************************************
		Vector3 PosInMatrix()const;
		用途: this行列内の位置情報を取得する
		戻り値: 位置ベクトル
		＊thisは変更しない
		***************************************************************************/
		Vector3 PosInMatrix()const {
			Vector3 Vec(0, 0, 0);
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(*this))) {
				Vec = Translation;
			}
			return Vec;
		}
		/**************************************************************************
		Vector3 ScaleInMatrix()const;
		用途: this行列内のスケーリングを取得する
		戻り値: スケーリングベクトル
		＊thisは変更しない
		***************************************************************************/
		Vector3 ScaleInMatrix()const {
			Vector3 Vec(1.0f, 1.0f, 1.0f);
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(*this))) {
				Vec = Scale;
			}
			return Vec;
		}
		/**************************************************************************
		Quaternion QtInMatrix()const;
		用途: this行列内の回転を取得する
		戻り値: 回転クオータニオン
		＊thisは変更しない
		***************************************************************************/
		Quaternion QtInMatrix() const {
			Quaternion RetQt;
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(*this))) {
				RetQt = Qt;
			}
			return RetQt;
		}

		/**************************************************************************
		static void GetSafeScale(
		Vector3&	Scale	//変化させるスケール
		);
		用途: 0スケールをなくす
		戻り値: なし、0スケーリングを0.0001に変更する
		***************************************************************************/
		static void GetSafeScale(Vector3& Scale) {
			if (Scale.x == 0.0f) {
				Scale.x = 0.0001f;
			}
			if (Scale.y == 0.0f) {
				Scale.y = 0.0001f;
			}
			if (Scale.z == 0.0f) {
				Scale.z = 0.0001f;
			}
		}
		/**************************************************************************
		Vector3 RotXInMatrix()const;
		用途: this行列内のX軸傾きを取得する
		戻り値: X軸の傾き
		＊thisは変更しない
		***************************************************************************/
		Vector3 RotXInMatrix()const {
			Vector3 ret(_11, _12, _13);
			Vector3 Scale = ScaleInMatrix();
			GetSafeScale(Scale);
			ret.x /= Scale.x;
			ret.y /= Scale.x;
			ret.z /= Scale.x;
			return ret;
		}
		/**************************************************************************
		Vector3 RotYInMatrix()const;
		用途: this行列内のY軸傾きを取得する
		戻り値: Y軸の傾き
		＊thisは変更しない
		***************************************************************************/
		Vector3 RotYInMatrix()const {
			Vector3 ret(_21, _22, _23);
			Vector3 Scale = ScaleInMatrix();
			GetSafeScale(Scale);
			ret.x /= Scale.y;
			ret.y /= Scale.y;
			ret.z /= Scale.y;
			return ret;
		}
		/**************************************************************************
		Vector3 RotZInMatrix()const;
		用途: this行列内のZ軸傾きを取得する
		戻り値: Z軸の傾き
		＊thisは変更しない
		***************************************************************************/
		Vector3 RotZInMatrix()const {
			Vector3 ret(_31, _32, _33);
			Vector3 Scale = ScaleInMatrix();
			GetSafeScale(Scale);
			ret.x /= Scale.z;
			ret.y /= Scale.z;
			ret.z /= Scale.z;
			return ret;
		}

		/**************************************************************************
		void AffineTransformation(
		const Vector3& Scaling,					//スケーリング
		const Vector3& RotOrigin,				//回転の中心
		const Quaternion& RotationQuaternion,	//クオータニオン
		const Vector3& Translation				//平行移動
		);
		用途: スケーリング、回転の中心座標、クオータニオン、平行移動から行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void AffineTransformation(const Vector3& Scaling, const Vector3& RotOrigin,
			const Quaternion& RotationQuaternion, const Vector3& Translation) {
			*this = (Matrix4X4)XMMatrixAffineTransformation(XMVECTOR(Scaling), XMVECTOR(RotOrigin),
				XMVECTOR(RotationQuaternion), XMVECTOR(Translation));
		}
		/**************************************************************************
		void AffineTransformation2D(
		const Vector2& Scaling,					//スケーリング
		const Vector2& RotOrigin,				//回転の中心
		float Rotation,							//回転
		const Vector2& Translation				//平行移動
		);
		用途: スケーリング、回転の中心座標、回転値（ラジアン）、平行移動から2D行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void AffineTransformation2D(const Vector2& Scaling,
			const Vector2& RotOrigin, float Rotation, const Vector2& Translation) {
			*this = (Matrix4X4)XMMatrixAffineTransformation2D(XMVECTOR(Scaling), XMVECTOR(RotOrigin),
				Rotation, XMVECTOR(Translation));
		}
		/**************************************************************************
		bool Decompose(
		Vector3& rScaling,					//スケーリング
		Quaternion& rQt,					//クオータニオン
		Vector3& rTranslation,				//平行移動
		)const;
		用途: thisからスケーリング、回転、平行移動を取り出す
		戻り値: 成功すればtrue
		＊thisは変更しない
		***************************************************************************/
		void Decompose(Vector3& rScaling, Quaternion& rQt,
			Vector3& rTranslation)const {
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(*this))) {
				rScaling = Scale;
				rQt = Qt;
				rTranslation = Translation;
			}
			else {
				throw BaseException(
					L"行列のデコンポーズに失敗しました",
					L"if(!XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(*this)))",
					L"Matrix4X4::Decompose()"
					);
			}
		}
		/**************************************************************************
		void ScaleIdentity();
		用途: スケーリングを1.0にした行列に変換する
		戻り値: なし
		＊thisに設定する
		***************************************************************************/
		void ScaleIdentity() {
			Vector3 Scale, Translation;
			Quaternion Qt;
			Decompose(Scale, Qt, Translation);
			Scale = Vector3(1.0f, 1.0f, 1.0f);
			DefTransformation(Scale, Qt, Translation);
		}

		/**************************************************************************
		Vector4 Determinant()const;
		用途: thisの行列式を返す
		戻り値: 作成した行列式（Vector4の各要素に入る）
		＊thisは変更しない
		***************************************************************************/
		Vector4 Determinant()const {
			return (Vector4)XMMatrixDeterminant(XMMATRIX(*this));
		}
		/**************************************************************************
		void Identity();
		用途: 単位行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Identity() {
			*this = (Matrix4X4)XMMatrixIdentity();
		}
		/**************************************************************************
		void Inverse(
		Vector4* pVec					//行列式を受け取るベクトル
		);
		用途:逆行列をthisに設定する
		戻り値: なし
		＊pVecには行列式が入る
		＊thisに結果を設定する
		***************************************************************************/
		void Inverse(Vector4* pVec) {
			XMVECTOR Vec(*pVec);
			*this = (Matrix4X4)XMMatrixInverse(&Vec, XMMATRIX(*this));
			*pVec = Vec;
		}
		/**************************************************************************
		bool IsIdentity()const;
		用途: thisが単位行列かどうかを検証する
		戻り値: 単位行列ならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsIdentity()const {
			return XMMatrixIsIdentity(XMMATRIX(*this));
		}
		/**************************************************************************
		bool IsInfinite()const;
		用途: thisのいずれかの要素が正または負の無限大かどうかを検証する
		戻り値: 正または負の無限大ならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsInfinite()const {
			return XMMatrixIsInfinite(XMMATRIX(*this));
		}
		/**************************************************************************
		bool IsNaN()const;
		用途: thisのいずれかの要素がNaNかどうかを検証する
		戻り値: NaNならtrue
		＊thisは変更しない
		***************************************************************************/
		bool IsNaN()const {
			return XMMatrixIsNaN(XMMATRIX(*this));
		}
		/**************************************************************************
		void LookAtLH(
		const Vector3& Eye,					//カメラ位置
		const Vector3& At,					//カメラ視点
		const Vector3& Up					//カメラ傾き（通常<0,1,0>）
		);
		用途:左手座標ビュー行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void LookAtLH(const Vector3& Eye, const Vector3& At, const Vector3& Up) {
			*this = (Matrix4X4)XMMatrixLookAtLH(
				XMVECTOR(Eye),
				XMVECTOR(At),
				XMVECTOR(Up));
		}
		/**************************************************************************
		void LookAtRH(
		const Vector3& Eye,					//カメラ位置
		const Vector3& At,					//カメラ視点
		const Vector3& Up					//カメラ傾き（通常<0,1,0>）
		);
		用途:右手座標ビュー行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void LookAtRH(const Vector3& Eye, const Vector3& At, const Vector3& Up) {
			*this = (Matrix4X4)XMMatrixLookAtRH(
				XMVECTOR(Eye),
				XMVECTOR(At),
				XMVECTOR(Up));
		}
		/**************************************************************************
		void LookToLH(
		const Vector3& Eye,					//カメラ位置
		const Vector3& EyeDirection,		//カメラ向き
		const Vector3& Up					//カメラ傾き（通常<0,1,0>）
		);
		用途:カメラ位置、向き、傾きから左手座標ビュー行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void LookToLH(const Vector3& Eye, const Vector3& EyeDirection, const Vector3& Up) {
			*this = (Matrix4X4)XMMatrixLookToLH(
				XMVECTOR(Eye),
				XMVECTOR(EyeDirection),
				XMVECTOR(Up));
		}
		/**************************************************************************
		void LookToRH(
		const Vector3& Eye,					//カメラ位置
		const Vector3& EyeDirection,		//カメラ向き
		const Vector3& Up					//カメラ傾き（通常<0,1,0>）
		);
		用途:カメラ位置、向き、傾きから右手座標ビュー行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void LookToRH(const Vector3& Eye, const Vector3& EyeDirection, const Vector3& Up) {
			*this = (Matrix4X4)XMMatrixLookToRH(
				XMVECTOR(Eye),
				XMVECTOR(EyeDirection),
				XMVECTOR(Up));
		}
		/**************************************************************************
		void Multiply(
		const Matrix4X4& m					//演算する行列
		);
		用途:thisと行列の積をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Multiply(const Matrix4X4& m) {
			*this = (Matrix4X4)XMMatrixMultiply(XMMATRIX(*this), XMMATRIX(m));
		}
		/**************************************************************************
		void MultiplyTranspose(
		const Matrix4X4& m					//演算する行列
		);
		用途:thisと行列の積の転置行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void MultiplyTranspose(const Matrix4X4& m) {
			*this = (Matrix4X4)XMMatrixMultiplyTranspose(XMMATRIX(*this), XMMATRIX(m));
		}
		/**************************************************************************
		void OrthographicLH(
		float ViewWidth,		//幅
		float ViewHeight,		//高さ
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途:左手座標系の正射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void OrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		/**************************************************************************
		void OrthographicRH(
		float ViewWidth,		//幅
		float ViewHeight,		//高さ
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途:右手座標系の正射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void OrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixOrthographicRH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		/**************************************************************************
		void OrthographicOffCenterLH(
		float ViewLeft,			//左
		float ViewRight,		//右
		float ViewBottom,		//下
		float ViewTop,			//上
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途:左手座標系のカスタム正射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixOrthographicOffCenterLH(
				ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		/**************************************************************************
		void OrthographicOffCenterRH(
		float ViewLeft,			//左
		float ViewRight,		//右
		float ViewBottom,		//下
		float ViewTop,			//上
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途:右手座標系のカスタム正射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void OrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixOrthographicOffCenterRH(
				ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveFovLH(
		float FovAngleY,		//トップダウン視野角度
		float AspectHByW,		//ビュー空間の高さと幅のアスペクト比
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: 視野に基づいて、左手座標系のパースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveFovLH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveFovLH(FovAngleY, AspectHByW, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveFovRH(
		float FovAngleY,		//トップダウン視野角度
		float AspectHByW,		//ビュー空間の高さと幅のアスペクト比
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: 視野に基づいて、右手座標系のパースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveFovRH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveFovRH(FovAngleY, AspectHByW, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveLH(
		float ViewWidth,		//近くのクリップ面の視錐台の幅
		float ViewHeight,		//近くのクリップ面の視錐台の高さ
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: 左手座標系のパースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveLH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveRH(
		float ViewWidth,		//近くのクリップ面の視錐台の幅
		float ViewHeight,		//近くのクリップ面の視錐台の高さ
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: 右手座標系のパースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveRH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveOffCenterLH(
		float ViewLeft,			//クリップ視錐台の左側の x 座標
		float ViewRight,		//クリップ視錐台の右側の x 座標
		float ViewBottom,		//クリップ視錐台の下側の y 座標
		float ViewTop,			//クリップ視錐台の上側の y 座標
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: カスタム バージョンの左手座標系パースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop,
			float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		/**************************************************************************
		void PerspectiveOffCenterRH(
		float ViewLeft,			//クリップ視錐台の左側の x 座標
		float ViewRight,		//クリップ視錐台の右側の x 座標
		float ViewBottom,		//クリップ視錐台の下側の y 座標
		float ViewTop,			//クリップ視錐台の上側の y 座標
		float NearZ,			//奥行手前
		float FarZ				//奥行奥
		);
		用途: カスタム バージョンの右手座標系パースペクティブ射影行列をthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void PerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop,
			float NearZ, float FarZ) {
			*this = (Matrix4X4)XMMatrixPerspectiveOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		/**************************************************************************
		void Reflect(
		const Plane& Pln		//平面
		);
		用途: 指定された平面を介してベクトルを反映するトランスフォーム行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Reflect(const Plane& Pln) {
			*this = (Matrix4X4)XMMatrixReflect(XMVECTOR(Pln));
		}
		/**************************************************************************
		void RotationAxis(
		const Vector3& Axis,		//回転軸を表すベクトル
		float Angle				//回転角度（ラジアン）
		);
		用途: 任意の軸を回転軸にして、回転した行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationAxis(const Vector3& Axis, float Angle) {
			*this = (Matrix4X4)XMMatrixRotationAxis(XMVECTOR(Axis), Angle);
		}
		/**************************************************************************
		void RotationNormal(
		const Vector3& NormalAxis,		//回転軸を記述する法線ベクトル
		float Angle						//回転角度（ラジアン）
		);
		用途: 任意の法線ベクトルを回転軸にして、回転した行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationNormal(const Vector3& NormalAxis, float Angle) {
			*this = (Matrix4X4)XMMatrixRotationNormal(XMVECTOR(NormalAxis), Angle);
		}
		/**************************************************************************
		void RotationQuaternion(
		const Quaternion& Qt		//回転を記述するクオータニオン
		);
		用途: クオータニオンから回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationQuaternion(const Quaternion& Qt) {
			*this = (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(Qt));
		}
		/**************************************************************************
		void RotationRollPitchYaw(
		float Pitch,		//X軸回転
		float Yaw,			//Y軸回転
		float Roll			//Z軸回転
		);
		用途: オイラー角に基づいて回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationRollPitchYaw(float Pitch, float Yaw, float Roll) {
			*this = (Matrix4X4)XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);
		}
		/**************************************************************************
		void RotationRollPitchYawFromVector(
		const Vector3& Angles		//回転を記述するベクトル
		);
		用途: オイラー角(ベクトル)に基づいて回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationRollPitchYawFromVector(const Vector3& Angles) {
			*this = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Angles));
		}
		/**************************************************************************
		void RotationX(
		float Angle		//X軸回転
		);
		用途: X軸回転に基づいて回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationX(float Angle) {
			*this = (Matrix4X4)XMMatrixRotationX(Angle);
		}
		/**************************************************************************
		void RotationY(
		float Angle		//X軸回転
		);
		用途: Y軸回転に基づいて回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationY(float Angle) {
			*this = (Matrix4X4)XMMatrixRotationY(Angle);
		}
		/**************************************************************************
		void RotationZ(
		float Angle		//X軸回転
		);
		用途: Z軸回転に基づいて回転行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void RotationZ(float Angle) {
			*this = (Matrix4X4)XMMatrixRotationZ(Angle);
		}
		/**************************************************************************
		void Scaling(
		float ScaleX,	//Xに沿ったスケーリング
		float ScaleY,	//Yに沿ったスケーリング
		float ScaleZ	//Zに沿ったスケーリング
		);
		用途: x 軸、y 軸、および z 軸に沿ってスケーリングした行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Scaling(float ScaleX, float ScaleY, float ScaleZ) {
			*this = (Matrix4X4)XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
		}
		/**************************************************************************
		void ScalingFromVector(
		const Vector3& Scale		//スケーリングを記述するベクトル
		);
		用途: ベクトルに沿ってスケーリングした行列を作成しthisに設定する
		戻り値: なし
		***************************************************************************/
		void ScalingFromVector(const Vector3& Scale) {
			*this = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scale));
		}
		/**************************************************************************
		void Set(
		float m00,	//(0,0)の値
		float m01,	//(0,1)の値
		float m02,	//(0,2)の値
		float m03,	//(0,3)の値
		float m10,	//(1,0)の値
		float m11,	//(1,1)の値
		float m12,	//(1,2)の値
		float m13,	//(1,3)の値
		float m20,	//(2,0)の値
		float m21,	//(2,1)の値
		float m22,	//(2,2)の値
		float m23,	//(2,3)の値
		float m30,	//(3,0)の値
		float m31,	//(3,1)の値
		float m32,	//(3,2)の値
		float m33	//(3,3)の値
		);
		用途: 各値を設定した行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Set(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			) {
			*this = (Matrix4X4)XMMatrixSet(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33
				);
		}
		/**************************************************************************
		void Shadow(
		const Plane& ShadowPlane,			//基準面
		const Vector4& LightPosition		//ライトの位置
		);
		用途: ジオメトリを平面に射影するトランスフォーム行列を作成しthisに設定する
		戻り値: なし
		＊ライトのw要素は、0.0でディレクショナルライト、1.0でポイントライトとなる
		＊thisに結果を設定する
		***************************************************************************/
		void Shadow(const Plane& ShadowPlane, const Vector4& LightPosition) {
			*this = (Matrix4X4)XMMatrixShadow(XMVECTOR(ShadowPlane), XMVECTOR(LightPosition));
		}
		/**************************************************************************
		void Transformation(
		const Vector3& ScalingOrigin,		//スケーリングの中心を記述する 3D ベクトル
		const Quaternion& ScalingOrientationQuaternion,		//スケーリングの向きを記述するクオータニオン
		const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
		const Vector3& RotationOrigin,		//回転の中心を記述する 3D ベクトル
		const Quaternion& RotationQuaternion,		//RotationOrigin によって示される原点を中心とする回転を記述するクオータニオン
		const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		);
		用途: トランスフォーム行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transformation(const Vector3& ScalingOrigin, const Quaternion& ScalingOrientationQuaternion, const Vector3& Scaling,
			const Vector3& RotationOrigin, const Quaternion& RotationQuaternion,
			const Vector3& Translation) {
			*this = (Matrix4X4)XMMatrixTransformation(
				XMVECTOR(ScalingOrigin), XMVECTOR(ScalingOrientationQuaternion), XMVECTOR(Scaling),
				XMVECTOR(RotationOrigin), XMVECTOR(RotationQuaternion),
				XMVECTOR(Translation)
				);
		}
		/**************************************************************************
		void Transformation2D(
		const Vector2& ScalingOrigin,		//スケーリングの中心を記述する 2D ベクトル
		float ScalingOrientation,			//スケーリング回転係数
		const Vector2& Scaling,				//x 軸および y 軸のスケーリング係数を含む 2D ベクトル
		const Vector2& RotationOrigin,		//回転の中心を記述する 2D ベクトル
		float Rotation,						//回転の角度 (ラジアン単位)
		const Vector2& Translation			//平行移動を記述する 2D ベクトル
		);
		用途: 2Dトランスフォーム行列をxy平面に作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transformation2D(const Vector2& ScalingOrigin, float ScalingOrientation, const Vector2& Scaling,
			const Vector2& RotationOrigin, float Rotation,
			const Vector2& Translation) {
			*this = (Matrix4X4)XMMatrixTransformation2D(XMVECTOR(ScalingOrigin), ScalingOrientation, XMVECTOR(Scaling),
				XMVECTOR(RotationOrigin), Rotation, XMVECTOR(Translation));
		}
		/**************************************************************************
		void Translation(
		float OffsetX,	//X軸に沿った平行移動
		float OffsetY,	//Y軸に沿った平行移動
		float OffsetZ	//Z軸に沿った平行移動
		);
		用途: 指定されたオフセットから平行移動行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Translation(float OffsetX, float OffsetY, float OffsetZ) {
			*this = (Matrix4X4)XMMatrixTranslation(OffsetX, OffsetY, OffsetZ);
		}
		/**************************************************************************
		void TranslationFromVector(
		const Vector3& Offset				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		);
		用途: ベクトルから平行移動行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void TranslationFromVector(const Vector3& Offset) {
			*this = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Offset));
		}
		/**************************************************************************
		void Transpose(
		const Matrix4X4& m				//転置する行列
		);
		用途: 行列を転置してthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transpose(const Matrix4X4& m) {
			*this = (Matrix4X4)XMMatrixTranspose(XMMATRIX(m));
		}
		/**************************************************************************
		void Transpose();
		用途: thisを転置してthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void Transpose() {
			*this = (Matrix4X4)XMMatrixTranspose(XMMATRIX(*this));
		}
		/**************************************************************************
		void DefTransformation(
		const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
		const Quaternion& RotationQuaternion,		//回転を記述するクオータニオン
		const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		);
		用途: スケーリング、回転、移動から行列を作成しthisに設定する
		戻り値: なし
		＊thisに結果を設定する
		***************************************************************************/
		void DefTransformation(const Vector3& Scaling, const Quaternion& RotationQuaternion, const Vector3& Translation) {
			AffineTransformation(Scaling, Vector3(0, 0, 0), RotationQuaternion, Translation);
		}
		/**************************************************************************
		void DefTransformation(
		const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
		const Vector3& Rotation,			//回転を記述するベクトル
		const Vector3& Translation				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		);
		用途: スケーリング、回転、移動から行列を作成する
		戻り値: 作成した行列
		＊thisに結果を設定する
		***************************************************************************/
		void DefTransformation(const Vector3& Scaling, const Vector3& Rotation, const Vector3& Translation) {
			Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
			Matrix4X4 Rot = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Rotation));
			Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
			*this = Scale * Rot * Trans;
		}
		/**************************************************************************
		void STRTransformation(
		const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
		const Vector3& Translation,				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		const Quaternion& RotationQuaternion		//回転を記述するクオータニオン
		);
		用途: スケーリング、移動、回転から行列を作成しthisに設定する
		戻り値: なし
		＊回転より移動を先に行う
		＊thisに結果を設定する
		***************************************************************************/
		void STRTransformation(const Vector3& Scaling, const Vector3& Translation, const Quaternion& RotationQuaternion) {
			Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
			Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
			Matrix4X4 Qt = (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(RotationQuaternion));
			*this = Scale * Trans * Qt;
		}
		/**************************************************************************
		void STRTransformation(
		const Vector3& Scaling,				//x 軸、y 軸、z 軸のスケーリング係数を含む 3D ベクトル
		const Vector3& Translation,				//x 軸、y 軸、z 軸に沿った平行移動を記述する 3D ベクトル
		const Vector3& Rotation			//回転を記述するベクトル
		);
		用途: スケーリング、移動、回転から行列を作成しthisに設定する
		戻り値: なし
		＊回転より移動を先に行う
		＊thisに結果を設定する
		***************************************************************************/
		void STRTransformation(const Vector3& Scaling, const Vector3& Translation, const Vector3& Rotation) {
			Matrix4X4 Scale = (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scaling));
			Matrix4X4 Trans = (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Translation));
			Matrix4X4 Rot = (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Rotation));
			*this = Scale * Trans * Rot;
		}
	};

	//回転行列から作成
	Quaternion::Quaternion(const Matrix4X4& m) {
		*this = (Quaternion)XMQuaternionRotationMatrix(XMMATRIX(m));
	}

	void Quaternion::RotationMatrix(const Matrix4X4& m) {
		*this = (Quaternion)XMQuaternionRotationMatrix(XMMATRIX(m));
	}




	/**************************************************************************
	Vector3 GetRotation();
	用途: クオータニオンから回転ベクトルを返す
	戻り値: 回転ベクトル
	***************************************************************************/
	Vector3 Quaternion::GetRotation()const {
		Quaternion Temp = *this;
		Temp.Normalize();
		Matrix4X4 mt;
		mt.RotationQuaternion(Temp);
		Vector3 Rot;
		if (mt._32 == 1.0f) {
			Rot.x = XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21, mt._11);
		}
		else if (mt._32 == -1.0f) {
			Rot.x = -XM_PI / 2.0f;
			Rot.y = 0;
			Rot.z = -atan2(mt._21, mt._11);
		}
		else {
			Rot.x = -asin(mt._32);
			Rot.y = -atan2(-mt._31, mt._33);
			Rot.z = atan2(mt._12, mt._11);
		}
		return Rot;
	}



	void Quaternion::Billboard(const Vector3& Line) {
		Vector3 Temp = Line;
		Matrix4X4 RotMatrix;
		Vector4 dammi(0, 0, 0, 0);
		Vector3 DefUp(0, 1.0f, 0);
		Vector2 TempVec2(Temp.x, Temp.z);
		if (TempVec2.Length() < 0.1f) {
			DefUp = Vector3(0, 0, 1.0f);
		}
		Temp.Normalize();
		RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
		RotMatrix.Inverse(&dammi);
		*this = RotMatrix.QtInMatrix();
		this->Normalize();
	}


	void Quaternion::Facing(const Vector3& Line) {
		Vector3 Temp = Line;
		Matrix4X4 RotMatrix;
		Vector4 dammi(0, 0, 0, 0);
		Vector3 DefUp(0, 1.0f, 0);
		Vector2 TempVec2(Temp.x, Temp.z);
		if (TempVec2.Length() < 0.1f) {
			DefUp = Vector3(0, 0, 1.0f);
		}
		RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
		RotMatrix.Inverse(&dammi);
		*this = RotMatrix.QtInMatrix();
		this->Normalize();
	}

	void Quaternion::FacingY(const Vector3& Line) {
		Vector3 Temp = Line;
		Temp.Normalize();
		this->Identity();
		this->RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
		this->Normalize();
	}



}
//end of namespace basedx11.

