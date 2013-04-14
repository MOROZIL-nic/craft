#include <stdio.h>
#include "sg.h"
#include <cstdlib>
#include <ctime>

struct Dimension
{
    int width;
    int height;
};
struct Coords
{
    int x;
    int y;
};
struct Point
{
    int x;
    int y;
};
class Block
{
    private:
        Coords coord;
        Dimension size;
        HDC pic;
        int tipe, nomber;
    public:
        void init (Coords _coords, Dimension _size);
        void Draw (int _tipe);
};
void Block::init (Coords _coords, Dimension _size)
{
    coord = _coords;
    size = _size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "Blocks.bmp", IMAGE_BITMAP, 130, 600, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
    nomber= rand()%5;
}
void Block::Draw (int _tipe)
{
    BitBlt(SGWndDC, coord.x, coord.y, size.width, size.height, pic, nomber*26, _tipe*26, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
class List
{
    private:
        Coords coords;
        Dimension size;
        HDC pic;
        Block block [24][32];
    public:
        List(Coords _coords, Dimension _size);
        void Draw (int tipe);

};
List::List (Coords _coords, Dimension _size)
{
    coords = _coords;
    size = _size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "List.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
    for (int y=0; y<23; y++)
    {
        for (int x=0; x<31; x++)
        {
            block[y][x].init ({x*26, y*26}, {26, 26});
        }
    }
}
void List::Draw (int tipe)
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
    for (int y=0; y<23; y++)
    {
        for (int x=0; x<31; x++)
        {
            block[y][x].Draw (tipe);
        }
    }
}
class Tipes
{
    private:
        Coords mousecoords;
        Coords coords;
        int tipe;
        Dimension size;
        HDC pic;
    public:
        Tipes (Coords _coords, Dimension _size);
        int tipenomber ();
        void Draw ();
};
Tipes::Tipes (Coords _coords, Dimension _size)
{
    coords = _coords;
    size = _size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "Tipes.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);

}
int Tipes::tipenomber ()
{
    if (coords.y+26*2>=sgGetMouseY ()>=coords.y+26)
    {
        if (coords.x+26>=sgGetMouseX ()>=coords.x+26*2)
            return 0;
        if (coords.x+26*3>=sgGetMouseX ()>=coords.x+26*4)
            return 1;
        if (coords.x+26*5>=sgGetMouseX ()>=coords.x+26*6)
            return 2;
        if (coords.x+26*7>=sgGetMouseX ()>=coords.x+26*8)
            return 3;
        if (coords.x+26*9>=sgGetMouseX ()>=coords.x+ 26*10)
            return 5;
    }
}
void Tipes::Draw ()
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
int main ()
{
    sgCreateWindow (800, 600);
    List list({0,0}, {400, 300});
    Tipes tipes({0, 400}, {287, 479});
    int tipe=0;
    while (1>0)
    {
        tipes.Draw ();
        if (sgGetMouseButtons ()==1)
        {
            tipe = tipes.tipenomber ();
        }
        list.Draw (tipe);
        Sleep (100);
    }
}
//16:12
