#include "pch.h"
#include "PhdXml.h"
#include "tinyxml.h"

namespace Phd{

PhdXml::PhdXml()
	:m_pDocument(nullptr)
	, m_pDeclaration(nullptr)
	, m_pElementRoot(nullptr)
{
}


PhdXml::~PhdXml()
{
	if (m_pDocument)
	{
		m_pDocument->Clear();	//清除子节点
		delete m_pDocument;		//释放自己
		m_pDocument = NULL;
	}
}

bool PhdXml::OpenXmlFile(LPCTSTR szFilePath)
{
	//得到xml文档指针
	m_strXmlPath = szFilePath;
	m_pDocument = new TiXmlDocument();
	if (!m_pDocument->LoadFile(m_strXmlPath))
		return false;
	//得到声明指针
	TiXmlNode* pNode = m_pDocument->FirstChild();
	if (NULL != pNode)
		m_pDeclaration = pNode->ToDeclaration();
	//得到根节点指针
// 	TiXmlHandle handleDoc(m_pDocument);
// 	m_pElementRoot = handleDoc.FirstChildElement().Element();
	m_pElementRoot = m_pDocument->RootElement();

	return true;
}

bool PhdXml::GetDeclaration(CString& strVersion, CString& strEncoding, CString& strStandalone) const
{
	USES_CONVERSION;

	if (NULL != m_pDeclaration)
	{
		strVersion = A2T(m_pDeclaration->Version());
		strEncoding = A2T(m_pDeclaration->Encoding());
		strStandalone = A2T(m_pDeclaration->Standalone());
		return true;
	}
	return false;
}

TiXmlElement* PhdXml::GetRootNode() const
{
	return m_pElementRoot;
}

CString PhdXml::GetRootNodeName() const
{
	USES_CONVERSION;
	CString strName = A2T(m_pElementRoot->Value());
	return strName;
}

CString PhdXml::GetRootNodeText() const
{
	USES_CONVERSION;
	CString strText = A2T(m_pElementRoot->GetText());
	return strText;
}

bool PhdXml::FindNode(LPCTSTR nodeName, TiXmlElement*& pNode) const
{
	CString strRootNodeName = GetRootNodeName();
	if (_tcscmp(nodeName, strRootNodeName) == 0)
	{
		pNode = m_pElementRoot;
		return true;
	}

	TiXmlElement* p = m_pElementRoot;
	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		FindNode(p, nodeName, pNode);
	}

	return false;
}

bool PhdXml::FindNode(TiXmlElement *pRoot, LPCTSTR nodeName, TiXmlElement*& pNode) const
{
	CString strNodeName = GetNodeName(pRoot);
	if (_tcscmp(nodeName, strNodeName) == 0)
	{
		pNode = pRoot;
		return true;
	}

	TiXmlElement* p = pRoot;
	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		FindNode(p, nodeName, pNode);
	}

	return false;
}

bool PhdXml::FindNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath, TiXmlElement*& pFindNode) const
{
	USES_CONVERSION;

	CString strTemp = szNodePath;
	std::vector<CString> vecNode;
	while (true)
	{
		int nFindIndex = strTemp.Find(_T('\\'));
		if (-1 == nFindIndex)
		{
			if (strTemp != _T(""))
				vecNode.push_back(strTemp);
			break;
		}
		CString strNode = strTemp.Left(nFindIndex);
		if (strNode != _T(""))
			vecNode.push_back(strNode);
		strTemp = strTemp.Right(strTemp.GetLength() - nFindIndex - 1);
	}
	std::size_t nCount = vecNode.size();
	if (0 == nCount)
		return false;
	//
	TiXmlElement* pTempRoot = pRoot;
	TiXmlElement* pTempFind = NULL;
	for (int i = 0; i < nCount; i++)
	{
		if (!FindChildNode(pTempRoot, vecNode[i], pTempFind))
			return false;
		pTempRoot = pTempFind;
	}
	pFindNode = pTempFind;
	return true;
}

