//------------------------------------------------------
// 檔名：shape.h
// 本模組供應之 classes（Shape hierarchy）：
//   CShape, CSquare, CRect, CCircle, CEllipse, CTriangle
// 注意：為使用「MFCLITE 三大特質」，必須含入 mfclite.h
//------------------------------------------------------

#ifndef _SHAPE_
#define _SHAPE_

#include "mfclite.h"  // MFC Lite module

class CShape : public CObject
{
   DECLARE_DYNCREATE(CShape)
   public:
      virtual void display() { }
      virtual void Serialize(CArchive&) { };
};

class CSquare : public CShape
{
   DECLARE_SERIAL(CSquare)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);
      CSquare(float xval=0.0, float yval=0.0, float hval=0.0)
      {
	 x = xval;
	 y = yval;
	 height = hval;
      }

   protected:
      float x, y;
      float height;
};

class CRect : public CSquare
{
   DECLARE_SERIAL(CRect)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);
      CRect(float xval=0.0, float yval=0.0,
	    float hval=0.0, float wval=0.0)
	 : CSquare(xval, yval, hval)
      {
	 width = wval;
      }

   protected:
      float width;
};

class CCircle : public CShape
{
   DECLARE_SERIAL(CCircle)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);
      CCircle(float xval=0.0, float yval=0.0, float rval=0.0)
      {
	 x = xval;
	 y = yval;
	 r = rval;
      }

   protected:
      float x, y;
      float r;
};

class CEllipse : public CCircle
{
   DECLARE_SERIAL(CEllipse)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);
      CEllipse(float xval=0.0, float yval=0.0,
	    float rval=0.0, float r2val=0.0)
	 : CCircle(xval, yval, rval)
      {
	 r2 = r2val;
      }

   protected:
      float r2;
};

class CTriangle : public CShape
{
   DECLARE_SERIAL(CTriangle)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);
      CTriangle(float x1val=0.0, float y1val=0.0,
	    float x2val=0.0, float y2val=0.0,
	    float x3val=0.0, float y3val=0.0)
      {
	 x1 = x1val;  y1 = y1val;
	 x2 = x2val;  y2 = y2val;
	 x3 = x3val;  y3 = y3val;
      }

   protected:
      float x1, y1, x2, y2, x3, y3;
};

class CStroke : public CShape
{
   DECLARE_SERIAL(CStroke)
   public:
      virtual void display();
      virtual void Serialize(CArchive&);

   public:
      CDWordArray aDArray;
};

#endif /* _SHAPE_ */
