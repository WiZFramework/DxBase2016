
/******************************************************************************
 VertexHelper.h

 頂点定義、プリミティブ作成関数等

 Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
 DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "StdAfx.h"

namespace basedx11{


    #if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
    #define XM_CALLCONV __fastcall
    typedef const XMVECTOR& HXMVECTOR;
    typedef const XMMATRIX& FXMMATRIX;
    #endif

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[];
 用途: 位置を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPosition;
 用途: 位置を持つ頂点の定義
****************************************************************************/
struct VertexPosition
{
	VertexPosition()
	{ }

	VertexPosition(float x,float y,float z)
		: position(XMFLOAT3(x,y,z))
	{ }


	VertexPosition(XMFLOAT3 const& position)
		: position(position)
	{ }

	VertexPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&this->position, position);
	}

	XMFLOAT3 position;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[];
 用途: 位置と色を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionColor;
 用途: 位置と色を持つ頂点の定義
****************************************************************************/
struct VertexPositionColor
{
	VertexPositionColor()
	{ }

	VertexPositionColor(XMFLOAT3 const& position, XMFLOAT4 const& color)
		: position(position),
		color(color)
	{ }

	VertexPositionColor(FXMVECTOR position, FXMVECTOR color)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat4(&this->color, color);
	}

	XMFLOAT3 position;
	XMFLOAT4 color;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionColorLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionColorLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[];
 用途: 位置とテクスチャを持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionTexture;
 用途: 位置とテクスチャを持つ頂点の定義
****************************************************************************/
struct VertexPositionTexture
{
	VertexPositionTexture()
	{ }

	VertexPositionTexture(XMFLOAT3 const& position, XMFLOAT2 const& textureCoordinate)
		: position(position),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionTexture(FXMVECTOR position, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalLayout[];
 用途: 位置と法線を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionNormal;
 用途: 位置と法線を持つ頂点の定義
****************************************************************************/
struct VertexPositionNormal
{
	VertexPositionNormal()
	{ }

	VertexPositionNormal(XMFLOAT3 const& position, XMFLOAT3 const& normal)
		: position(position),
		normal(normal)
	{ }

	VertexPositionNormal(FXMVECTOR position, FXMVECTOR normal)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalLayout );
	}
};




/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[];
 用途: 位置と色とテクスチャを持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionColorTexture;
 用途: 位置と色とテクスチャを持つ頂点の定義
****************************************************************************/
struct VertexPositionColorTexture
{
	VertexPositionColorTexture()
	{ }

	VertexPositionColorTexture(XMFLOAT3 const& position, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		color(color),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionColorTexture(FXMVECTOR position, FXMVECTOR color, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat4(&this->color, color);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionColorTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[];
 用途: 位置と法線と色を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalColor;
 用途: 位置と法線と色を持つ頂点の定義
****************************************************************************/
struct VertexPositionNormalColor
{
	VertexPositionNormalColor()
	{ }

	VertexPositionNormalColor(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color)
		: position(position),
		normal(normal),
		color(color)
	{ }

	VertexPositionNormalColor(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->color, color);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalColorLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalColorLayout );
	}
};



/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[];
 用途: 位置と法線とテクスチャを持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTexture;
 用途: 位置と法線とテクスチャを持つ頂点の定義
****************************************************************************/
struct VertexPositionNormalTexture
{
	VertexPositionNormalTexture()
	{ }

	VertexPositionNormalTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionNormalTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTextureLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[];
 用途: 位置と法線とテクスチャとスキニング情報を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
struct  VertexPositionNormalTextureSkinning{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 textureCoordinate;
	uint32_t indices[4];
	float weights[4];
	VertexPositionNormalTextureSkinning(){}
	