bool PhdXml::AddNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath,
	LPCTSTR newNodeName, LPCTSTR text /*= NULL*/)
{
	USES_CONVERSION;

	CString strTemp = szNodePath;
	std::vector<CString> vecNode;
	while (true)
	{
		int nFindIndex = strTemp.Find(_T('\\'));
		if (-1 == nFindIndex)
		{
			if (strTemp != _T(""))
				vecNode.push_back(strTemp);
			break;
		}
		CString strNode = strTemp.Left(nFindIndex);
		if (strNode != _T(""))
			vecNode.push_back(strNode);
		strTemp = strTemp.Right(strTemp.GetLength() - nFindIndex - 1);
	}
	std::size_t nCount = vecNode.size();
	if (0 == nCount)
	{//在根节点下添加新节点
		TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
		if (NULL != text)
		{
			TiXmlText *pNewText = new TiXmlText(T2A(text));
			pNewNode->LinkEndChild(pNewText);
		}
		m_pElementRoot->InsertEndChild(*pNewNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));

		return true;
	}
	else
	{//在节点路径下添加新节点
		TiXmlHandle hRoot(pRoot);       // hRoot是根节点
		TiXmlHandle hTemp = hRoot;
		for (int i = 0; i < nCount; i++)
		{
			hTemp = hTemp.FirstChild(T2A(vecNode[i]));
			if (!hTemp.ToElement())
				return false;
		}

		TiXmlElement* pFindNode = hTemp.Element();

		TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
		if (NULL != text)
		{
			TiXmlText *pNewText = new TiXmlText(T2A(text));
			pNewNode->LinkEndChild(pNewText);
		}
		pFindNode->InsertEndChild(*pNewNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));

		return true;
	}
}

bool PhdXml::AddNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath, LPCTSTR newNodeName, LPCTSTR text /*= NULL*/)
{
	USES_CONVERSION;

	CString strTemp = szNodePath;
	std::vector<CString> vecNode;
	while (true)
	{
		int nFindIndex = strTemp.Find(_T('\\'));
		if (-1 == nFindIndex)
		{
			if (strTemp != _T(""))
				vecNode.push_back(strTemp);
			break;
		}
		CString strNode = strTemp.Left(nFindIndex);
		if (strNode != _T(""))
			vecNode.push_back(strNode);
		strTemp = strTemp.Right(strTemp.GetLength() - nFindIndex - 1);
	}
	std::size_t nCount = vecNode.size();
	if (0 == nCount)
	{//在根节点下添加新节点
		TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
		if (NULL != text)
		{
			TiXmlText *pNewText = new TiXmlText(T2A(text));
			pNewNode->LinkEndChild(pNewText);
		}
		m_pElementRoot->InsertEndChild(*pNewNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));

		return true;
	}
	else
	{//在节点路径下添加新节点
		TiXmlElement* pTempRoot = pRoot;
		TiXmlElement* pTempFind = NULL;
		for (int i = 0; i < nCount; i++)
		{
			if (!FindChildNodeByText(pTempRoot, vecNode[i], pTempFind))
				return false;
			pTempRoot = pTempFind;
		}

		TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
		if (NULL != text)
		{
			TiXmlText *pNewText = new TiXmlText(T2A(text));
			pNewNode->LinkEndChild(pNewText);
		}
		pTempFind->InsertEndChild(*pNewNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));

		return true;
	}
}

bool PhdXml::FindNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath, TiXmlElement*& pFindNode) const
{
	USES_CONVERSION;

	CString strTemp = szNodePath;
	std::vector<CString> vecNode;
	while (true)
	{
		int nFindIndex = strTemp.Find(_T('\\'));
		if (-1 == nFindIndex)
		{
			if (strTemp != _T(""))
				vecNode.push_back(strTemp);
			break;
		}
		CString strNode = strTemp.Left(nFindIndex);
		if (strNode != _T(""))
			vecNode.push_back(strNode);
		strTemp = strTemp.Right(strTemp.GetLength() - nFindIndex - 1);
	}
	std::size_t nCount = vecNode.size();
	if (0 == nCount)
		return false;
	//
	TiXmlElement* pTempRoot = pRoot;
	TiXmlElement* pTempFind = NULL;
	for (int i = 0; i < nCount; i++)
	{
		if (!FindChildNodeByText(pTempRoot, vecNode[i], pTempFind))
			return false;
		pTempRoot = pTempFind;
	}
	pFindNode = pTempFind;
	return true;
}

