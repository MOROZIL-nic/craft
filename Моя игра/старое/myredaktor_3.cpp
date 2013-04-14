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
//        HDC pic;
        int tipe, nomber;
    public:
        void init (Coords _coords, Dimension _size);
        void Draw (HDC pic);
        void Tipe (int tipe);
};
void Block::init (Coords _coords, Dimension _size)
{
    coord = _coords;
    size = _size;
    nomber= rand()%5;
}
void Block::Tipe (int _tipe)
{
    tipe=_tipe;
}
void Block::Draw (HDC pic)
{
    BitBlt(SGWndDC, coord.x, coord.y, size.width, size.height, pic, nomber*size.width, tipe*size.height, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
class List
{
    private:
        Coords coords;
        Dimension size;
        HDC pic;
        HDC block_pic;
        Block block [24][32];
        int block_size;
    public:
        List(Coords _coords, Dimension _size, int _block_size);
        void Draw (int tipe);

};
List::List (Coords _coords, Dimension _size, int _block_size)
{
    coords = _coords;
    size = _size;
    block_size = _block_size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "List.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);

    HBITMAP block_picbmp = (HBITMAP)LoadImage(0, "res/Blocks.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    block_pic = CreateCompatibleDC(SGWndDC);
    SelectObject(block_pic, block_picbmp);

    for (int y=0; y<size.height/block_size; y++)
    {
        for (int x=0; x<size.width/block_size; x++)
        {
            block[y][x].init ({coords.x+x*block_size, coords.y+y*block_size}, {block_size, block_size});
        }
    }
}
void List::Draw (int tipe)
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
    if (sgGetMouseButtons ()==1)
    {
            int y = (sgGetMouseY ()-coords.y)/block_size;
            int x = (sgGetMouseX()-coords.x)/block_size;
            block[y][x].Tipe (tipe);
    }
    for (int y=0; y<size.height/block_size; y++)
    {
        for (int x=0; x<size.width/block_size; x++)
        {
            block[y][x].Draw (block_pic);
        }

    }

}
class Tipes
{
    private:
        //Coords mousecoords;
        Coords coords;
        int tipe;
        Dimension size;
        HDC pic;
        int block_size;
    public:
        Tipes (Coords _coords, Dimension _size, int _block_size);
        int tipenomber (Coords mousecoords);
        void Draw ();
};
Tipes::Tipes (Coords _coords, Dimension _size, int _block_size)
{
    coords = _coords;
    block_size = _block_size;
    size = _size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "res/Tipes.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);

}
int Tipes::tipenomber (Coords mousecoords)
{
    if (coords.y+block_size*2>=mousecoords.y && mousecoords.y>=coords.y+block_size)
    {
        if (coords.x+block_size*2>=mousecoords.x && mousecoords.x>=coords.x+block_size)
            tipe = 0;
        if (coords.x+block_size*4>=mousecoords.x && mousecoords.x>=coords.x+block_size*3)
            tipe = 1;
        if (coords.x+block_size*6>=mousecoords.x && mousecoords.x>=coords.x+block_size*5)
            tipe = 2;
        if (coords.x+block_size*8>=mousecoords.x && mousecoords.x>=coords.x+block_size*7)
            tipe = 3;
        if (coords.x+block_size*10>=mousecoords.x && mousecoords.x>=coords.x+ block_size*9)
            tipe = 4;
    }
    return tipe;
}
void Tipes::Draw ()
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
class Menu
{
    private:
        Coords coords;
        Dimension size;
        int block_size;
        HDC pic;

    public:
        Menu (Coords _coords, Dimension _size, int _block_size);
        void Draw ();
};
Menu::Menu (Coords _coords, Dimension _size, int _block_size)
{
    coords = _coords;
    block_size = _block_size;
    size = _size;
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "Menu/Menu.bmp", IMAGE_BITMAP, size.width, size.height, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);
}
void Menu::Draw ()
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width/3, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
int main ()
{
    sgCreateWindow (800, 600);
    Menu menu ({0,0}, {400, 300}, 25);
    List list ({200,150}, {400, 300}, 25);
    Tipes tipes ({200, 500}, {400, 300}, 25);
    int tipe=1;
    while (1>0)
    {
        tipes.Draw ();
        if (sgGetMouseButtons ()==1)
        {
            tipe = tipes.tipenomber ({sgGetMouseX (), sgGetMouseY ()});
        }
        list.Draw (tipe);
        menu.Draw ();
        Sleep (100);
    }
}
