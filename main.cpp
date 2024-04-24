#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <ctime>
#include <shobjidl.h> // shell object interface definition language
#include <map> // shell object interface definition language

using namespace std::literals::chrono_literals;

constexpr uint8_t MAXPATHLEN = (1 << 8) - 1; // The actual Path limit of the OS is 260 chars.

struct StateInfo
{
};
class BaseWindow
{
public:
    BaseWindow() : m_hwnd(NULL) {}
    ~BaseWindow() {}

    HWND Window() const { return m_hwnd; }

    static LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        PAINTSTRUCT ps = {};
        RECT windowRect;
        RECT boxRect;
        POINT cursor = { 0, 0 };
        int bkgrndcol = 5;
        constexpr const unsigned int squreSize = 32; // x-pixels.
        GetWindowRect(hwnd, &boxRect);
        int screenWidth = boxRect.right - boxRect.left;

        GetCursorPos(&cursor);
        /*cursor.x = cursor.x + boxRect.left;
        cursor.y = cursor.y + boxRect.top;*/

        cursor.x -= boxRect.left;
        cursor.y -= boxRect.top;

        boxRect.left   = cursor.x - squreSize;
        boxRect.top    = cursor.y - squreSize;
        
        boxRect.right  = cursor.x                         ;
        boxRect.bottom = cursor.y;

        

        switch (msg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
            } break;
            case WM_CREATE:
            {
            } break;

            case WM_KEYDOWN:
            {
                switch (wp)
                {
                    case VK_SPACE: RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE); break;
                    case VK_RETURN:
                    {
                        InvalidateRect(hwnd, &boxRect, TRUE);
                        RedrawWindow(hwnd, &windowRect, NULL, RDW_UPDATENOW | RDW_ERASENOW);
                    } break;
                }
            } break;

            case WM_PAINT:
            {
                HDC hdc = BeginPaint(hwnd, &ps);
                GetClientRect(hwnd, &windowRect);
                FillRect(hdc, &windowRect, (HBRUSH)(COLOR_WINDOW + bkgrndcol));
                FillRect(hdc, &boxRect, (HBRUSH)(COLOR_WINDOW + bkgrndcol+2));
                EndPaint(hwnd, &ps);

            } break;
            default:
            {
            } break;
            
        }
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = BaseWindow::HandleMessage;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();
        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(),
            lpWindowName, dwStyle,
            x, y, nWidth, nHeight,
            hWndParent, hMenu,
            GetModuleHandle(NULL),
            this
        );
        return (m_hwnd ? TRUE : FALSE);
    }

protected:
    virtual PCWSTR ClassName() const = 0;
    //virtual LRESULT HandleMessage(UINT uMsg, WPARAM wp, LPARAM lp) = 0;

    HWND m_hwnd;
};
class MainWindow : public BaseWindow
{
public:
    PCWSTR ClassName() const { return L"Sample Window Class"; }
    //LRESULT HandleMessage(UINT uMsg, WPARAM wp, LPARAM lp);

private:

};
class IDrawable
{
public:
    virtual void Draw() = 0;
};
class Shape : IDrawable
{

};

//Initialization -> bitbool = bfalse;
struct Bit {
    unsigned int BitBool : 1; // haha, i wont use whole 16bits, i can use 1 bit instead!
    enum BooleanKeyword {
        bfalse,
        btrue
    };
    Bit(BooleanKeyword em) : BitBool(em) { }
};

// I dont want to use Heap! >:| | P -> Pointer.
// In my case m_0arr is the array of chars -> string
// And m_1arr is the array of these ( (array of chars) -> string)
// and that array of strings i am calling "Parts"
struct PArray {
    int   m_116maxsize = 0; char* m_116arr = 0; // 1D|2D // 116 -> 't' -> temp buffer -> only 
    int   m_0maxsize   = 0,     m_1maxsize = 0; // 1D|2D
    char* m_0arr       = 0;  char** m_1arr = 0; // 1D|2D
    // void* because i use int and int8_t.
    PArray(void* maxsize) : m_1maxsize(0), m_1arr(0) {
        m_0maxsize = *(int*)maxsize;
        m_0arr = {}; m_1arr = {};
    }
    void LinkPtr(void** a, void* b) { a = &b; } //m_1arr = &m_0arr; 
};