    VertexPositionNormalTextureSkinning(XMFLOAT3 const& pos, XMFLOAT3 const& nor, XMFLOAT2 const& texture,
		uint32_t* index,float* weight)
        : position(pos),
        normal(nor),
        textureCoordinate(texture)
    { 
		for(int i = 0;i < 4;i++){
			indices[i] = index[i];
			weights[i] = weight[i];
		}
	}
	
	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTextureSkinningLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTextureSkinningLayout );
	}
};



/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[];
 用途: 位置と法線と色とテクスチャを持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalColorTexture;
 用途: 位置と法線と色とテクスチャを持つ頂点の定義
****************************************************************************/
struct VertexPositionNormalColorTexture
{
	VertexPositionNormalColorTexture()
	{ }

	VertexPositionNormalColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		color(color),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionNormalColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color, CXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->color, color);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalColorTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[];
 用途: 位置と法線とタンジェントと色とテクスチャを持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTangentColorTexture;
 用途: 位置と法線とタンジェントと色とテクスチャを持つ頂点の定義
****************************************************************************/
struct VertexPositionNormalTangentColorTexture
{
	VertexPositionNormalTangentColorTexture()
	{ }

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 tangent;
	uint32_t color;
	XMFLOAT2 textureCoordinate;

	VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		tangent(tangent),
		color(rgba),
		textureCoordinate(textureCoordinate)
	{
	}

	VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate)
		: color(rgba)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->tangent, tangent);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		tangent(tangent),
		textureCoordinate(textureCoordinate)
	{
		SetColor( color );
	}

	VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->tangent, tangent);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);

		SetColor( color );
	}

	void SetColor( XMFLOAT4 const& color ) { SetColor( XMLoadFloat4( &color ) ); }
	void XM_CALLCONV SetColor( FXMVECTOR color );

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTangentColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTangentColorTextureLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[];
 用途: 位置と法線とタンジェントと色とテクスチャとスキニング情報を持つ入力レイアウトの定義
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture;
 用途: 位置と法線とタンジェントと色とテクスチャとスキニング情報を持つ頂点の定義
****************************************************************************/
struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
{
	VertexPositionNormalTangentColorTextureSkinning()
	{ }

	uint32_t indices;
	uint32_t weights;

	VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba,
								XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate,
								XMUINT4 const& indices, CXMVECTOR weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color,
								XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate,
								XMUINT4 const& indices, CXMVECTOR weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	void SetBlendIndices( XMUINT4 const& indices );

	void SetBlendWeights( XMFLOAT4 const& weights ) { SetBlendWeights( XMLoadFloat4( &weights ) ); }
	void XM_CALLCONV SetBlendWeights( FXMVECTOR weights );

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTangentColorTextureSkinningLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTangentColorTextureSkinningLayout );
	}
};

/**************************************************************************
 class VertexUtil;
 用途: 頂点関連のユーティリティ関数群
 ＊static呼び出しをする
****************************************************************************/
class VertexUtil{
    static const float SQRT2;
    static const float SQRT3;
    static const float SQRT6;
/**************************************************************************
 用途:CreateGeoSphereで使用されるヘルパー
***************************************************************************/
    static inline void CheckIndexOverflow(size_t value)
    {
        // Use >=, not > comparison, because some D3D level 9_x hardware does not support 0xFFFF index values.
        if (value >= USHRT_MAX){
			throw BaseException(
				L"数がオーバーフローしました",
				L"if (value >= USHRT_MAX)",
				L"VertexUtil::CheckIndexOverflow()"
				);
		}
    }
/**************************************************************************
 用途: Cylinder / Coneで使用されるヘルパー
***************************************************************************/
	static inline XMVECTOR GetCircleVector(size_t i, size_t tessellation)
	{
		float angle = i * XM_2PI / tessellation;
		float dx, dz;

		XMScalarSinCos(&dx, &dz, angle);

		XMVECTORF32 v = { dx, 0, dz, 0 };
		return v;
	}
	static inline XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
	{
		float angle = ( i * XM_2PI / tessellation ) + XM_PIDIV2;
		float dx, dz;

		XMScalarSinCos(&dx, &dz, angle);

		XMVECTORF32 v = { dx, 0, dz, 0 };
		return v;
	}
	static void CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices,
			size_t tessellation, float height, float radius, bool isTop);