bool PhdXml::InitXmlByDirPath(LPCTSTR szDirPath, TiXmlElement* pRootNode) const
{
	BOOL bRet = ::PathIsDirectory(szDirPath);//判断路径是不是文件夹
	if (!bRet)
		return FALSE;

	CFileFind ff;
	CString szPathAll = szDirPath;
	szPathAll += _T("\\*.*");
	BOOL b = ff.FindFile(szPathAll);
	if (!b)
		return FALSE;

	int i = 0;
	while (b)
	{
		b = ff.FindNextFileW();
		if (ff.IsDots())
			continue;
		CString strFileName = ff.GetFileName();
		CString strFilePath = ff.GetFilePath();
		if (ff.IsDirectory())
		{
			TiXmlElement* pNode = AddNode(pRootNode, _T("folder"), strFileName);
			InitXmlByDirPath(strFilePath, pNode);
		}
		else
		{
			CString strSuffix = strFileName;
			int nIndex = strSuffix.ReverseFind(_T('.'));
			if (nIndex != -1)
				strSuffix = strSuffix.Right(strSuffix.GetLength() - nIndex - 1);
			if (strSuffix.CompareNoCase(_T("dwg")) == 0)
			{
				AddNode(pRootNode, _T("dwg"), strFileName);
			}
		}
		i++;
	}
	ff.Close();

	return TRUE;
}

bool PhdXml::FindChildNode(TiXmlElement* pNode, LPCTSTR szName, TiXmlElement*& pFindNode) const
{
	USES_CONVERSION;

	TiXmlElement* p = pNode;
	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		CString name = A2T(p->Value());
		if (_tcscmp(name, szName) == 0)
		{
			pFindNode = p;
			return true;
		}
	}
	return false;
}

bool PhdXml::FindChildNodeByText(TiXmlElement* pNode, LPCTSTR szText, TiXmlElement*& pFindNode) const
{
	USES_CONVERSION;

	TiXmlElement* p = pNode;
	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		CString text = A2T(p->GetText());
		if (_tcscmp(text, szText) == 0)
		{
			pFindNode = p;
			return true;
		}
	}
	return false;
}

CString PhdXml::GetNodeName(TiXmlElement* pNode) const
{
	USES_CONVERSION;
	CString name = A2T(pNode->Value());
	return name;
}

bool PhdXml::GetNodeText(LPCTSTR nodeName, CString& text) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		text = A2T(pNode->GetText());
		return true;
	}

	return false;
}

CString PhdXml::GetNodeText(TiXmlElement* pNode) const
{
	USES_CONVERSION;
	CString text = A2T(pNode->GetText());
	return text;
}

bool PhdXml::GetNodeAttribute(LPCTSTR nodeName, std::map<CString, CString>& mapAttribute) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		TiXmlAttribute *pAttr = NULL;
		for (pAttr = pNode->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
		{
			CString name = A2T(pAttr->Name());		//属性名
			CString value = A2T(pAttr->Value());	//属性值
			mapAttribute.insert(std::make_pair(name, value));
		}

		return true;
	}

	return false;
}

std::map<CString, CString> PhdXml::GetNodeAttribute(TiXmlElement* pNode) const
{
	USES_CONVERSION;

	std::map<CString, CString> mapAttribute;
	TiXmlAttribute *pAttr = NULL;
	for (pAttr = pNode->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
	{
		CString name = A2T(pAttr->Name());		//属性名
		CString value = A2T(pAttr->Value());	//属性值
		mapAttribute.insert(std::make_pair(name, value));
	}
	return mapAttribute;
}

bool PhdXml::DeleteNode(LPCTSTR nodeName) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		if (pNode == m_pElementRoot)
		{//如果是根节点
			m_pDocument->RemoveChild(m_pElementRoot);
			//m_pDocument->SaveFile(T2A(m_strXmlPath));
			return true;
		}
		else
		{//子节点
			TiXmlNode *parent = pNode->Parent();//找到该节点的父节点
			if (NULL == parent)
			{
				return false;
			}

			TiXmlElement *parentElem = parent->ToElement();
			if (NULL == parentElem)
			{
				return false;
			}
			parentElem->RemoveChild(pNode);
			//m_pDocument->SaveFile(T2A(m_strXmlPath));
			return true;
		}
	}

	return false;
}

