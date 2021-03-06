#include "StructLib.h"
#include "transparent.h"

void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor)
{
    BITMAP     bm;
    COLORREF   cColor;
    HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
    HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
    HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
    POINT      ptSize;

    hdcTemp = CreateCompatibleDC(hdc);
    SelectObject(hdcTemp, hBitmap);   // �������� ������

    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
    ptSize.x = bm.bmWidth;            // �������� ������ �������
    ptSize.y = bm.bmHeight;           // �������� ������ �������
    DPtoLP(hdcTemp, &ptSize, 1);      // ������������ �� ���������
                                     // ���������� � ����������
                                     // �����

    // ������ ��������� DC ��� �������� ��������� ������.
    hdcBack   = CreateCompatibleDC(hdc);
    hdcObject = CreateCompatibleDC(hdc);
    hdcMem    = CreateCompatibleDC(hdc);
    hdcSave   = CreateCompatibleDC(hdc);

   // ������ ������ ��� ������� DC.

   // ����������� DC
    bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // ����������� DC
    bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

    bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
    bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // � ������ DC ������ ���� ������ ������ ������� ��� ��������
   // ��������.


   // ������������� ����� ��������.
    SetMapMode(hdcTemp, GetMapMode(hdc));

   // ��������� ������, ���������� � ��������� �������, ��� ���
   // �� ����� �������.
    BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // ������������� ������� ���� (� �������� DC) ��� ������,
   // ������� ����� �����������.
    cColor = SetBkColor(hdcTemp, cTransparentColor);

   // ������ ����� ��� ������� ���� ������ BitBlt �� ���������
   // ������� �� ����������� ������.
    BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // ������������� ������� ���� ��������� DC ������� �
   // ������������ ����.
    SetBkColor(hdcTemp, cColor);

   // ������ �������� �����.
    BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // �������� ��� �������� DC � ��������.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // ����������� ����� �� �� �����, ��� ����� ������� ������.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // ����������� ����� �� ���������� ������� �������.
    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR-�� ������ � ����� �� �������� DC.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // �������� �� �����.
    BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // �������� ������������ ������ ������� � ������, ���������� �
   // ��������� �������.
    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // ������� ������� �� ������.
    DeleteObject(SelectObject(hdcBack, bmBackOld));
    DeleteObject(SelectObject(hdcObject, bmObjectOld));
       DeleteObject(SelectObject(hdcMem, bmMemOld));
       DeleteObject(SelectObject(hdcSave, bmSaveOld));

       // ������� DC �� ������.
       DeleteDC(hdcMem);
       DeleteDC(hdcBack);
       DeleteDC(hdcObject);
       DeleteDC(hdcSave);
       DeleteDC(hdcTemp);
}
int main()
{
    sgCreateWindow (800, 600);
    HDC pic;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "Blocks.bmp", IMAGE_BITMAP, 400, 1000, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
    while (1>0)
    {
        BitBlt(SGWndDC, 0, 0, 800, 600, pic, 0, 0, SRCCOPY);
        DrawTransparentBitmap(SGWndDC, picbmp, 0, 0, 0x00FFFFFF);
    }

}
