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