bool PhdXml::DeleteNode(TiXmlElement* pNode) const
{
	USES_CONVERSION;

	if (pNode == m_pElementRoot)
	{//如果是根节点
		m_pDocument->RemoveChild(m_pElementRoot);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
		return true;
	}
	else
	{//子节点
		TiXmlNode *parent = pNode->Parent();//找到该节点的父节点
		if (NULL == parent)
			return false;

		TiXmlElement *parentElem = parent->ToElement();
		if (NULL == parentElem)
			return false;

		parentElem->RemoveChild(pNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
		return true;
	}
}

bool PhdXml::DeleteNodeAllText(TiXmlElement* pNode) const
{
	USES_CONVERSION;

	TiXmlNode* pFirstNode = pNode->FirstChild();
	for (TiXmlNode* p = pFirstNode; p != NULL; p = p->NextSibling())
	{
		pNode->RemoveChild(p);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
	}

	return true;
}

bool PhdXml::DeleteAllChildNode(TiXmlElement* pNode) const
{
	USES_CONVERSION;

	TiXmlElement* pFirstNode = pNode->FirstChildElement();
	if (!pFirstNode)
		return true;
	for (TiXmlElement* p = pFirstNode; p != NULL; p = p->NextSiblingElement())
	{
		pNode->RemoveChild(p);
	}
	//m_pDocument->SaveFile(T2A(m_strXmlPath));
	return true;
}

bool PhdXml::DeleteAttribute(TiXmlElement* pNode, LPCTSTR szAttName) const
{
	USES_CONVERSION;

	pNode->RemoveAttribute(T2A(szAttName));
	//m_pDocument->SaveFile(T2A(m_strXmlPath))
	return true;
}

bool PhdXml::ModifyNodeText(LPCTSTR nodeName, LPCTSTR text) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		//pNode->Clear();//删除原节点下的其他元素
		TiXmlText *pText = new TiXmlText(T2A(text));
		pNode->LinkEndChild(pText);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
		return true;
	}

	return false;
}

bool PhdXml::SetNodeText(TiXmlElement* pNode, LPCTSTR text) const
{
	USES_CONVERSION;

	DeleteNodeAllText(pNode);

	TiXmlText *pText = new TiXmlText(T2A(text));
	pNode->LinkEndChild(pText);
	//m_pDocument->SaveFile(T2A(m_strXmlPath))
	return true;
}

bool PhdXml::SetNodeAttribute(TiXmlElement* pNode, const std::map<CString, CString>& mapAttribute) const
{
	USES_CONVERSION;

	std::map<CString, CString> mapCurAtt = GetNodeAttribute(pNode);
	std::map<CString, CString>::const_iterator iterCur;
	for (iterCur = mapCurAtt.begin(); iterCur != mapCurAtt.end(); iterCur++)
	{
		pNode->RemoveAttribute(T2A(iterCur->first));
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
	}

	std::map<CString, CString>::const_iterator iter;
	for (iter = mapAttribute.begin(); iter != mapAttribute.end(); iter++)
	{
		pNode->SetAttribute(T2A(iter->first), T2A(iter->second));
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
	}
	return true;
}

bool PhdXml::SetNodeAttribute(TiXmlElement* pNode, LPCTSTR szAttName, LPCTSTR szAttValue) const
{
	USES_CONVERSION;

	pNode->SetAttribute(T2A(szAttName), T2A(szAttValue));
	//m_pDocument->SaveFile(T2A(m_strXmlPath))
	return true;
}

bool PhdXml::ModifyNodeAttribution(LPCTSTR nodeName, const std::map<CString, CString> &mapAttribute) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		TiXmlAttribute *pAttr = pNode->FirstAttribute();
		CString strName;
		for (; pAttr != NULL; pAttr = pAttr->Next())
		{
			strName = A2T(pAttr->Name());
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if (_tcscmp(strName, it->first) == 0)
				{
					pNode->SetAttribute(T2A(strName), T2A(it->second));
				}
			}
		}
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
		return true;
	}

	return false;
}

