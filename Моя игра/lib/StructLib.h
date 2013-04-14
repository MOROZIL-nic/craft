#ifndef StructLib
#define StructLib

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
class Cell
{
    private:
        Coords coords;
        Dimension size;
        int n;
        HDC pic;
    public:
        void init (Coords _coords, Dimension _size, int _n);
        void draw ();
        int open ();
};
void Cell::init (Coords _coords, Dimension _size, int _n)
{
    coords = _coords;
    size = _size;
//    printf ("%d %d\n", coords.x, coords.y);
    n=_n;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "res/menu.bmp", IMAGE_BITMAP, 400, 1000, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
}
void Cell::draw ()
{
        BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 150*n, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
int Cell::open ()
{
    if (sgGetMouseButtons ()==1 && coords.x<sgGetMouseX() && sgGetMouseX()< coords.x+ size.width && coords.y<sgGetMouseY() && sgGetMouseY()< coords.y+ size.height)
    {
        return true;
    }
    else
    {
        return false;
    }
}
class Menu
{
    private:
        FILE *file;
        Coords coords, cell_coords;
        Dimension size, cell_size;
        Cell cell[5]; //New_Game, Map_Editor, Options, Authors, Exit
        HDC pic;
    public:
        Menu (Coords _coords, Dimension _size);
        int draw ();
        ~Menu ();

};
Menu::Menu (Coords _coords, Dimension _size)
{
    coords = _coords;
    size = _size;
    file=fopen("menu.txt", "r");
    for (int i=0; i<5; i++)
    {
        fscanf (file, "%d %d %d %d\n", &cell_coords.x, &cell_coords.y, &cell_size.width, &cell_size.height);
        cell[i].init (cell_coords, cell_size, i);
    }
    fclose(file);

    HBITMAP picbmp = (HBITMAP)LoadImage(0, "res/font.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
}
Menu::~Menu()
{

}
int Menu::draw ()
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
    for (int i=0; i<5; i++)
    {
        cell[i].draw ();
        bool x= cell[i].open();
        if (x==true)
            return (i+1);
    }
}
#endif
