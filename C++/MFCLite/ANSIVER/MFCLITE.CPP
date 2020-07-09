//------------------------------------------------------
// 檔名：mfclite.cpp
// 用途：實作以下 classes：
//   CRuntimeClass, CObject, 及其他純為展示用的 empty classes...
//   CFile, CArchive, CDWordArray, CObList
// 程式碼來源：修剪 MFC source 並略加更改而得。
// 修改部份：檔案之存取改用 ANSI C 標準函式
//           （MFC 原採用 Win32 API 函式）
//------------------------------------------------------

#include <stdio.h>      // fopen, fclose, fread, fwrite...
#include <memory.h>     // memset, memcpy
#include <string.h>     // strlen, strcmp
#include "mfclite.h"

   IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
   IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)
   IMPLEMENT_DYNAMIC(CWinApp, CWinThread)
   IMPLEMENT_DYNAMIC(CDocument, CCmdTarget)
   IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)
   IMPLEMENT_DYNAMIC(CView, CWnd)
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)

   IMPLEMENT_DYNAMIC(CFile, CObject)
   IMPLEMENT_SERIAL(CObList, CObject, 0)
IMPLEMENT_SERIAL(CDWordArray, CObject, 0)

   // static members initialization
   static char szCObject[] = "CObject";
   struct CRuntimeClass CObject::classCObject =
{ szCObject, sizeof(CObject), 0xFFFF, NULL, NULL, NULL };
static AFX_CLASSINIT _init_CObject(&CObject::classCObject);
CRuntimeClass* CRuntimeClass::pFirstClass = NULL;

AFX_CLASSINIT::AFX_CLASSINIT(CRuntimeClass* pNewClass)
{
   pNewClass->m_pNextClass = CRuntimeClass::pFirstClass;
   CRuntimeClass::pFirstClass = pNewClass;
}

//--- CRuntimeClass Implementation ---

CObject* CRuntimeClass::CreateObject()
{
   if (m_pfnCreateObject == NULL)
   {
      TRACE1("Error: class not DECLARE_DYNCREATE or \n"
	    "DECLARE_SERIAL : %s\n", m_lpszClassName);
      return NULL;
   }

   CObject* pObject = NULL;
   pObject = (*m_pfnCreateObject)();

   return pObject;
}

CRuntimeClass* CRuntimeClass::Load(CArchive& ar,
      UINT* pwSchemaNum)
   // loads a runtime class description
{
   WORD nLen;
   char szClassName[64];
   CRuntimeClass* pClass;

   ar >> nLen;
   ar.Read(szClassName, nLen*sizeof(char));
   szClassName[nLen] = '\0';

   for (pClass = pFirstClass;
	 pClass != NULL;
	 pClass = pClass->m_pNextClass)
   {
      if (::strcmp(szClassName, pClass->m_lpszClassName) == 0)
	 return pClass;
   }

   TRACE1("Error: Class not found: %s \n", szClassName);
   return NULL; // not found
}

void CRuntimeClass::Store(CArchive& ar) const
// stores a runtime class description
{
   WORD nLen = (WORD)::strlen(m_lpszClassName);
   ar << nLen;
   ar.Write(m_lpszClassName, nLen*sizeof(char));
}

//--- CObject Implementation ---

CRuntimeClass* CObject::GetRuntimeClass() const
{
   return &CObject::classCObject;
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
   CRuntimeClass* pClassThis = GetRuntimeClass();
   while (pClassThis != NULL)
   {
      if (pClassThis == pClass)
	 return TRUE;
      pClassThis = pClassThis->m_pBaseClass;
   }
   return FALSE;   // walked to the top, no match
}

//--- CFile Implementation ---

CFile::CFile(char* lpszFileName, UINT nOpenFlags)
{
   Open(lpszFileName, nOpenFlags);
}

UINT CFile::Read(void* lpBuf, UINT nCount)
{
   if (nCount == 0)
      return 0;   // avoid Win32 "null-read"

   assert(lpBuf != NULL);

   if (nCount != ::fread(lpBuf, 1, nCount, m_hFile))
      assert(FALSE);  // fread() fail

   return (UINT)nCount;
}

void CFile::Write(const void* lpBuf, UINT nCount)
{
   if (nCount == 0)
      return;     // avoid Win32 "null-write" option

   assert(lpBuf != NULL);

   DWORD nWritten = ::fwrite(lpBuf, 1, nCount, m_hFile);
   assert(nWritten == nCount);
}