public:

/**************************************************************************
static void CreatePrimitiveBuffer(
	ID3D11Device* pDx11Device,	//デバイス
	const D3D11_BUFFER_DESC& bufferDesc,	//バッファデスクプリタ
	const D3D11_SUBRESOURCE_DATA& dataDesc,	//データデスクプリタ
	ID3D11Buffer** pBuffer		//頂点バッファ
)
用途: バッファを作成する
戻り値: なし
***************************************************************************/
	static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, const D3D11_BUFFER_DESC* pBufferDesc, const D3D11_SUBRESOURCE_DATA* pDataDesc, ID3D11Buffer** pBuffer){
		if (FAILED(pDx11Device->CreateBuffer(pBufferDesc, pDataDesc, pBuffer))){
			// 初期化失敗
			throw BaseException(
				L"バッファ作成に失敗しました。",
				L"if(FAILED(pDx11Device->CreateBuffer()))",
				L"VertexUtil::CreatePrimitiveBuffer()"
				);
		}
	}
/**************************************************************************
 template<typename T>
 static void CreatePrimitiveBuffer(
	ID3D11Device* pDx11Device,	//デバイス
	T const& data,				//データの配列
	D3D11_BIND_FLAG bindFlags,	//フラグ
	ID3D11Buffer** pBuffer		//バッファ
	)
 用途: バッファを作成する
 戻り値: なし
***************************************************************************/
	template<typename T>
	static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, T const& data, D3D11_BIND_FLAG bindFlags, ID3D11Buffer** pBuffer){
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		bufferDesc.BindFlags = bindFlags;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if(FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))){
			// 初期化失敗
			throw BaseException(
				L"バッファ作成に失敗しました。",
				L"if(FAILED(pDx11Device->CreateBuffer()))",
				L"VertexUtil::CreatePrimitiveBuffer()"
				);
		}
	}


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateVertexBuffer(
	//		ComPtr<ID3D11Buffer>& VertexBuffer,	//戻りバッファ
	//		T const& data						//頂点データ
	//	)
	//	用途: 頂点バッファを作成する
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//頂点バッファの作成
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_VERTEX_BUFFER, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateIndexBuffer(
	//		ComPtr<ID3D11Buffer>& IndexBuffer,	//戻りバッファ
	//		T const& data						//インデックスデータ
	//	)
	//	用途: インデックスバッファを作成する
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//インデックスバッファの作成
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_INDEX_BUFFER, pResult);
		});
	}



/**************************************************************************
 template<typename T>
 static void CreateDynamicPrimitiveVertexBuffer(
	ID3D11Device* pDx11Device,	//デバイス
	T const& data,				//データの配列
	ID3D11Buffer** pBuffer		//バッファ
	)
 用途: 頂点データを変更できる頂点バッファを作成する
 戻り値: なし
***************************************************************************/
	template<typename T>
	static void CreateDynamicPrimitiveVertexBuffer(ID3D11Device* pDx11Device, T const& data, ID3D11Buffer** pBuffer){
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if(FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))){
			// 初期化失敗
			throw BaseException(
			L"バッファ作成に失敗しました。",
			L"if(FAILED(pDx11Device->CreateBuffer()))",
			L"VertexUtil::CreateDynamicVertexBuffer()"
			);
		}
	}


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateDynamicPrimitiveVertexBuffer(
	//		ComPtr<ID3D11Buffer>& VertexBuffer,	//戻りバッファ
	//		T const& data						//頂点データ
	//	)
	//	用途: 頂点データを変更できる頂点バッファを作成する
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateDynamicVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//頂点バッファの作成
			VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, data, pResult);
		});
	}