/*
--Check-Ptr-String-Lenght-By-Repetition (ptr, parts to check)--
Im using this trick because when the index
of an array (made of pointers) is already at the end,
It will be reseted at the first index.
So i could approximate the lenght of the array.
But the more similar words are in the array,
the more i prefer to set an higher value of amtrep.
amtrep -> amountofRepetition -> repetition of parts, not chars.
*/
int8_t CkPtrstrLRep(char*& str, int8_t amtPartrep) // DEBUG: imagine it's 3. -> "Hello how are you doing?--Hello how are you doing?"
{
    // D:\Coding\Visual Studio\VisualStudioProjects\Training\Window006
    // if Text -> check every space-char.
    // if Path -> check every Slash symbol '\'. (NOW I AM MAKING THIS)

    Bit IsFile(Bit::bfalse); Bit IsPath(Bit::btrue);
    char seperator = (IsPath.BitBool == Bit::btrue) ? '\\' : ' ';
    constexpr int maxstringlen = 10;

    PArray stringparts( (void*)(&maxstringlen) );
    stringparts.m_1maxsize = amtPartrep; // maybe 3 is it too few
    stringparts.LinkPtr((void**)stringparts.m_1arr, (void*)&stringparts.m_0arr);

    int8_t stringParts   = 0;
    int8_t result        = 0;
    int8_t offsetedIndex = 0;
    //char arrBuf[MAXPATHLENGTH];

    for (int i = 0; i < MAXPATHLEN; i++) {

        char currChar = str[i + offsetedIndex];
        if (currChar == '\0')
            break;

        if (i == 0)
        {
            stringparts.m_116arr = &str[i];
        }
        else if (stringparts.m_116arr == nullptr)
        {
            stringparts.m_116arr = &str[i + offsetedIndex];
        }
        else if (currChar != seperator)
        {
            stringparts.m_116arr[i] = currChar; // on index 4 it shows 'U' but it is for real 'r'
        }
        else {

            // switch it
            // copy the first word, convert it in a sorted array 
            //stringparts.m_1arr[stringParts] = 0;
            for (int j = 0; j < maxstringlen; j++)
            {
                if (stringparts.m_116arr[j] != '\0')
                {
                    stringparts.m_0arr = &stringparts.m_116arr[j];
                }
            }
            std::string asdklj;
            for (int k = 0; k < maxstringlen*10; k++)
            {
                if (stringparts.m_0arr[k] != '\0')
                {
                    if (k == 0)
                    {
                        asdklj = stringparts.m_0arr;
                    }
                    else
                    {
                        asdklj += stringparts.m_0arr[k];
                    }
                }
            }
            stringparts.m_1arr[stringParts] = stringparts.m_0arr;
            ++stringParts;

            stringparts.m_116arr = nullptr;


        }
        offsetedIndex++;
        //if (stringParts > 1) // you need at least 2 Parts to compare each other
        //{
        //    //if (stringparts.m[i] == pp_partsstring.m_arr[i-1]) //WRONG not the chars, the parts! ( i even said that in the explaination of the method !) D:< | so comparing strings!
        //    //{
        //    //    //Its a repetition!
        //    //}
        //}

        //if (p_partschar.m_arr[i] == seperator)
        //    ++stringParts;

        //if (stringParts == amtPartrep)
        //    break;

        
    }

    return result;
}

char* SFileInPath(char* path, uint8_t szpath) // Search <the> file <name> in path.
{
    char* result = {};
    char crrchpath; // currentcharpath
    Bit  myBoolBit(Bit::bfalse);

    char    fileTypeName[8];
    uint8_t pointPos     = 0; // the value when you found the dot(so the file) in the path.
    uint8_t totalfileLen = 0;
    uint8_t fileTypeLen  = 0;
    
    // 1. Get dot in Path | 2. get len of filetype.
    for (int i = 0; i < MAXPATHLEN; i++) {
        crrchpath = path[i];
        if (crrchpath == '.' && myBoolBit.BitBool == Bit::bfalse) {
            Bit* address = &myBoolBit;
            myBoolBit.BitBool = Bit::btrue;
            pointPos = (pointPos == 0) ? i : pointPos; // it can be changed ones 
        }
        if (crrchpath == '\0' && i % 2 == 0 || i >= szpath) { // xy for some reason every second char is '\0'
            fileTypeLen = i - pointPos;
            totalfileLen = i;
            break;
        } 
    }


    // search back for the filename (after '\')
    const int szfilename = totalfileLen - fileTypeLen;
    std::vector<char> filenameTemp(szfilename); // i dont want to use Heap D:
    for (int i = pointPos; i > 0; i--) {
        crrchpath = path[i];
        if (crrchpath == '\\') {

            for (int i = 0; i < szfilename; i++) {
                crrchpath = path[i];
                filenameTemp.push_back(crrchpath);
            }
        }
    }

    // copy the data of the vector to result
    uint8_t i = 0;
    for (auto& o : filenameTemp) {
        result[i++] = o;
    }

    return result;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    char*   path   = {};
    uint8_t szpath = {};

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    IFileOpenDialog* pfileOpen;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&pfileOpen));
    if (SUCCEEDED(hr))
    {
        hr = pfileOpen->Show(NULL);
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pfileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszpathName;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszpathName);
                if (SUCCEEDED(hr))
                {
                    // if the first 3 values are the same -> delete these an mark as end of the string | an pointer array at the end of the index is setting next to the first again.
                    path = (char*)pszpathName;
                    int8_t ptrlen = CkPtrstrLRep(path, 3); 
                    MessageBox(NULL, pszpathName, L"Your selected path:", MB_OK | MB_ICONINFORMATION);
                    CoTaskMemFree(pszpathName);
                }
                pItem->Release(); // ULONG
            }
        }
        pfileOpen->Release();
    }
    CoUninitialize();


    WCHAR msgTxt[100] = {};
    char* filename = new char[szpath];
    filename = SFileInPath(path, szpath);

    wsprintf(msgTxt, L"Do you want to open ", filename);
    MessageBox(NULL, msgTxt, L"Info", MB_OK | MB_ICONINFORMATION);

    delete[] filename;
    return 0;







    // windows Stuff. | DEACTIVATED

    MainWindow mainWin;
    if (!mainWin.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 1;
    }

    ShowWindow(mainWin.Window(), nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
