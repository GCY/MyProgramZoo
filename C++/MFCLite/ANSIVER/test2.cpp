//------------------------------------------------------
// �ɦW�Gtest.cpp
// �γ~�G���� MFCLITE �ҲթҴ��Ѫ��T�j��¦�A��
//   1. �H COblist �e�ǦU�� shape objects�C
//      �M���X���ɮ� output.dat ���A���� persistence�C
//   2. �q�ɮ� output.dat ��Ū�X��ƨ���ܡA
//      ���� persistence �M dynamic creation�C
//   3. �I�s IsKindOf �ΧQ�� GetRuntimeClass ��X class
//      �W�١A�H�������� RTTI�C�W�z 1, 2 �����\��w
//      �������ҤF RTTI �����\�I
//------------------------------------------------------

#include <iostream>
#include "mfclite.h"   // MFC Lite Module
#include "shape.h"     // Shape Hierarchy

using namespace std;

// instead CObList's Serialize() invocation by << and >> operator

int main()
{
  // try persistence, write to file
  {
    CShape* pShape[8];

    // prepare raw data
    pShape[0] = new CEllipse(3.0, 3.0, 7.0, 21.0);
    pShape[1] = new CCircle(5.0, 5.0, 7.0);
    pShape[2] = new CTriangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    pShape[3] = new CRect(5.6, 6.8, 3.0, 9.0);
    pShape[4] = new CSquare(3.2, 4.3, 6.0);
    pShape[5] = new CStroke;
    CStroke* pStroke = dynamic_cast<CStroke*>(pShape[5]);
    assert(pStroke);
    pStroke->aDArray.Add(1);
    pStroke->aDArray.Add(2);
    pStroke->aDArray.Add(3);
    pStroke->aDArray.Add(4);
    pStroke->aDArray.Add(5);
    pStroke->aDArray.Add(6);
    pStroke->aDArray.Add(7);
    pShape[6] = new CTriangle(0.0, 0.0, 3.5, 0.0, 0.0, 5.3);
    pShape[7] = new CRect(9.9, 9.9, 4.8, 9.8);

    CObList* pMyList = new CObList;     // my container

    for (int i=0; i<8; i++)   // prepare container's data
        pMyList->AddTail(pShape[i]);

    // retrieval all data
    POSITION pos = pMyList->GetHeadPosition();
    while (pos != NULL) {
        CShape* pS = (CShape*)pMyList->GetNext(pos);
        pS->display();  // show it.
    }

    CFile* pFile = new CFile("output.dat", CFile::modeWrite);
    CArchive ar(pFile, CArchive::store);

    ar << pMyList;   // persistence, write to file

    ar.Close();
    pFile->Close();
  }

  // try persistence, read from file
  {
    CFile* pFile = new CFile("output.dat", CFile::modeRead);
    CArchive ar(pFile, CArchive::load);
    CObList* pMyNewList;      // another container
    ar >> pMyNewList;         // persistence, read from file
    ar.Close();
    pFile->Close();

    // retrieval all data
    POSITION pos = pMyNewList->GetHeadPosition();
    while (pos != NULL) {
        CShape* pS = (CShape*)pMyNewList->GetNext(pos);
        pS->display();
    }
  }
}
