#pragma once

/***********************************************
   >   Class Name: PhdXml
   >     Describe: 通过tinyxml接口可对xml文件进行读写的封装类
   >       Author: peihaodong
   > Created Time: 2020年9月30日
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/

class TiXmlDocument;
class TiXmlDeclaration;
class TiXmlElement;

//USES_CONVERSION;转换宏	T2A：wchar_t->char	A2T：char->wchar_t

namespace Phd{

class PHD_NOTE_API PhdXml
{
public:
	PhdXml();
	~PhdXml();

#pragma region	常用
	// Summary:   打开指定的xml文件
	// Time:	  2019年10月25日
	// Explain:	  
	bool OpenXmlFile(LPCTSTR szFilePath);

	// Summary:   创建xml文件
	// Time:	  2019年10月26日
	// Explain:	  
	bool CreateXmlFile(LPCTSTR szXmlPath, LPCTSTR szRootName, LPCTSTR szRootText = NULL);

	bool Save();

	// Summary:   得到根节点指针
	// Time:	  2019年10月26日
	// Explain:	  
	TiXmlElement* GetRootNode() const;

	// Summary:   得到节点名
	// Time:	  2019年10月26日
	// Explain:	  
	CString GetNodeName(TiXmlElement* pNode) const;

	// Summary:   得到节点文本
	// Time:	  2019年11月2日
	// Explain:	  
	CString GetNodeText(TiXmlElement* pNode) const;

	// Summary:   得到节点属性
	// Time:	  2019年11月2日
	// Explain:	  
	std::map<CString, CString> GetNodeAttribute(TiXmlElement* pNode) const;

	// Summary:   得到某节点的父节点
	// Time:	  2019年10月26日
	// Explain:	  
	TiXmlElement* GetParentNode(TiXmlElement* pNode) const;

	// Summary:   得到某节点的所有子节点
	// Time:	  2019年10月26日
	// Explain:	  
	std::vector<TiXmlElement*> GetChildNodes(TiXmlElement* pNode) const;

	// Summary:   添加节点的子节点
	// Time:	  2019年11月2日
	// Explain:	  
	TiXmlElement* AddNode(TiXmlElement* pNode, LPCTSTR newNodeName, LPCTSTR text = NULL) const;

	// Summary:   添加节点文本
	// Time:	  2019年11月2日
	// Explain:	  
	bool AddNodeText(TiXmlElement* pNode, LPCTSTR text) const;

	// Summary:   设置节点名
	// Time:	  2019年11月2日
	// Explain:	  
	bool SetNodeName(TiXmlElement* pNode, LPCTSTR name) const;

	// Summary:   设置节点文本
	// Time:	  2019年11月2日
	// Explain:	  
	bool SetNodeText(TiXmlElement* pNode, LPCTSTR text) const;

	// Summary:   设置节点属性
	// Time:	  2019年11月2日
	// Explain:	  
	bool SetNodeAttribute(TiXmlElement* pNode, LPCTSTR szAttName, LPCTSTR szAttValue) const;
	bool SetNodeAttribute(TiXmlElement* pNode,
		const std::map<CString, CString>& mapAttribute) const;

	// Summary:   删除该节点
	// Time:	  2019年11月2日
	// Explain:	  
	bool DeleteNode(TiXmlElement* pNode) const;

	// Summary:   删除节点文本
	// Time:	  2019年11月2日
	// Explain:	  
	bool DeleteNodeAllText(TiXmlElement* pNode) const;

	// Summary:   删除该节点的所有子节点
	// Time:	  2019年11月2日
	// Explain:	  
	bool DeleteAllChildNode(TiXmlElement* pNode) const;

	// Summary:   删除节点属性
	// Time:	  2019年11月2日
	// Explain:	  
	bool DeleteAttribute(TiXmlElement* pNode, LPCTSTR szAttName) const;

	// Summary:   通过文件夹路径初始化xml文件
	// Time:	  2019年11月1日
	// Explain:	  
	bool InitXmlByDirPath(LPCTSTR szDirPath, TiXmlElement* pRootNode) const;

	// Summary:   查找节点的子节点
	// Time:	  2019年11月4日 peihaodong
	// Explain:	  
	bool FindChildNode(TiXmlElement* pNode, LPCTSTR szName, TiXmlElement*& pFindNode) const;

	// Summary:   通过文本找到节点的子节点
	// Time:	  2019年11月4日 peihaodong
	// Explain:	  
	bool FindChildNodeByText(TiXmlElement* pNode, LPCTSTR szText, TiXmlElement*& pFindNode) const;

#pragma endregion

#pragma region 通过路径
	// Summary:   根据根节点和节点路径寻找节点
	// Time:	  2019年10月26日
	// Explain:	  szNodePath：network\\nodes\\node		寻找pRoot节点下node节点
	bool FindNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath, TiXmlElement*& pFindNode) const;

	// Summary:   根据节点路径添加节点
	// Time:	  2019年10月31日
	// Explain:	  szNodePath：network\\node		在pRoot根节点下的network节点下的node节点下
	//添加newNodeName新节点，如果要在根目录下添加节点，szNodePath->_T("")
	bool AddNodeByPath(TiXmlElement* pRoot, LPCTSTR szNodePath, LPCTSTR newNodeName,
		LPCTSTR text = NULL);

	// Summary:   
	// Time:	  2019年11月4日 peihaodong
	// Explain:	  
	bool AddNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath, LPCTSTR newNodeName,
		LPCTSTR text = NULL);

	bool FindNodeByTextPath(TiXmlElement* pRoot, LPCTSTR szNodePath,
		TiXmlElement*& pFindNode) const;

#pragma endregion

	// Summary:   获取声明信息
	// Time:	  2019年10月25日
	// Explain:	  
	bool GetDeclaration(CString& strVersion, CString& strEncoding, CString& strStandalone) const;

	// Summary:   得到根节点名
	// Time:	  2019年10月25日
	// Explain:	  
	CString GetRootNodeName() const;

	// Summary:   得到根节点文本
	// Time:	  2019年10月25日
	// Explain:	  
	CString GetRootNodeText() const;

	// Summary:   根据节点名，判断节点是否存在，并返回该节点指针
	// Time:	  2019年10月25日
	// Explain:	  
	bool FindNode(LPCTSTR nodeName, TiXmlElement*& pNode) const;

	// Summary:   获取节点文本
	// Time:	  2019年10月25日
	// Explain:	  
	bool GetNodeText(LPCTSTR nodeName, CString& text) const;

	// Summary:   获取节点属性
	// Time:	  2019年10月25日
	// Explain:	  
	bool GetNodeAttribute(LPCTSTR nodeName, std::map<CString, CString>& mapAttribute) const;

	// Summary:   删除节点
	// Time:	  2019年10月25日
	// Explain:	  
	bool DeleteNode(LPCTSTR nodeName) const;

	// Summary:   修改节点文本
	// Time:	  2019年10月25日
	// Explain:	  
	bool ModifyNodeText(LPCTSTR nodeName, LPCTSTR text) const;

	// Summary:   修改节点属性
	// Time:	  2019年10月25日
	// Explain:	  
	bool ModifyNodeAttribution(LPCTSTR nodeName,
		const std::map<CString, CString> &mapAttribute) const;
	bool ModifyNodeAttribution(TiXmlElement* pNode,
		const std::map<CString, CString> &mapAttribute) const;

	// Summary:   添加某一节点的子节点
	// Time:	  2019年10月25日
	// Explain:	  
	bool AddNode(LPCTSTR nodeName, LPCTSTR newNodeName, LPCTSTR text = NULL) const;

private:
	bool FindNode(TiXmlElement *pRoot, LPCTSTR nodeName, TiXmlElement*& pNode) const;


private:
	CString m_strXmlPath;

	TiXmlDocument* m_pDocument;			//文档
	TiXmlDeclaration* m_pDeclaration;	//声明
	TiXmlElement* m_pElementRoot;		//根节点元素
};

}