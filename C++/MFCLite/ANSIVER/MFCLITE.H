//------------------------------------------------------
// 檔名：mfclite.h
// 用途：模擬 MFC framework 的三大重要特質
//   1. Run Time Type Identification
//   2. Dynamic Creation
//   3. Persistence（Serialization）
// 本模組供應之 classes：
//   CRuntimeClass, CObject, CFile, CArchive, CDWordArray,
//   CObList，及其他純只為模擬 MFC 繼承架構之 empty classes.
// 本模組供應之 macros：
//   DECLARE_DYNAMIC,   IMPLEMENT_DYNAMIC
//   DECLARE_DYNCREATE, IMPLEMENT_DYNCREATE
//   DECLARE_SERIAL,    IMPLEMENT_SERIAL
// 程式碼來源：修剪 MFC source 並略加更改而得。
// 修改部份：檔案之存取改用 ANSI C 標準函式
//           （MFC 原採用 Win32 API 函式）
//------------------------------------------------------

#ifndef _MFCLITE_
#define _MFCLITE_

#include <cassert>
#include <assert.h>   // for assert()
#include <stdio.h>    // for file stream's FILE*

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

#define TRACE1 printf
#define UINT   unsigned int
#define BOOL   int
#define DWORD  unsigned long
#define WORD   unsigned short
#define BYTE   unsigned char
#define LONG   long
#define TRUE   1
#define FALSE  0
#define HFILE  FILE*

// abstract iteration position
struct __POSITION { };
typedef __POSITION* POSITION;

// forward class declaration
class CArchive;
class CObject;

struct CRuntimeClass
{
   char* m_lpszClassName;
   int m_nObjectSize;
   UINT m_wSchema; // schema number of the loaded class
   CObject* (* m_pfnCreateObject)();
   CRuntimeClass* m_pBaseClass;

   // CRuntimeClass objects linked together in simple list
   CRuntimeClass* m_pNextClass;       // next node
   static CRuntimeClass* pFirstClass; // start node

   // for Dynamic Creation
   CObject* CreateObject();

   // for Persistence
   void Store(CArchive& ar) const;
   static CRuntimeClass* Load(CArchive& ar, UINT* pwSchemaNum);
};

struct AFX_CLASSINIT
{ AFX_CLASSINIT(CRuntimeClass* pNewClass); };

#define RUNTIME_CLASS(class_name) \
   (&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name) \
   public: \
   static CRuntimeClass class##class_name; \
   virtual CRuntimeClass* GetRuntimeClass() const;

#define DECLARE_DYNCREATE(class_name) \
   DECLARE_DYNAMIC(class_name) \
   static CObject* CreateObject();

#define DECLARE_SERIAL(class_name) \
   DECLARE_DYNCREATE(class_name) \
   friend CArchive& operator>>(CArchive& ar, class_name* &pOb);

#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
   static char _lpsz##class_name[] = #class_name; \
   CRuntimeClass class_name::class##class_name = { \
      _lpsz##class_name, sizeof(class_name), wSchema, pfnNew, \
      RUNTIME_CLASS(base_class_name), NULL }; \
      static AFX_CLASSINIT _init_##class_name(&class_name::class##class_name); \
      CRuntimeClass* class_name::GetRuntimeClass() const \
{ return &class_name::class##class_name; } \

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
   _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
   CObject* class_name::CreateObject() \
{ return new class_name; } \
_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
      class_name::CreateObject)

#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
   CObject* class_name::CreateObject() \
{ return new class_name; } \
_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, \
      class_name::CreateObject) \
      CArchive& operator>>(CArchive& ar, class_name* &pOb) \
{ pOb = (class_name*)ar.ReadObject(RUNTIME_CLASS(class_name)); return ar; }


//--- MFC Lite classes...

class CObject
{
   public:
      virtual CRuntimeClass* GetRuntimeClass() const;
      BOOL IsKindOf(const CRuntimeClass* pClass) const;
      virtual void Serialize(CArchive&) {  };

   public:
      static CRuntimeClass classCObject;
};

class CCmdTarget : public CObject
{
   DECLARE_DYNAMIC(CCmdTarget)
};

class CWinThread : public CCmdTarget
{
   DECLARE_DYNAMIC(CWinThread)
};

class CWnd;
class CWinApp : public CWinThread
{
   DECLARE_DYNAMIC(CWinApp)
};


class CDocument : public CCmdTarget
{
   DECLARE_DYNAMIC(CDocument)
};