bool PhdXml::ModifyNodeAttribution(TiXmlElement* pNode, const std::map<CString, CString> &mapAttribute) const
{
	USES_CONVERSION;

	TiXmlAttribute *pAttr = pNode->FirstAttribute();
	CString strName;
	for (; pAttr != NULL; pAttr = pAttr->Next())
	{
		strName = A2T(pAttr->Name());
		for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
		{
			if (_tcscmp(strName, it->first) == 0)
			{
				pNode->SetAttribute(T2A(strName), T2A(it->second));
			}
		}
	}
	//m_pDocument->SaveFile(T2A(m_strXmlPath));
	return true;
}

bool PhdXml::AddNode(LPCTSTR nodeName, LPCTSTR newNodeName, LPCTSTR text /*= NULL*/) const
{
	USES_CONVERSION;

	TiXmlElement *pNode = NULL;
	if (FindNode(m_pElementRoot, nodeName, pNode))
	{
		TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
		if (NULL != text)
		{
			TiXmlText *pNewText = new TiXmlText(T2A(text));
			pNewNode->LinkEndChild(pNewText);
		}
		pNode->InsertEndChild(*pNewNode);
		//m_pDocument->SaveFile(T2A(m_strXmlPath));
		return true;
	}

	return false;
}

TiXmlElement* PhdXml::AddNode(TiXmlElement* pNode, LPCTSTR newNodeName, LPCTSTR text /*= NULL*/) const
{
	USES_CONVERSION;

	TiXmlElement *pNewNode = new TiXmlElement(T2A(newNodeName));
	if (NULL != text)
	{
		TiXmlText *pNewText = new TiXmlText(T2A(text));
		pNewNode->LinkEndChild(pNewText);
	}
	TiXmlNode* pRealNode = pNode->InsertEndChild(*pNewNode);
	//	bool bRt = m_pDocument->SaveFile(T2A(m_strXmlPath));

	if (pRealNode)
		return pRealNode->ToElement();
	else
		return NULL;
}

bool PhdXml::AddNodeText(TiXmlElement* pNode, LPCTSTR text) const
{
	USES_CONVERSION;

	TiXmlText *pNewText = new TiXmlText(T2A(text));
	TiXmlNode* pRealNode = pNode->LinkEndChild(pNewText);
	//m_pDocument->SaveFile(T2A(m_strXmlPath));
	return true;
}

bool PhdXml::SetNodeName(TiXmlElement* pNode, LPCTSTR name) const
{
	USES_CONVERSION;

	pNode->SetValue(T2A(name));
	//m_pDocument->SaveFile(T2A(m_strXmlPath));
	return true;
}

TiXmlElement* PhdXml::GetParentNode(TiXmlElement* pNode) const
{
	TiXmlNode *parent = pNode->Parent();//找到该节点的父节点
	if (!parent)
		return NULL;
	return parent->ToElement();
}

std::vector<TiXmlElement*> PhdXml::GetChildNodes(TiXmlElement* pNode) const
{
	std::vector<TiXmlElement*> vecNodes;
	TiXmlElement* p = NULL;
	for (p = pNode->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		vecNodes.push_back(p);
	}
	return vecNodes;
}

bool PhdXml::CreateXmlFile(LPCTSTR szXmlPath, LPCTSTR szRootName, LPCTSTR szRootText /*= NULL*/)
{
	USES_CONVERSION;

	m_strXmlPath = szXmlPath;

	//创建XML文档指针
	m_pDocument = new TiXmlDocument();
	if (!m_pDocument)
		return false;

	//声明XML
	m_pDeclaration = new TiXmlDeclaration("1.0", "gb2312", "");
	if (!m_pDeclaration)
		return false;

	m_pDocument->LinkEndChild(m_pDeclaration);

	//创建根节点
	m_pElementRoot = new TiXmlElement(T2A(szRootName));
	if (!m_pElementRoot)
		return false;
	if (szRootText)
	{
		TiXmlText* pTextRoot = new TiXmlText(T2A(szRootText));
		m_pElementRoot->LinkEndChild(pTextRoot);
	}

	//关联XML文档，成为XML文档的根节点
	m_pDocument->LinkEndChild(m_pElementRoot);
	//m_pDocument->SaveFile(T2A(szXmlPath));

	return true;
}

bool PhdXml::Save()
{
	USES_CONVERSION;
	bool bRt = m_pDocument->SaveFile(T2A(m_strXmlPath));
	return bRt;
}

}