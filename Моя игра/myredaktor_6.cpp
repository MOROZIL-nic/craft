#include "lib/StructLib.h"

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
        Block block [12][8];
       // Save save;
        int block_size;
        int tipe[100][100];
        FILE *file;

    public:
        List(Coords _coords, int _block_size);
        void OpenFile ();
        void Draw (int tipe);

};
List::List (Coords _coords, int _block_size)
{
    block_size = _block_size;
    coords = _coords;
    size = {8*block_size, 12*block_size};
    HBITMAP picbmp = (HBITMAP)LoadImage(0, "res/pus.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
    pic = CreateCompatibleDC(SGWndDC);
    SelectObject(pic, picbmp);

    HBITMAP block_picbmp = (HBITMAP)LoadImage(0, "res/Blocks3.bmp", IMAGE_BITMAP, 400, 1000, LR_LOADFROMFILE);
    block_pic = CreateCompatibleDC(SGWndDC);
    SelectObject(block_pic, block_picbmp);

    for (int y=0; y<12; y++)
    {
        for (int x=0; x<8; x++)
        {
            block[y][x].init ({coords.x+x*block_size, coords.y+y*block_size}, {block_size, block_size});
        }
    }
   //
    BitBlt(SGWndDC, 0, 0, 800, 600, pic, 0, 0, SRCCOPY);
                                            //2,3 - левый верхний угол картинки, 4,5-ширина и высота, 7,8- отображаемый левый верхний угол
}
void List::OpenFile ()
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
void List::Draw (int tipe)
{
    if (sgGetMouseButtons ()==1)
    {
            int y = (sgGetMouseY ()-coords.y)/block_size;
            int x = (sgGetMouseX()-coords.x)/block_size;
            block[y][x].Tipe (tipe);
        //    save.init ({x, y}, tipe);
    }
    if (sgGetMouseButtons ()==2)
    {
      //  save.fPrintf ();
    }
    for (int y=0; y<12; y++)
    {
        for (int x=0; x<8; x++)
        {
            block[y][x].Draw (block_pic);
        }

    }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------

int main ()
{
    sgCreateWindow (800, 600);
    Menu menu({0, 0}, {800, 600});
    while (menu.draw()==0)
    {
        menu.draw();
        Sleep (20);
    }
    if (menu.draw()==2)
    {
        menu.~Menu();
        List list ({0,0}, 50);
        list.OpenFile ();
        while (1>0)
        {
            list.Draw (2);
            Sleep (20);

        }
    }
}
