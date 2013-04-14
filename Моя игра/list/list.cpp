#include <stdio.h>
#include "sg.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

struct Coords
{
    double x;
    double y;
};
struct Dimension
{
    double width;
    double height;
};

void Point (Coords coord, int r)
{
    Ellipse (SGWndDC, coord.x-r, coord.y-r, coord.x+r, coord.y+r);
}
void RotateDC(HANDLE dc,float angle)
{
    XFORM xform;

    angle = angle * 3.1416 / 180.0;

    xform.eDx = 0.0f;
    xform.eDy = 0.0f;
    xform.eM11 = cos(angle);
    xform.eM12 = sin(angle);
    xform.eM21 = -sin(angle);
    xform.eM22 = cos(angle);

    SetGraphicsMode(SGWndDC, GM_ADVANCED);
    SetWorldTransform(SGWndDC, &xform);
}
class List
{
    private:
        Coords t1, t2, t3, t4, m, a1, a2, a3;
        Dimension _size;
        HDC pic;
        double f, b;
        int r1, r2;
    public:
        void init(Coords t, Dimension size);
        void a (Coords m_coords);
        void Draw ();
};
void List::init (Coords t, Dimension size)
{
    _size=size;
    m= {t.x+2*size.width-1, t.y+size.height};
    t1 = {t.x + size.width, t.y};                       //  t-----1-----0           1--------0
    t2 = {t1.x, t1.y + size.height/2};                  //  |     |     |           |        |
    t3 = {t1.x, t1.y + size.height};                    //  |     2     |           2   m    |
    t4 = {t1.x + size.width, t.y + size.height};        //  |     |     |           |     a1 |
                                                        //  0-----3-----4           3--a3-a2-4
    r1 = size.width;
    r2 = sqrt (size.width*size.width + size.height*size.height);
    //
    //2*x*(t4.x - m.x) + 2*y*(t4.y - m.y) + m.x*m.x - t4.x*t4.x + m.y*m.y - t4.y*t4.y;
    //
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "Blocks.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);

}
void List::a (Coords m_coords)
{

        if (hypot(t3.y-m_coords.y, m_coords.x-t3.x) < r1 && sgGetMouseButtons ()==1)
        {
            m = m_coords;
        }
        a1 = {(m.x+t4.x)/2, (m.y+t4.y)/2};
        a2 = {a1.x, t4.y};
        a3 = {a1.x-(a2.y-a1.y)*(a2.y-a1.y)/(t4.x-a2.x), t4.y};
      //  f = (a2.y-a1.y)/(a2.x-a3.x);// f = tga
     //   b=(-f)*(a3.x-t3.x);
}
void List::Draw ()
{
    BitBlt(SGWndDC, t1.x, t1.y, _size.width, _size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол

    RotateDC(SGWndDC->Canvas->Handle, 45.0);

    BitBlt(SGWndDC, m.x, m.y, _size.width, _size.height, pic, 0, 0, SRCCOPY);


 //   sgSelectTool (SGWndDC, CreateSolidBrush (RGB (100, 0, 0)));
   // Rectangle (SGWndDC, t1.x, t1.y, t4.x, t4.y);


    Point (m, 5);

    MoveToEx (SGWndDC, a3.x, a3.y, 0);
    LineTo (SGWndDC, a1.x, a1.y);



}
int main ()
{
    sgCreateWindow (800, 600);
    List list;
    list.init ({260, 0}, {130, 600});
    while (true)
    {
        list.a ({sgGetMouseX (), sgGetMouseY ()});
        list.Draw ();
        Sleep (100);
    }
}