class CWnd : public CCmdTarget
{
   DECLARE_DYNCREATE(CWnd)
};

class CFrameWnd : public CWnd
{
   DECLARE_DYNCREATE(CFrameWnd)
};

class CView : public CWnd
{
   DECLARE_DYNAMIC(CView)
};

class CFile : public CObject
{
   DECLARE_DYNAMIC(CFile)

   public:
      // Flag values
      enum OpenFlags {
	 modeRead =      0x0000,
	 modeWrite =     0x0001, };

      CFile(char* lpszFileName, UINT nOpenFlags);
      virtual BOOL Open(char* lpszFileName, UINT nOpenFlags);
      virtual void Close();
      virtual UINT Read(void* lpBuf, UINT nCount);
      virtual void Write(const void* lpBuf, UINT nCount);

      HFILE m_hFile;  // associated file

   public:
      virtual ~CFile();

   protected:
      char m_strFileName[40];   // file name
};

class CArchive
{
   public:
      // Flag values
      enum Mode { store = 0, load = 1 };

      CArchive(CFile* pFile, UINT nMode);
      ~CArchive();

      BOOL IsLoading() const;
      BOOL IsStoring() const;

      UINT Read(void* lpBuf, UINT nMax);
      void Write(const void* lpBuf, UINT nMax);
      void Close();

      // Object I/O is pointer based to avoid added construction overhead.
      // Use the Serialize member function directly for embedded objects.
      friend CArchive& operator<<(CArchive& ar, const CObject* pOb);
      friend CArchive& operator>>(CArchive& ar, CObject*& pOb);
      friend CArchive& operator>>(CArchive& ar, const CObject*& pOb);

      // insertion operations
      CArchive& operator<<(BYTE by);
      CArchive& operator<<(WORD w);
      CArchive& operator<<(LONG l);
      CArchive& operator<<(DWORD dw);
      CArchive& operator<<(float f);
      CArchive& operator<<(double d);

      CArchive& operator<<(int i);
      CArchive& operator<<(short w);
      CArchive& operator<<(char ch);
      CArchive& operator<<(unsigned u);

      // extraction operations
      CArchive& operator>>(BYTE& by);
      CArchive& operator>>(WORD& w);
      CArchive& operator>>(DWORD& dw);
      CArchive& operator>>(LONG& l);
      CArchive& operator>>(float& f);
      CArchive& operator>>(double& d);

      CArchive& operator>>(int& i);
      CArchive& operator>>(short& w);
      CArchive& operator>>(char& ch);
      CArchive& operator>>(unsigned& u);

      // object read/write
      CObject* ReadObject(const CRuntimeClass* pClass);
      void WriteObject(const CObject* pOb);
      void WriteClass(const CRuntimeClass* pClassRef);
      CRuntimeClass* ReadClass(const CRuntimeClass*
	    pClassRefRequested = NULL);
      DWORD ReadCount();
      void WriteCount(DWORD dwCount);

   protected:
      BOOL m_nMode;      // open mode
      CFile* m_pFile;    // associated CFile object
};

class CDWordArray : public CObject
{
   DECLARE_SERIAL(CDWordArray)

   public:
      CDWordArray();
      ~CDWordArray();

      void SetSize(int nNewSize, int nGrowBy = -1);
      int GetSize() const;

      DWORD GetAt(int nIndex) const;
      DWORD& ElementAt(int nIndex);

      // Potentially growing the array
      void SetAtGrow(int nIndex, DWORD newElement);
      int Add(DWORD newElement);

      // overloaded operator helpers
      DWORD operator[](int nIndex) const;
      DWORD& operator[](int nIndex);

      void Serialize(CArchive&);

   protected:
      DWORD* m_pData;  // the actual array of data
      int m_nSize;     // # of elements (upperBound - 1)
      int m_nMaxSize;  // max allocated
      int m_nGrowBy;   // grow amount
};

class CObList : public CObject
{
   DECLARE_SERIAL(CObList)

   protected:
      struct CNode
      {
	 CNode* pNext;
	 CNode* pPrev;
	 CObject* data;
      };

   public:
      CObList();
      ~CObList();

      // count of elements
      int GetCount() const;
      BOOL IsEmpty() const;

      POSITION AddTail(CObject* newElement);
      void RemoveAll();

      // iteration
      POSITION GetHeadPosition() const;
      CObject* GetNext(POSITION& rPosition) const; // return *Position++

      void Serialize(CArchive&);

