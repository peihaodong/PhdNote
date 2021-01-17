#pragma once

/***********************************************
   >   Class Name: PhdXml
   >     Describe: ͨ��tinyxml�ӿڿɶ�xml�ļ����ж�д�ķ�װ��
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/

class TiXmlDocument;
class TiXmlDeclaration;
class TiXmlElement;

//USES_CONVERSION;ת����	T2A��wchar_t->char	A2T��char->wchar_t

namespace Phd{

class PHD_NOTE_API PhdXml
{
public:
	PhdXml();
	~PhdXml();

#pragma region	����
	// Summary:   ��ָ����xml�ļ�
	// Time:	  2019��10��25��
	// Explain:	  
	bool OpenXmlFile(LPCTSTR szFilePath);

	// Summary:   ����xml�ļ�
	// Time:	  2019��10��26��
	// Explain:	  
	bool CreateXmlFile(LPCTSTR szXmlPath, LPCTSTR szRootName, LPCTSTR szRootText = NULL);

	bool Save();

	// Summary:   �õ����ڵ�ָ��
	// Time:	  2019��10��26��
	// Explain:	  
	TiXmlElement* GetRootNode() const;

	// Summary:   �õ��ڵ���
	// Time:	  2019��10��26��
	// Explain:	  
	CString GetNodeName(TiXmlElement* pNode) const;

	// Summary:   �õ��ڵ��ı�
	// Time:	  2019��11��2��
	// Explain:	  
	CString GetNodeText(TiXmlElement* pNode) const;

	// Summary:   �õ��ڵ�����
	// Time:	  2019��11��2��
	// Explain:	  
	std::map<CString, CString> GetNodeAttribute(TiXmlElement* pNode) const;

	// Summary:   �õ�ĳ�ڵ�ĸ��ڵ�
	// Time:	  2019��10��26��
	// Explain:	  
	TiXmlElement* GetParentNode(TiXmlElement* pNode) const;

	// Summary:   �õ�ĳ�ڵ�������ӽڵ�
	// Time:	  2019��10��26��
	// Explain:	  
	std::vector<TiXmlElement*> GetChildNodes(TiXmlElement* pNode) const;

	// Summary:   ��ӽڵ���ӽڵ�
	// Time:	  2019��11��2��
	// Explain:	  
	TiXmlElement* AddNode(TiXmlElement* pNode, LPCTSTR newNodeName, LPCTSTR text = NULL) const;

	// Summary:   ��ӽڵ��ı�
	// Time:	  2019��11��2��
	// Explain:	  
	bool AddNodeText(TiXmlElement* pNode, LPCTSTR text) const;

	// Summary:   ���ýڵ���
	// Time:	  2019��11��2��
	// Explain:	  
	bool SetNodeName(TiXmlElement* pNode, LPCTSTR name) const;

	// Summary:   ���ýڵ��ı�
	// Time:	  2019��11��2��
	// Explain:	  
	bool SetNodeText(TiXmlElement* pNode, LPCTSTR text) const;

	// Summary:   ���ýڵ�����
	// Time:	  2019��11��2��
	// Explain:	  
	bool SetNodeAttribute(TiXmlElement* pNode, LPCTSTR szAttName, LPCTSTR szAttValue) const;
	bool SetNodeAttribute(TiXmlElement* pNode,
		const std::map<CString, CString>& mapAttribute) const;

	// Summary:   ɾ���ýڵ�
	// Time:	  2019��11��2��
	// Explain:	  
	bool DeleteNode(TiXmlElement* pNode) const;

	// Summary:   ɾ���ڵ��ı�
	// Time:	  2019��11��2��
	// Explain:	  
	bool DeleteNodeAllText(TiXmlElement* pNode) const;

	// Summary:   ɾ���ýڵ�������ӽڵ�
	// Time:	  2019��11��2��
	// Explain:	  
	bool DeleteAllChildNode(TiXmlElement* pNode) const;

	// Summary:   ɾ���ڵ�����
	// Time:	  2019��11��2��
	// Explain:	  
	bool DeleteAttribute(TiXmlElement* pNode, LPCTSTR szAttName) const;

	// Summary:   ͨ���ļ���·����ʼ��xml�ļ�
	// Time:	  2019��11��1��
	// Explain:	  
	bool InitXmlByDirPath(LPCTSTR szDirPath, TiXmlElement* pRootNode) const;

	// Summary:   ���ҽڵ���ӽڵ�
	// Time:	  2019��11��4�� peihaodong
	// Explain:	  
	bool FindChildNode(TiXmlElement* pNode, LPCTSTR szName, TiXmlElement*& pFindNode) const;

	// Summary:   ͨ���ı��ҵ��ڵ���ӽڵ�
	// Time:	  2019��11��4�� peihaodong
	// Explain:	  
	bool FindChildNodeByText(TiXmlElement* pNode, LPCTSTR szText, TiXmlElement*& pFindNode) const;

#pragma endregion

#pragma region ͨ��·��
	// Summary:   ���ݸ��ڵ�ͽڵ�·��Ѱ�ҽڵ�
	// Time:	  2019��10��26��
	// Explain:	  szNodePath��network\\nodes\\node		Ѱ��pRoot�ڵ���node�ڵ�
	bool FindNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath, TiXmlElement*& pFindNode) const;

	// Summary:   ���ݽڵ�·����ӽڵ�
	// Time:	  2019��10��31��
	// Explain:	  szNodePath��network\\node		��pRoot���ڵ��µ�network�ڵ��µ�node�ڵ���
	//���newNodeName�½ڵ㣬���Ҫ�ڸ�Ŀ¼����ӽڵ㣬szNodePath->_T("")
	bool AddNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath, LPCTSTR newNodeName,
		LPCTSTR text = NULL);

	// Summary:   
	// Time:	  2019��11��4�� peihaodong
	// Explain:	  
	bool AddNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath, LPCTSTR newNodeName,
		LPCTSTR text = NULL);

	bool FindNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath,
		TiXmlElement*& pFindNode) const;

#pragma endregion

	// Summary:   ��ȡ������Ϣ
	// Time:	  2019��10��25��
	// Explain:	  
	bool GetDeclaration(CString& strVersion, CString& strEncoding, CString& strStandalone) const;

	// Summary:   �õ����ڵ���
	// Time:	  2019��10��25��
	// Explain:	  
	CString GetRootNodeName() const;

	// Summary:   �õ����ڵ��ı�
	// Time:	  2019��10��25��
	// Explain:	  
	CString GetRootNodeText() const;

	// Summary:   ���ݽڵ������жϽڵ��Ƿ���ڣ������ظýڵ�ָ��
	// Time:	  2019��10��25��
	// Explain:	  
	bool FindNode(LPCTSTR nodeName, TiXmlElement*& pNode) const;

	// Summary:   ��ȡ�ڵ��ı�
	// Time:	  2019��10��25��
	// Explain:	  
	bool GetNodeText(LPCTSTR nodeName, CString& text) const;

	// Summary:   ��ȡ�ڵ�����
	// Time:	  2019��10��25��
	// Explain:	  
	bool GetNodeAttribute(LPCTSTR nodeName, std::map<CString, CString>& mapAttribute) const;

	// Summary:   ɾ���ڵ�
	// Time:	  2019��10��25��
	// Explain:	  
	bool DeleteNode(LPCTSTR nodeName) const;

	// Summary:   �޸Ľڵ��ı�
	// Time:	  2019��10��25��
	// Explain:	  
	bool ModifyNodeText(LPCTSTR nodeName, LPCTSTR text) const;

	// Summary:   �޸Ľڵ�����
	// Time:	  2019��10��25��
	// Explain:	  
	bool ModifyNodeAttribution(LPCTSTR nodeName,
		const std::map<CString, CString> &mapAttribute) const;
	bool ModifyNodeAttribution(TiXmlElement* pNode,
		const std::map<CString, CString> &mapAttribute) const;

	// Summary:   ���ĳһ�ڵ���ӽڵ�
	// Time:	  2019��10��25��
	// Explain:	  
	bool AddNode(LPCTSTR nodeName, LPCTSTR newNodeName, LPCTSTR text = NULL) const;

private:
	bool FindNode(TiXmlElement *pRoot, LPCTSTR nodeName, TiXmlElement*& pNode) const;


private:
	CString m_strXmlPath;

	TiXmlDocument* m_pDocument;			//�ĵ�
	TiXmlDeclaration* m_pDeclaration;	//����
	TiXmlElement* m_pElementRoot;		//���ڵ�Ԫ��
};

}