CFile::~CFile()
{
   Close();
}

void CFile::Close()
{
   ::fclose(m_hFile);
   m_hFile = NULL;
}


BOOL CFile::Open(char* lpszFileName, UINT nOpenFlags)
{
   // attempt file creation
   switch (nOpenFlags)
   {
      case modeRead:
	 m_hFile = ::fopen(lpszFileName, "rt");
	 break;
      case modeWrite:
	 m_hFile = ::fopen(lpszFileName, "wt");
	 break;
      default:
	 assert(FALSE);  // invalid share mode
   }

   assert(m_hFile != NULL);
   return TRUE;
}

//--- CDwordArray Implementation ---

CDWordArray::CDWordArray()
{
   m_pData = NULL;
   m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CDWordArray::~CDWordArray()
{
   delete[] (BYTE*)m_pData;
}

void CDWordArray::SetSize(int nNewSize, int nGrowBy)
{
   assert(nNewSize >= 0);

   if (nGrowBy != -1)
      m_nGrowBy = nGrowBy;  // set new size

   if (nNewSize == 0)
   {
      // shrink to nothing
      delete[] (BYTE*)m_pData;
      m_pData = NULL;
      m_nSize = m_nMaxSize = 0;
   }
   else if (m_pData == NULL)
   {
      // create one with exact size
      m_pData = (DWORD*) new BYTE[nNewSize * sizeof(DWORD)];
      ::memset(m_pData, 0, nNewSize * sizeof(DWORD));
      m_nSize = m_nMaxSize = nNewSize;
   }
   else if (nNewSize <= m_nMaxSize)
   {
      // it fits
      if (nNewSize > m_nSize)
      {
	 // initialize the new elements
	 ::memset(&m_pData[m_nSize], 0,
	       (nNewSize-m_nSize) * sizeof(DWORD));
      }
      m_nSize = nNewSize;
   }
   else
   {
      // otherwise, grow array
      int nGrowBy = m_nGrowBy;
      if (nGrowBy == 0)
      {
	 // heuristically determine growth when nGrowBy == 0
	 //  (this avoids heap fragmentation in many situations)
	 nGrowBy = min(1024, max(4, m_nSize / 8));
      }
      int nNewMax;
      if (nNewSize < m_nMaxSize + nGrowBy)
	 nNewMax = m_nMaxSize + nGrowBy;  // granularity
      else
	 nNewMax = nNewSize;  // no slush

      assert(nNewMax >= m_nMaxSize);  // no wrap around
      DWORD* pNewData = (DWORD*) new BYTE[nNewMax * sizeof(DWORD)];

      // copy new data from old
      ::memcpy(pNewData, m_pData, m_nSize * sizeof(DWORD));

      // construct remaining elements
      assert(nNewSize > m_nSize);

      ::memset(&pNewData[m_nSize], 0,
	    (nNewSize-m_nSize) * sizeof(DWORD));

      // get rid of old stuff (note: no destructors called)
      delete[] (BYTE*)m_pData;
      m_pData = pNewData;
      m_nSize = nNewSize;
      m_nMaxSize = nNewMax;
   }
}

void CDWordArray::SetAtGrow(int nIndex, DWORD newElement)
{
   assert(nIndex >= 0);

   if (nIndex >= m_nSize)
      SetSize(nIndex+1);
   m_pData[nIndex] = newElement;
}

void CDWordArray::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar.WriteCount(m_nSize);
      ar.Write(m_pData, m_nSize * sizeof(DWORD));
   }
   else
   {
      DWORD nOldSize = ar.ReadCount();
      SetSize(nOldSize);
      ar.Read(m_pData, m_nSize * sizeof(DWORD));
   }
}

//---  CArchive Implementation ---

CArchive::CArchive(CFile* pFile, UINT nMode)
{
   // initialize members not dependent on allocated buffer
   m_nMode = nMode;
   m_pFile = pFile;
}

CArchive::~CArchive()
{
   if (m_pFile != NULL)
      Close();
}

void CArchive::Close()
{
   m_pFile = NULL;
}

UINT CArchive::Read(void* lpBuf, UINT nMax)
{
   if (nMax == 0)
      return 0;

   assert(lpBuf != NULL);
   assert(IsLoading());

   return m_pFile->Read(lpBuf, nMax);
}