/**************************************************************************
 static void ReverseWinding(
	vector<uint16_t>& indices,
	vector<VertexPositionNormalTexture>& vertices
 )
 用途: 頂点とインデックスの向きを反転させる（LHとRH変換）
 戻り値: なし
***************************************************************************/
    static void ReverseWinding(vector<uint16_t>& indices,vector<VertexPositionNormalTexture>& vertices)
    {
		if((indices.size() % 3) != 0){
			throw BaseException(
				L"インデックスの数が合いません",
				L"if((indices.size() % 3) == 0)",
				L"VertexUtil::ReverseWinding()"
				);
		}
        for( auto it = indices.begin(); it != indices.end(); it += 3 )
        {
            std::swap( *it, *(it+2) );
        }

        for( auto it = vertices.begin(); it != vertices.end(); ++it )
        {
			it->textureCoordinate.x = ( 1.f - it->textureCoordinate.x );
        }
    }
/**************************************************************************
static  void CreateSquare(
	float size,	//1つの辺のサイズ
	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
);
	用途: 平面を作成する(static関数)
	戻り値: なし
	＊VertexPosNormalTexフォーマットの平面を作成する
***************************************************************************/
	static void CreateSquare(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);

/**************************************************************************
 static  void CreateCube(
	float size,	//1つの辺のサイズ
	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
 );
 用途: 立方体を作成する(static関数)
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの立方体を作成する
***************************************************************************/
	static void CreateCube(float size,
		 vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateSphere(
	float diameter,	//直径サイズ
	size_t tessellation,	//分割
	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
 );
 用途: 球体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの球体を作成する
***************************************************************************/
	static void CreateSphere(float diameter,size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateCapsule(
	float diameter,				//直径サイズ
	const Vector3& PointA,		//中間部線分の開始点
	const Vector3& PointB,		//中間部線分の終了点
	size_t tessellation,	//分割
	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
 );
 用途: カプセルを作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの球体を作成する
***************************************************************************/
	static void CreateCapsule(float diameter,
		const Vector3& PointA,const Vector3& PointB,
		size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateGeoSphere(
	float diameter,	//直径サイズ
	size_t tessellation,	//分割
	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
 );
 用途: ジオメトリック球体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットのジオメトリック球体を作成する
***************************************************************************/
	static void CreateGeoSphere(float diameter, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateCylinder(
	float height,	//高さ
	float diameter,	//直径サイズ
	size_t tessellation,	//分割
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: シリンダーを作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットのシリンダーを作成する
***************************************************************************/
	static void CreateCylinder(float height,float diameter,size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateCone(
	float diameter,	//直径サイズ
	float height,	//高さ
	size_t tessellation,	//分割数
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	)
 用途: コーンを作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットのコーンを作成する
***************************************************************************/
	static void CreateCone(float diameter, float height, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateTorus(
	float diameter,			//ドーナッツ中心までの直径
	float thickness,		//ドーナッツ太さ
	size_t tessellation,	//分割
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: トーラスを作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットのトーラスを作成する
***************************************************************************/
	static void CreateTorus(float diameter, float thickness, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateTetrahedron(
	float size,		//サイズ
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: 正4面体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの正4面体を作成する
***************************************************************************/
	static void CreateTetrahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateOctahedron(
	float size,	//サイズ
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: 正8面体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの正8面体を作成する
***************************************************************************/
	static void CreateOctahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateDodecahedron(
	float size,	//サイズ
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: 正12面体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの正12面体を作成する
***************************************************************************/
	static void CreateDodecahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateIcosahedron(
	float size,	//サイズ
	vector<VertexPositionNormalTexture>& vertices,	//頂点を作成するための配列
	vector<uint16_t>& indices	//インデックスを作成するための配列
	);
 用途: 正20面体を作成する
 戻り値: なし
 ＊VertexPosNormalTexフォーマットの正20面体を作成する
***************************************************************************/
	static void CreateIcosahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);

};


}
//end of namespace basedx11.
