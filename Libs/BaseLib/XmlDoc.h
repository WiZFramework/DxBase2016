#pragma once

#include "StdAfx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class XmlDocReader;
	//	用途: XML読み込みオンリークラス
	//--------------------------------------------------------------------------------------
	class XmlDocReader {
	protected:
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
	public:
		//構築と消滅
		XmlDocReader(const wstring& StrReadContext, bool IsMemory = false);
		virtual ~XmlDocReader();
		//操作
		//XPathによる取得
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;

		//Node等から取得するstatic関数
		static long GetLength(const IXMLDOMNodeListPtr& NodeList);
		static IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index);
		static IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node);
		static wstring GetBaseName(const IXMLDOMNodePtr& Node);
		static wstring GetText(const IXMLDOMNodePtr& Node);
		static wstring GetXML(const IXMLDOMNodePtr& Node);
		static wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag);
		static IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
		static IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
	};



	//--------------------------------------------------------------------------------------
	//	class XmlDoc;
	//	用途: XMLクラス
	//--------------------------------------------------------------------------------------
	class XmlDoc{
		void CheckXmlDoc() const;
		IXMLDOMNodePtr CreateNodeBase(const CComBSTR& NodeName);
		IXMLDOMNodePtr AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode);
	public:
		//構築と消滅
		XmlDoc(const wstring& Filename);
		virtual ~XmlDoc();
		//操作
		void Save(const wstring& Filename, bool Exp = true);
		//XPathによる取得
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;

		//Node等から取得するstatic関数
		long GetLength(const IXMLDOMNodeListPtr& NodeList)const;
		IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index) const;
		IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node)const;
		wstring GetBaseName(const IXMLDOMNodePtr& Node) const;
		wstring GetText(const IXMLDOMNodePtr& Node) const;
		void SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text);
		//ノード内XMLへのアクセスはリードオンリー
		wstring GetXML(const IXMLDOMNodePtr& Node) const;

		wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag)const;
		void SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue);

		IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;

		IXMLDOMNodePtr AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		XmlDoc(const XmlDoc&) = delete;
		XmlDoc& operator=(const XmlDoc&) = delete;
		//ムーブ禁止
		XmlDoc(const XmlDoc&&) = delete;
		XmlDoc& operator=(const XmlDoc&&) = delete;
	};



}
//endof  basedx11

