//TTList<Template> - Производное от TList, для создания списков любых объектов.
//TMStream - Производное от TMemoryStream имеет Put(int), Put(char)...
//TFStream - тоже, что и TMStream, но с файлами.
#include<vcl.h>
#include<jpeg.hpp>
#include<math.h>
#include <tlhelp32.h>
#include <wincrypt.h>
#include <winsock2.h>



class CToken
{
char*str;
bool Owner;//может ли класс удалить str после завершения или нет
public:
int Position;
char Separator;
CToken(char*text,bool Own)
     {
     Separator=' ';
     Position=0;
     str=NULL;
     Init(text,Own);
     }
~CToken()
     {
     if(str!=NULL)
          {
          if(Owner==false)
               delete [] str;
          }
     }
void Init(char*text,bool Own)//true - программа делает со строкой что хочет.
                    //false программа копирует text, и редактирует уже свою строку.
     {
     if(str!=NULL)
          {
          if(Owner==false)
               delete [] str;
          }
     if(Own==true)
          {
          Owner=true;
          str=text;
          }else
          {
          Owner=false;
          int len=strlen(text);
          str=new char[len+1];
          strcpy(str,text);
          }
     Position=0;
     }
char*NextToken()
     {
     char *result=str+Position;
     int simvcount=0;
     while(true)
          {
          if(str[Position]==Separator)
               {
               str[Position]=0;
               Position++;
               break;
               }
          if(str[Position]==0)
               {
               if(simvcount==0)
                    {
                    result=NULL;
                    }
                    break;
               }
          Position++;
          simvcount++;
          }
     return result;
     }
};
class CWToken
{
wchar_t*str;
bool Owner;//может ли класс удалить str после завершения или нет
public:
int Position;
wchar_t Separator;
CWToken(wchar_t*text,bool Own)
     {
     Separator=' ';
     Position=0;
     str=NULL;
     Init(text,Own);
     }
~CWToken()
     {
     if(str!=NULL)
          {
          if(Owner==false)
               delete [] str;
          }
     }
void Init(wchar_t*text,bool Own)//true - программа делает со строкой что хочет.
                    //false программа копирует text, и редактирует уже свою строку.
     {
     if(str!=NULL)
          {
        //  if(Owner==false)
         //      delete [] str;
          }
     if(Own==true)
          {
          Owner=true;
          str=text;
          }else
          {
          Owner=false;
          int len=wcslen(text);
          str=new wchar_t[len+1];
          wcscpy(str,text);
          }
     Position=0;
     }
wchar_t*NextToken()
     {
     wchar_t *result=str+Position;
     int simvcount=0;
     while(true)
          {
          if(str[Position]==Separator)
               {
               str[Position]=0;
               Position++;
               break;
               }
          if(str[Position]==0)
               {
               if(simvcount==0)
                    {
                    result=NULL;
                    }
                    break;
               }
          Position++;
          simvcount++;
          }
     return result;
     }
wchar_t*ToEnd()
     {
     return str+Position;

     }
};






class IPInformation//Недоделанный(как и в принципе все остальные классы)класс по получению своего IP 
//сделано классом, так как хотел добавить других функций, но потом не добавил
//Один метод String LocalIP()-получает сроку содержащую ИП компьютера
{
public:
static String LocalIP()
        {//А это все можно очень легко найти в нете(кусок был просто откуда-то скопирован, откуда не помню)
        AnsiString out = "WinSock ERR";
        WSADATA wsaData;
        if (!WSAStartup(WINSOCK_VERSION, &wsaData)){char chInfo[64];
        if (!gethostname(chInfo, sizeof(chInfo)))
        {
        hostent *sh;
        sh=gethostbyname((char*)&chInfo);
        if (sh!=NULL)
        {
        int nAdapter = 0;
        while (sh->h_addr_list[nAdapter])
        {
        struct sockaddr_in adr;
        memcpy(&adr.sin_addr, sh->h_addr_list[nAdapter], sh->h_length);
        out = inet_ntoa(adr.sin_addr);
        nAdapter++;
        }
        }
        }
        }
        WSACleanup();
        return out;
        }
};


