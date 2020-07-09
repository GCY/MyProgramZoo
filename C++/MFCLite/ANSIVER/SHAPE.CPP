//------------------------------------------------------
// 檔名：shape.cpp
// 用途：實作以下 classes：
//   CShape, CSquare, CRect, CCircle, CEllipse, CTriangle
//------------------------------------------------------

#include <iostream>
#include "shape.h"

using namespace std;

   IMPLEMENT_DYNCREATE(CShape, CObject)
   IMPLEMENT_SERIAL(CSquare, CShape, 0)
   IMPLEMENT_SERIAL(CRect, CSquare, 0)
   IMPLEMENT_SERIAL(CCircle, CShape, 0)
   IMPLEMENT_SERIAL(CEllipse, CCircle, 0)
   IMPLEMENT_SERIAL(CTriangle, CShape, 0)
IMPLEMENT_SERIAL(CStroke, CShape, 0)

void CSquare::display()
{
   cout << "CSquare:";
   cout << " x=" << x << ", y=" << y;
   cout << ", height=" << height << endl;
}

void CRect::display()
{
   cout << "CRect:";
   cout << " x=" << x << ", y=" << y;
   cout << ", height=" << height
      << ", width=" << width << endl;
}

void CCircle::display()
{
   cout << "CCircle:";
   cout << " x=" << x << ", y=" << y
      << " ,r=" << r << endl;
}

void CEllipse::display()
{
   cout << "CEllipse:";
   cout << " x=" << x << ", y=" << y;
   cout << ", r=" << r << ", r2=" << r2 << endl;
}

void CTriangle::display()
{
   cout << "CTriangle:";
   cout << " x1=" << x1 << ", y1=" << y1;
   cout << " x2=" << x2 << ", y2=" << y2;
   cout << " x3=" << x3 << ", y3=" << y3 << endl;
}

void CStroke::display()
{
   cout << "CStroke:";

   CDWordArray* pDArray = (CDWordArray*)&(this->aDArray);
   for (int i = 0; i< pDArray->GetSize(); i++)
      cout << " " << (*pDArray)[i];
   cout << endl;
}

void CSquare::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << x << y << height;
   }
   else
   {
      ar >> x >> y >> height;
   }
}

void CRect::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << x << y << height << width;
   }
   else
   {
      ar >> x >> y >> height >> width;
   }
}

void CCircle::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << x << y << r;
   }
   else
   {
      ar >> x >> y >> r;
   }
}

void CEllipse::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << x << y << r << r2;
   }
   else
   {
      ar >> x >> y >> r >> r2;
   }
}

void CTriangle::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << x1 << y1 << x2 << y2 << x3 << y3;
   }
   else
   {
      ar >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
   }
}

void CStroke::Serialize(CArchive& ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      aDArray.Serialize(ar);
   }
   else
   {
      aDArray.Serialize(ar);
   }
}

