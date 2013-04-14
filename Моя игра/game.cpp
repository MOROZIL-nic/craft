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

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------

class List
{
    private:
        Coords coords;
        Dimension size;
        HDC pic;
        HDC block_pic;
        Block block [24][32];
        int block_size;
        int tipe[100][100];
        FILE *file;
    public:
        List(Coords _coords, Dimension _size, int _block_size);
        void fScanf ();
        void Draw ();

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

void List::fScanf ()
{
    char fname[64];
    scanf ("%s", fname);
    file=fopen(fname, "r");
    for (int y=0; y<12; y++)
    {
        for (int x=0; x<16; x++)
        {
            fscanf (file, "%d", &tipe[y][x]);
        }
    }
    fclose(file);

}
void List::Draw ()
{
    BitBlt(SGWndDC, coords.x, coords.y, size.width, size.height, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
    for (int y=0; y<size.height/block_size; y++)
    {
        for (int x=0; x<size.width/block_size; x++)
        {
            block[y][x].Tipe (tipe[y][x]);
            block[y][x].Draw (block_pic);
        }

    }

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


int main ()
{
    //sgCreateWindow (800, 600);
    sgCreateFullscreenWindow ();
    List list ({0,0}, {800, 600}, 50);
    list.fScanf ();
    char str[20] = "It's my game!";
    while (1>0)
    {
        TextOut (SGWndDC, 850, 100, str, 13);
        ExtTextOut (SGWndDC, 850, 200, 0/*fuOptions*/, NULL/*lprc*/, str, 13, NULL/*lpDx*/);
                                            // lprc- отсечение текста и фон

        list.Draw ();
        Sleep (100);
    }
}