//Класс для кодирования информации методом RSA с использованием Виндовсовского провайдера шифрования информации
//Коструктор 
  //RSACryptor(char*parol,int len) parol-набор байтов, который будет использоваться как пароль для шшифровки/расшифровки
//len-длина пароля. пароль может содержать \0 
  //Password(char*parol,int len)-ьоже самое что т в конструкторе
  //Crypt(bool Last,char*buffer,ULONG*count,ULONG buflen)
//Last-указываем, последний ли это кусок информации(надо ли алгоритму привязываться к предыдущим кускам информации)
//buffer-шифруемые данные, резулютат будет тут же.
//count-длина шифруемоей информации
//buflen-длина буффера. Длина зашифрованного сообщения может быть больше исходного, надо быть уверенным, что мы не вылезем за пределы
  //Decrypt(bool Last,char*buffer,ULONG*count)
//Практически тоже, что и Crypt но только расшифровуем используя текущий пароль
//count -будет потом длиной расшифрованного сообщения, которое может быть меньше чем зашифрованное
//Но, вроде, для этого алгоритма и зашифрованные и исходные длины одинаковы
class RSACryptor
{
HCRYPTPROV hProv;
HCRYPTHASH hash;
HCRYPTKEY key;
public:
RSACryptor(char * parol,int len)
        {
        hProv=NULL;
        Password(parol,len);
        }
void Password(char*parol,int len)
        {
        if(hProv!=NULL)CryptReleaseContext(hProv,0);
        CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
        CryptCreateHash(hProv,CALG_SHA,0,0,&hash);

        CryptHashData(hash,parol,len,0);
        CryptDeriveKey(hProv,CALG_RC4,hash,0,&key);
        CryptDestroyHash(hash);
        }
~RSACryptor()
        {
        CryptReleaseContext(hProv,0);
        }
bool Crypt(bool Last,char*buffer,ULONG*count,ULONG buflen)
        {
        return CryptEncrypt(key, 0, true, 0, buffer,count,buflen);
        }
bool Decrypt(bool Last,char*buffer,ULONG*count)
        {
        return CryptDecrypt(key, 0, true, 0, buffer,count);
        }
};


//Просто отмеряем временные интервалы.
//Process()-возвращает время между вызовом этого Process и конструктором или предыдущим Process()
//Время в миллисекундах
class TCounter
{
int millseccol;
public:
TCounter()
        {
        millseccol=GetTickCount();
        }
int Process()
        {
        int tick=GetTickCount();
        int delta=tick-millseccol;
        millseccol=tick;
        return delta;
        }
};



//Реально, практически бесполезный класс как отображение файла на память
//Но я его только использовал, чтобы как нибудь связываться между приложением и DLL находящимися в других адресных пространствах
//Это при перехватах API.
//Использование:
//Global=new InterProcMemory("Name",NULL);//создаем глобальный объект с любым именем
//Memory=(memdata*)Global->Base;//Берем указатель на него. теперь, все, что будет записано в эту память,
//атоматически будет доступно для всех процессов, которые создадут такой же объект с таким же именем.
class InterProcMemory
{
HANDLE FMap;
HANDLE file;
int MaxSize;
public:
void * Base;
InterProcMemory()
        {
        FMap=NULL;
        file=NULL;
        MaxSize=10240;
        Base=NULL;
        }
InterProcMemory(String MappingName,String FileName)
        {
        FMap=NULL;
        file=NULL;
        MaxSize=10240;
        Base=NULL;
        Create(MappingName,FileName);
        }
~InterProcMemory()
{
if(Base!=NULL)UnmapViewOfFile(Base);
if(FMap!=NULL)CloseHandle(FMap);
if(file!=NULL)CloseHandle(file);
}
void Create(String MappingName,String FileName)
        {
        file=INVALID_HANDLE_VALUE;
        if(FileName!="" && FileName!="0")
                {
                file=CreateFile(FileName.c_str(),GENERIC_READ|GENERIC_WRITE,
                                FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,NULL);
                if(file==INVALID_HANDLE_VALUE)throw "Error FileMapping. File not found";
                }

        FMap=CreateFileMapping(file,NULL,PAGE_READWRITE,0,MaxSize,MappingName.c_str());
        Base=MapViewOfFile(FMap,FILE_MAP_ALL_ACCESS,0,0,0);
        if(Base==NULL)throw "Can't Mapp the file";
        }
};