   protected:
      CNode* NewNode(CNode*, CNode*);
      CNode* m_pNodeHead; CNode* m_pNodeTail; int m_nCount;
};
//--- inline function implementation
inline int CDWordArray::Add(DWORD newElement)
{ int nIndex = m_nSize;
   SetAtGrow(nIndex, newElement);
   return nIndex; }

   inline DWORD CDWordArray::GetAt(int nIndex) const
{ assert(nIndex >= 0 && nIndex < m_nSize);
   return m_pData[nIndex]; }

inline DWORD& CDWordArray::ElementAt(int nIndex)
{ assert(nIndex >= 0 && nIndex < m_nSize);
   return m_pData[nIndex]; }

   inline DWORD CDWordArray::operator[](int nIndex) const
{ return GetAt(nIndex); }

inline DWORD& CDWordArray::operator[](int nIndex)
{ return ElementAt(nIndex); }

inline int CDWordArray::GetSize() const
{ return m_nSize; }


inline BOOL CArchive::IsStoring() const
{ return (m_nMode & CArchive::load) == 0; }

inline BOOL CArchive::IsLoading() const
{ return (m_nMode & CArchive::load) != 0; }


inline CArchive& CArchive::operator<<(int i)
{ return CArchive::operator<<((LONG)i); }
inline CArchive& CArchive::operator<<(unsigned u)
{ return CArchive::operator<<((LONG)u); }
inline CArchive& CArchive::operator<<(short w)
{ return CArchive::operator<<((WORD)w); }
inline CArchive& CArchive::operator<<(char ch)
{ return CArchive::operator<<((BYTE)ch); }

inline CArchive& CArchive::operator<<(BYTE by)
{ m_pFile->Write(&by, sizeof(BYTE)); return *this; }
inline CArchive& CArchive::operator<<(WORD w)
{ m_pFile->Write(&w, sizeof(WORD)); return *this; }
inline CArchive& CArchive::operator<<(LONG l)
{ m_pFile->Write(&l, sizeof(LONG)); return *this; }
inline CArchive& CArchive::operator<<(DWORD dw)
{ m_pFile->Write(&dw, sizeof(DWORD)); return *this; }
inline CArchive& CArchive::operator<<(float f)
{ m_pFile->Write(&f, sizeof(float)); return *this; }
inline CArchive& CArchive::operator<<(double d)
{ m_pFile->Write(&d, sizeof(double)); return *this; }

inline CArchive& CArchive::operator>>(int& i)
{ return CArchive::operator>>((LONG&)i); }
inline CArchive& CArchive::operator>>(unsigned& u)
{ return CArchive::operator>>((LONG&)u); }
inline CArchive& CArchive::operator>>(short& w)
{ return CArchive::operator>>((WORD&)w); }
inline CArchive& CArchive::operator>>(char& ch)
{ return CArchive::operator>>((BYTE&)ch); }

inline CArchive& CArchive::operator>>(BYTE& by)
{ m_pFile->Read(&by, sizeof(BYTE)); return *this; }
inline CArchive& CArchive::operator>>(WORD& w)
{ m_pFile->Read(&w, sizeof(WORD)); return *this; }
inline CArchive& CArchive::operator>>(DWORD& dw)
{ m_pFile->Read(&dw, sizeof(DWORD)); return *this; }
inline CArchive& CArchive::operator>>(float& f)
{ m_pFile->Read(&f, sizeof(float)); return *this; }
inline CArchive& CArchive::operator>>(double& d)
{ m_pFile->Read(&d, sizeof(double)); return *this; }
inline CArchive& CArchive::operator>>(LONG& l)
{ m_pFile->Read(&l, sizeof(LONG)); return *this; }

inline CArchive& __stdcall operator<<(CArchive& ar, const CObject* pOb)
{ ar.WriteObject(pOb); return ar; }
inline CArchive& __stdcall operator>>(CArchive& ar, CObject*& pOb)
{ pOb = ar.ReadObject(NULL); return ar; }
inline CArchive& __stdcall operator>>(CArchive& ar, const CObject*& pOb)
{ pOb = ar.ReadObject(NULL); return ar; }


inline int CObList::GetCount() const
{ return m_nCount; }

inline BOOL CObList::IsEmpty() const
{ return m_nCount == 0; }


inline POSITION CObList::GetHeadPosition() const
{ return (POSITION) m_pNodeHead; }

inline CObject* CObList::GetNext(POSITION& rPosition) const // return *Position++
{ CNode* pNode = (CNode*) rPosition;
   rPosition = (POSITION) pNode->pNext;
   return pNode->data; }


#endif /* _MFCLITE_ */