void CArchive::Write(const void* lpBuf, UINT nMax)
{
   if (nMax == 0)
      return;

   assert(lpBuf != NULL);
   assert(IsStoring());

   m_pFile->Write(lpBuf, nMax);
}

void CArchive::WriteCount(DWORD dwCount)
{
   if (dwCount < 0xFFFF)
      *this << (WORD)dwCount;
   else
   {
      *this << (WORD)0xFFFF;
      *this << dwCount;
   }
}

DWORD CArchive::ReadCount()
{
   WORD wCount;
   *this >> wCount;
   if (wCount != 0xFFFF)
      return wCount;

   DWORD dwCount;
   *this >> dwCount;
   return dwCount;
}

void CArchive::WriteObject(const CObject* pOb)
{
   // write class of object first
   CRuntimeClass* pClassRef = pOb->GetRuntimeClass();
   WriteClass(pClassRef);

   // cause the object to serialize itself
   ((CObject*)pOb)->Serialize(*this);
}

CObject* CArchive::ReadObject(const CRuntimeClass* pClassRefRequested)
{
   // attempt to load next stream as CRuntimeClass
   CRuntimeClass* pClassRef = ReadClass(pClassRefRequested);

   // check to see if tag to already loaded object
   CObject* pOb;
   if (pClassRef == NULL)
   {
      TRACE1("bad class\n");
   }
   else
   {
      // allocate a new object based on the class acquired
      pOb = pClassRef->CreateObject();
      if (pOb == NULL)
	 TRACE1("Dynamic Creation Fail\n");

      pOb->Serialize(*this);
   }

   return pOb;
}

void CArchive::WriteClass(const CRuntimeClass* pClassRef)
{
   assert(pClassRef != NULL);
   assert(IsStoring());    // proper direction

   // store new class
   pClassRef->Store(*this);
}

CRuntimeClass* CArchive::ReadClass(const CRuntimeClass* pClassRefRequested)
{
   assert(IsLoading());    // proper direction

   if (pClassRefRequested != NULL &&
	 pClassRefRequested->m_wSchema == 0xFFFF)
   {
      TRACE1("Warning: Cannot call ReadClass/ReadObject for %hs.\n",
	    pClassRefRequested->m_lpszClassName);
   }

   CRuntimeClass* pClassRef;
   UINT nSchema;
   // new object follows a new class id
   if ((pClassRef = CRuntimeClass::Load(*this, &nSchema)) == NULL)
      TRACE1("bad class\n");

   // return the resulting CRuntimeClass*
   return pClassRef;
}

//--- CObList Implementation ---

CObList::CObList()
{
   m_nCount = 0;
   m_pNodeHead = m_pNodeTail = NULL;
}

CObList::~CObList()
{
   RemoveAll();
   assert(m_nCount == 0);
}

void CObList::RemoveAll()
{
   // destroy elements
   CNode* p = m_pNodeHead;
   while(p != NULL)
   {
      BYTE* bytes = (BYTE*) p;
      CNode* pNext = p->pNext;
      delete[] bytes;
      p = pNext;
   }

   m_nCount = 0;
   m_pNodeHead = m_pNodeTail = NULL;
}

POSITION CObList::AddTail(CObject* newElement)
{
   CNode* pNewNode = NewNode(m_pNodeTail, NULL);
   pNewNode->data = newElement;
   if (m_pNodeTail != NULL)
      m_pNodeTail->pNext = pNewNode;
   else
      m_pNodeHead = pNewNode;
   m_pNodeTail = pNewNode;
   return (POSITION) pNewNode;
}

   CObList::CNode*
CObList::NewNode(CObList::CNode* pPrev,
      CObList::CNode* pNext)
{
   CObList::CNode* pNode = (CNode*) new BYTE[sizeof(CNode)];
   pNode->pPrev = pPrev;
   pNode->pNext = pNext;
   m_nCount++;
   assert(m_nCount > 0);  // make sure we don't overflow

   pNode->data = 0; // start with zero
   return pNode;
}

void CObList::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar.WriteCount(m_nCount);
      for (CNode* pNode = m_pNodeHead;
	    pNode != NULL;
	    pNode = pNode->pNext)
      {
	 ar << pNode->data;
      }
   }
   else
   {
      DWORD nNewCount = ar.ReadCount();
      CObject* newData;
      while (nNewCount--)
      {
	 ar >> newData;
	 AddTail(newData);
      }
   }
}