////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//TProcessList - производное от TStringList, показывает список процессов.(типа как диспетчер задач)
//Update()-обновляет список процессов в списке
//Strings[int index]-названия процессов
//GetProcessName(int index)-Название процесса по индексу
//DWORD GetProcessID(int index)-PID процесса по индексу
class TProcessList:public TStringList
{
public:
__fastcall TProcessList():TStringList()
{
Update();
}
String GetProcessName(int index)
{
return Strings[index];
}
DWORD GetProcessID(int index)
{
return (DWORD)Objects[index];
}
void Update()
{
Clear();
HANDLE PHandle;
PROCESSENTRY32 ProcEntry;
BOOL Result;
Result=false;
PHandle = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS,0);
ProcEntry.dwSize = sizeof(PROCESSENTRY32);
Process32First(PHandle, &ProcEntry);
        do
        {
        AddObject(ExtractFileName(ProcEntry.szExeFile),(TObject*)ProcEntry.th32ProcessID);
        }while(Process32Next(PHandle, &ProcEntry));
CloseHandle(PHandle);
}
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//TMStream - Производное от TMemoryStream имеет Put(int), Put(char)...
//Put(и разные типы)-записть информации в поток
//GetInt()-чтение int из потока предварительно положенного туда с помощью Put(int)
//GetChar()-чтение char из потока предварительно положенного туда с помощью Put(char)
//...
class TMStream:public TMemoryStream
{
public:
void Put(int i);
void Put(char c);
void Put(float f);
void Put(double d);
void Put(String s);
void PutW(WideString&s);
int GetInt();
char GetChar();
float GetFloat();
double GetDouble();
String GetString();
WideString GetWString();
};
void TMStream::Put(int i)
{
Write(&i,sizeof(int));
}
void TMStream::Put(char i)
{
Write(&i,sizeof(char));
}
void TMStream::Put(float i)
{
Write(&i,sizeof(float));
}
void TMStream::Put(double i)
{
Write(&i,sizeof(double));
}
void TMStream::Put(String s)
{
int l=s.Length();
Write(&l,sizeof(int));
Write(s.c_str(),l);
}
void TMStream::PutW(WideString&s)
{
int l=s.Length();
Write(&l,sizeof(int));
Write(s.c_bstr(),l*sizeof(wchar_t));
}
WideString TMStream::GetWString()
{
int l=GetInt();
wchar_t*text=new wchar_t[l+1];
text[l]=0;
Read(text,l*sizeof(wchar_t));
WideString s=text;
delete [] text;
return s;
}
String TMStream::GetString()
{
int l=GetInt();
char*text=new char[l+1];
text[l]=0;
Read(text,l);
String s=text;
delete [] text;
return s;
}
int TMStream::GetInt()
{
int i;
Read(&i,sizeof(int));
return i;
}
char TMStream::GetChar()
{
char i;
Read(&i,sizeof(char));
return i;
}
float TMStream::GetFloat()
{
float i;
Read(&i,sizeof(float));
return i;
}
double TMStream::GetDouble()
{
double i;
Read(&i,sizeof(double));
return i;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//тоже что и TMStream только при работе с файлами. ДЛя дальнейшей справки смотрим в хелп Билдера для TFileStream
class TFStream:public TFileStream
{
public:
__fastcall TFStream(const AnsiString name,WORD mode);
void Put(int i);
void Put(char c);
void Put(float f);
void Put(double d);
void Put(String s);
void PutW(WideString&s);
int GetInt();
char GetChar();
float GetFloat();
double GetDouble();
String GetString();
WideString GetWString();
};
__fastcall TFStream::TFStream(const AnsiString name,WORD mode):TFileStream(name,mode)
{

}
void TFStream::Put(int i)
{
Write(&i,sizeof(int));
}
void TFStream::Put(char i)
{
Write(&i,sizeof(char));
}
void TFStream::Put(float i)
{
Write(&i,sizeof(float));
}
void TFStream::Put(double i)
{
Write(&i,sizeof(double));
}
void TFStream::Put(String s)
{
int l=s.Length();
Write(&l,sizeof(int));
Write(s.c_str(),l);
}
void TFStream::PutW(WideString&s)
{
int l=s.Length();
Write(&l,sizeof(int));
Write(s.c_bstr(),l*sizeof(wchar_t));
}
WideString TFStream::GetWString()
{
int l=GetInt();
wchar_t*text=new wchar_t[l+1];
text[l]=0;
Read(text,l*sizeof(wchar_t));
WideString s=text;
delete [] text;
return s;
}
int TFStream::GetInt()
{
int i;
Read(&i,sizeof(int));
return i;
}
char TFStream::GetChar()
{
char i;
Read(&i,sizeof(char));
return i;
}
float TFStream::GetFloat()
{
float i;
Read(&i,sizeof(float));
return i;
}
double TFStream::GetDouble()
{
double i;
Read(&i,sizeof(double));
return i;
}
String TFStream::GetString()
{
int l=GetInt();
char*text=new char[l+1];
text[l]=0;
Read(text,l);
String s=text;
delete [] text;
return s;
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////TFileList
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//TFileList - производное от TStringList, показывает список
//файлов и папок в данной дидиректории.
//Update(char*path)-получает информацию о папках и файлах в данной директории.
//TFileList(char* path);-Конструктор. Вызывает Update
//После вызова Update можно обращатся к остальным функциям. 
//Колличество эллементов - Count,это остается еще от TStringList
//GetPath-директория, какую мы сейчас рассматриваем
//GetExt(int index)-взять расширение файла по данному индексу в списке
//GetPureName(int index)-взять имя файла без расширения по данному индексу в списке(работает неправильно если в имени более одной точки)
//GetFullPath(int index)-взять имя файла с путем к нему по данному индексу в списке(работает неправильно если в имени более одной точки)
class TFileList:public TStringList
{
String Path;
public:
        TFileList(char* path);
   void Update(char*path);
 String GetPath();
 String GetPureName(int index);
 String GetExt(int index);
 String GetFullPath(int index);
 WIN32_FIND_DATA*GetFileType(int index);

};
String TFileList::GetPath()
{
return Path;
}
String TFileList::GetExt(int index)
{
if(index>=Count)return NULL;
return ExtractFileExt(Strings[index]);
}
String TFileList::GetPureName(int index)
{
if(index>=Count)return NULL;
String st=Strings[index];
for(int i=st.Length();i>0;i--)
        {
        if(st[i]=='.')
                {
                return st.SubString(0,i-1);
                }
        }
return st;
}
String TFileList::GetFullPath(int index)
{
if(index>=Count)return NULL;
String p=ExtractFilePath(Path);
return p+Strings[index];
}
WIN32_FIND_DATA*TFileList::GetFileType(int index)
{
return (WIN32_FIND_DATA*)Objects[index];
}

TFileList::TFileList(char* path):TStringList()
{
Path="";
Update(path);
}
void TFileList::Update(char*path)
{
Clear();
if(path==NULL)path=Path.c_str();else Path=path;
WIN32_FIND_DATA file;
HANDLE hfind=FindFirstFile(path,&file);
if(hfind!=INVALID_HANDLE_VALUE)
{
do
        {
        WIN32_FIND_DATA*temp=new WIN32_FIND_DATA;
        *temp=file;
        AddObject(file.cFileName,(TObject*)temp);
        }while(FindNextFile(hfind,&file));
FindClose(hfind);
}
}



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////TInetFile
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//TInetFile - производное от TStream закачивает файлы из интернета по протоколу HTTP FTP.
//int OpenFile(char*Url)-Открываем URL для чтения.
//Если нета нету, то результат = 1
//Неудается подключиться на URL, то результат = 2
//Все хорошо = 0
//После удачного подключения Count будет содержать длину "файла". У FTP файлов класс не определяет длину.
//Да и далеко не каждый HTTP URL даст свою длину, Редкая страничка вернет правильную длину.
//Seek -тут не работает
//Read(char*buffer,int count)-Читаем данные в буффер длиной в count.
//Результатом будет число реально прочитанных байтов. Надо помнить, что запросив, например ,10000 байт, можно получить
//всего 1000, или вообще 0, и это нормально. Зависит от соединения. 
//Когда длина контента не известна, нужно читать из потока, пока читается. Когда несколько раз подряд приходит 0,
//то теоретически, это конец потока.
//Write-использовать не нужно, ничего и не произойдет. Просто заглушка.




class TInetFile:public TStream
{
#define INET_FILE_FTP  1
#define INET_FILE_HTTP 2
protected:
HANDLE internet;
HANDLE hUrl;
int Protokol;
bool Error;
private:
int CheckFileSize();
public:

int Position;
int FSize;
int OpenFile(char*Url);
void CloseFile();
TInetFile();
virtual __fastcall ~TInetFile();
virtual int __fastcall Read(void *Buffer, int Count);
virtual int __fastcall Write(const void *Buffer, int Count);
};
void TInetFile::CloseFile()
{
if(hUrl!=NULL)InternetCloseHandle(hUrl);
if(internet!=NULL)InternetCloseHandle(internet);
Error=true;
hUrl=NULL;
internet=NULL;
}
int __fastcall TInetFile::Write(const void *Buffer, int Count)
{
return 0;
}
int __fastcall TInetFile::Read(void *Buffer, int Count)
{
if(Error==true)return 0;
ULONG bytes;
InternetReadFile(hUrl, Buffer, Count, &bytes);
if((Count!=0)&& (bytes==0))Error=true;
Position+=bytes;
return bytes;
}

int TInetFile::CheckFileSize()
{
if(Error==true)return -1;
char buffer[40];
unsigned long buflen=40;
unsigned long index=0;
if(Protokol==INET_FILE_FTP)
{
FSize=FtpGetFileSize(hUrl,&index);
}else   //Если это HTTP
{
if(HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH, buffer, &buflen, &index))
        {
        FSize=StrToInt(buffer);
        }else
        {
        return -1;
        }
}
return FSize;
}

__fastcall TInetFile::~TInetFile()
{
if(hUrl!=NULL)InternetCloseHandle(hUrl);
if(internet!=NULL)InternetCloseHandle(internet);
}
TInetFile::TInetFile()
{
Position=0;
Error=true;
Protokol=0;
internet=NULL;
hUrl=NULL;
}
int TInetFile::OpenFile(char*Url)
{
if(hUrl!=NULL)InternetCloseHandle(hUrl);
if(internet!=NULL)InternetCloseHandle(internet);
internet=InternetOpen(Application->Title.c_str(),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
if(internet==(HANDLE)NULL)
        {
        return 1;
        }
String sU=Url;
sU=sU.UpperCase();
if(sU.Pos("FTP://")) //Это FTP файл
{
Protokol=INET_FILE_FTP;
}else                //Это HTTP файл
{
Protokol=INET_FILE_HTTP;
}
hUrl=InternetOpenUrl(internet,Url,NULL,0,INTERNET_FLAG_EXISTING_CONNECT,0);
if(hUrl==NULL)
        {
        InternetCloseHandle(internet);
        return 2;
        }
Error=false;
CheckFileSize();
Position=0;
return 0;
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////TLIST
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//Тоже самое, что и TList,отличается тем, что вместо void* возвращаемых значений из списка, 
//возвращается тип определенный шаблоном.
template <class T>
class TTList:public TList
{
public:
T Get(int index)
        {
        return (T)TList::Items[index];
        }
void Add(T item)
        {
        TList::Add((void*)item);
        }
int IndexOf(T item)
        {
        return TList::IndexOf((void*)item);
        }
int Find(T Item)
        {
        for(int i=0;i<Count;i++)
                {
                if(T==TList::Items[index])return i;
                }
        }
void DeleteAll()
        {
        for(int i=0;i<Count;i++)
                {
                if(TList::Items[i]!=NULL)
                        {
                        delete TList::Items[i];
                        }
                }
        Clear();
        }
};

//SaveStringsToStream(),LoadStringsFromStream - это просто функции для того, чтобы избавиться от досаднейшей особенности
//TStringList при восстановлении из потока читать поток до самого конца, тем самым не давая прочесть то, что было записано 
//после него. Такие же глюки есть во многих классах(TJPEGImage например).
void SaveStringsToStream(TStream*stream,TStrings*str)
{
TMStream*tm=new TMStream();
tm->Position=0;
tm->Size=0;
tm->Put((int)0);//резервируем место для количества байт
str->SaveToStream(tm);
tm->Position=0;
int realsize=tm->Size-4;
tm->Put(realsize);
tm->Position=0;
stream->CopyFrom(tm,tm->Size);
delete tm;
}
void LoadStringsFromStream(TStream*stream,TStrings*str)
{
TMStream*tm=new TMStream();
tm->Size=0;
tm->Position=0;
int size;
stream->Read(&size,sizeof(int));
if(size!=0)
        {
        tm->CopyFrom(stream,size);
        tm->Position=0;
        str->LoadFromStream(tm);
        }else
        {
        str->Clear();
        }
delete tm;
}


//Улучшеный TRect
//Normalize-если кордината слева больше, чем справа, то меняем местами. Так же и верх и низ
//CheckPoint(int x,int y)-входит ли точка в прямоугольник
//OffsetRect(int x,int y)-Передвигаем прямогуольник, не изменяя его размеров
//SetWidth(int width)-Изменяем ширину
//SetHeight(int height)-Изменяем высоту
//FitProportional(LPRECT rect)-Очень полезная функция вписывания текущего прямоугольника в другой прямоугольник
//не изменяя соотношение его сторон (FitProportional)
//Rotate()-повернуть на 90 градусов
//Scale(float x,float y)-Изменить размеры в соответствии с коеффициентами
class ERect:public TRect
{
public:
ERect(int l, int t, int r, int b):TRect(l,t,r,b)
        {}
ERect()
        {
        left=0;right=0;top=0;bottom=0;
        }
ERect(TRect r)
        {
        left=r.left;right=r.right;top=r.top;bottom=r.bottom;
        }
void Normalize()
        {
        if(left>right)
                {
                int temp;
                temp=left;
                left=right;
                right=temp;
                }
        if(top>bottom)
                {
                int temp;
                temp=top;
                top=bottom;
                bottom=temp;
                }
        }
bool CheckPoint(int x,int y)
        {
        if(x>=left && x<right && y>=top && y<bottom)return true;
        return false;
        }
void OffsetRect(int x,int y)
        {
        left +=x;
        right+=x;

        top   +=y;
        bottom+=y;
        }
void SetWidth(int width)
        {
        int delta=right-left;
        right+=width-delta;
        }
void SetHeight(int height)
        {
        int delta=bottom-top;
        bottom+=height-delta;
        }
void FitProportional(LPRECT rect)
        {
        ERect Result;
        ERect r(rect->left,rect->top,rect->right,rect->bottom);
        r.Normalize();
        float w=Width();
        float h=Height();
        float cw=r.Width();
        float ch=r.Height();
        /*float resolution=(float)th/(float)tw;
        tw=(float)trh*resolution;
        if(tw<Width())
                {
                tw=Width();
                resolution=((float)tw/(float)th);
                th=(float)trw/resolution;
                SetWidth(trw);
                SetHeight(th);
                }else
                {
                tw=Width();
                resolution=((float)tw/(float)th);
                tw=(float)trh/resolution;
                SetWidth(tw);
                SetHeight(trh);
                }  */
        float xyaspect=w/h;
        if(w>h)
                {
                w=cw;
                h=cw/xyaspect;
                if(h>ch)
                        {
                        h=ch;
                        w=ch*xyaspect;
                        }
                }else
                {
                h=ch;
                w=ch*xyaspect;
                if(w>cw)
                        {
                        w=cw;
                        h=cw/xyaspect;
                        }
                }
        SetWidth(w);
        SetHeight(h);
        }
void Rotate()
        {
        int w=Width();
        int h=Height();
        SetHeight(w);
        SetWidth(h);
        }
void Scale(float x,float y)
        {
        SetWidth(Width()*x);
        SetHeight(Height()*y);
        }
};

//Немного расширеный клас Graphics::TBitmap. Загружает jpeg и bmp. По человечески масштабирует(не так как Canvas->CopyTo)
//и может установить режим поворота плоскости(в смысле, что все нарисованное на данном контексте будет рисоваться под углом)
class ExtImage:public TBitmap
{
char*myscanline;
int lasty;//Для сканлайна
public:
void Load(String path)
        {
        lasty=-1;
        String ext=ExtractFileExt(path).UpperCase();
        if(ext==".JPEG" || ext==".JPG")
                {
                TJPEGImage*im=new TJPEGImage();
                im->LoadFromFile(path);
                Assign(im);
                delete im;
                }else
                {
                LoadFromFile(path);
                }
        }
void DrawTo(HDC handle,int x,int y)
        {
        BitBlt(handle,x,y,Width,Height,Canvas->Handle,0,0,SRCCOPY);
        }
__inline unsigned char*GetPixels(int x,int y)
     {
     if(y!=lasty)
          {
          myscanline=(char*)ScanLine[y];
          lasty=y;
          }
     return &myscanline[x+x+x];
     }
__inline GetLuminosity(int x,int y)
     {
     unsigned char*c=(unsigned char*)ScanLine[y];
     c=&c[x+x+x];
     return (c[2]*77 + c[1]*151 + c[0]*28) >> 8;
     }
void StretchDrawTo(HDC handle,LPRECT rect,float Angle)
        {
        if(Angle==0)
                {
                StretchBlt(handle,rect->left,rect->top,rect->right-rect->left,rect->bottom-rect->top,
                   Canvas->Handle,0,0,Width,Height,SRCCOPY);
                }else
                {
                Angle=Angle*0.017453292;
                Angle=-Angle;
                XFORM Matrix;
                SetGraphicsMode(handle, GM_ADVANCED);
                Matrix.eM11 = 1;
                Matrix.eM12 = 0;
                Matrix.eM21 = 0;
                Matrix.eM22 = 1;
                Matrix.eDx = -rect->left;
                Matrix.eDy = -rect->top;
                SetWorldTransform(handle, &Matrix);
                Matrix.eM11 = cos(Angle);
                Matrix.eM12 = sin(Angle);
                Matrix.eM21 = -sin(Angle);
                Matrix.eM22 = cos(Angle);
                Matrix.eDx = 0;
                Matrix.eDy = 0;
                ModifyWorldTransform(handle, &Matrix, MWT_RIGHTMULTIPLY);
                Matrix.eM11 = 1;
                Matrix.eM12 = 0;
                Matrix.eM21 = 0;
                Matrix.eM22 = 1;
                Matrix.eDx = rect->left;
                Matrix.eDy = rect->top;
                ModifyWorldTransform(handle, &Matrix, MWT_RIGHTMULTIPLY);

                StretchBlt(handle,rect->left,rect->top,rect->right-rect->left,rect->bottom-rect->top,
                   Canvas->Handle,0,0,Width,Height,SRCCOPY);
                ModifyWorldTransform(handle, &Matrix, MWT_IDENTITY);
                }
        }
};
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

SetGraphicsMode(dc, GM_ADVANCED);
SetWorldTransform(dc, &xform);
}
void DrawOutlinedText(TCanvas* canvas,char*text,int x,int y,int count)
     {
     HDC dc=canvas->Handle;
     SetBkMode(dc,TRANSPARENT);
     SelectObject(dc,canvas->Pen->Handle);
     SelectObject(dc,canvas->Brush->Handle);
     BeginPath(dc);

     TextOut(dc, x, y, text, count);
     EndPath(dc);
     StrokeAndFillPath(dc);
     }

wchar_t * utf8_to_unicode(char *utf8_string)
{
	int err;
	wchar_t * res;
	int res_len = MultiByteToWideChar(
		CP_UTF8,			// Code page
		0,					// No flags
		utf8_string,		// Multibyte characters string
		-1,					// The string is NULL terminated
		NULL,				// No buffer yet, allocate it later
		0					// No buffer
		);
	if (res_len == 0) 
	{
		return NULL;
	}
	res = (wchar_t*)calloc(sizeof(wchar_t), res_len);
	if (res == NULL) 
	{
		return NULL;
	}
	err = MultiByteToWideChar(
		CP_UTF8,			// Code page
		0,					// No flags
		utf8_string,		// Multibyte characters string
		-1,					// The string is NULL terminated
		res,				// Output buffer
		res_len				// buffer size
		);
	if (err == 0)
	{
		free(res);
		return NULL;
	}
	return res;
}

char * unicode_to_1251(wchar_t *unicode_string)
{
	int err;
	char * res;
	int res_len = WideCharToMultiByte(
		1251,				// Code page
		0,					// Default replacement of illegal chars
		unicode_string,		// Multibyte characters string
		-1,					// Number of unicode chars is not known
		NULL,				// No buffer yet, allocate it later
		0,					// No buffer
		NULL,				// Use system default
		NULL				// We are not interested whether the default char was used
		);
	if (res_len == 0) 
	{
		return NULL;
	}
	res = (char*)calloc(sizeof(char), res_len);
	if (res == NULL) 
	{
		return NULL;
	}
	err = WideCharToMultiByte(
		1251,				// Code page
		0,					// Default replacement of illegal chars
		unicode_string,		// Multibyte characters string
		-1,					// Number of unicode chars is not known
		res,				// Output buffer
		res_len,			// buffer size
		NULL,				// Use system default
		NULL				// We are not interested whether the default char was used
		);
	if (err == 0)
	{
		free(res);
		return NULL;
	}
	return res;
}
int WStrToInt(WideString s)
     {
     wchar_t*str=s.c_bstr();
     int res=0;
     for(int i=0;str[i]!=0;i++)
          {
          if((str[i]>='0')&&(str[i]<='9'))
               {
               res*=10;
               res+=str[i]-'0';
               }else
               {
               if(str[i]!=' ')throw "Error in conversion";
               }
          }
     return res;
     }
WideString IntToWString(int i)
     {
     wchar_t massiv[20];
     int pos=1;
     massiv[19]=0;
     bool minus;
     if(i<0)
          {
          minus=true;
          i*=-1;
          }else
          {
          minus=false;
          }
     for(pos;i!=0;pos++)
          {
          massiv[19-pos]='0'+i%10;
          i=i/10;
          }
     if(minus)
          {
          massiv[19-pos]='-';
          }else
          {
          pos--;
          }
     return WideString(massiv+19-pos);
     }