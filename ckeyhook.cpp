#include "ckeyhook.h"
#define WM_GLOBALKEYBOARD WM_USER+100

HWND glhPrevTarWnd;    //上次鼠标所指的窗口句柄
HWND glhDisplayWnd;    //显示目标窗口标题编辑框的句柄
//HINSTANCE glhInstance; //DLL实例句柄
//BOOL gbIsWinDown ;
//BOOL gbWinFlag ;
//extern BOOL gbFlag ;
HHOOK hHook;       //安装的鼠标勾子句柄
short int g_NumLockSt; //小键盘的状态，0=关闭；非0=打开

CKeyHook::CKeyHook(QObject *parent) : QObject(parent)
{
    g_NumLockSt=GetKeyState(VK_NUMLOCK);
}

CKeyHook::~CKeyHook()
{
    StopHook();
}


bool CKeyHook::StartHook(HWND hWnd)
{
    BOOL bResult = FALSE;
    //DWORD dThreadID = 0;
    //dThreadID = AfxGetThread()->m_nThreadID;
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL,KeyProc,::GetModuleHandle(NULL),0);
    if(hHook != NULL)
        bResult = TRUE;
    glhDisplayWnd = hWnd;    //设置显示目标窗口标题编辑框的句柄

    return bResult;
}

bool CKeyHook::StopHook()
{
    BOOL bResult=FALSE;
    if(hHook)
    {
        bResult= UnhookWindowsHookEx(hHook);
        if(bResult)
        {
            glhPrevTarWnd=NULL;
            glhDisplayWnd=NULL;//清变量
            hHook=NULL;
        }
    }
    return bResult;
}//HC_ACTION

LRESULT CKeyHook::KeyProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    BOOL bFlag = FALSE;
    if(nCode == HC_ACTION)
    {
        LPKBDLLHOOKSTRUCT kblp=(LPKBDLLHOOKSTRUCT)lparam;
        //PostThreadMessage(g_dwThreadID, WM_GLOBALKEYBOARD, pKHook->vkCode, pKHook->scanCode);
        SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD, kblp->vkCode, kblp->scanCode);
       if((kblp->flags)&0x80)//为1 released
        {
        //	TRACE("win up\n");
            //if(gbWinFlag)
            //{
                //TRACE("win\n");
                switch(kblp->vkCode)
                {
                case VK_NUMLOCK:
                    g_NumLockSt=GetKeyState(VK_NUMLOCK);
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+90,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_LWIN:
                case VK_RWIN:
                    break;
                case VK_NUMPAD0:
                    //SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+10,kblp->vkCode,(LPARAM)kblp->scanCode);
                    //bFlag = TRUE;
                    break;
                case VK_END:
                //case VK_NUMPAD1:
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+11,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_DOWN:
                //case VK_NUMPAD2:
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+12,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_NEXT:
                //case VK_NUMPAD3:
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+13,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_LEFT:
                //case VK_NUMPAD4:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+14,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_CLEAR:
                //case VK_NUMPAD5:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+15,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_RIGHT:
                //case VK_NUMPAD6:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+16,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_HOME:
                //case VK_NUMPAD7:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+17,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_UP:
                //case VK_NUMPAD8:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+18,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_PRIOR:
                //case VK_NUMPAD9:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+19,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_ADD:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+80,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
/*                case VK_SUBTRACT:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my -");
                    //bFlag = TRUE;
                    break;
                case VK_DECIMAL:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+30,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_DIVIDE:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my /");
                    bFlag = TRUE;
                    break;
                case VK_ADD:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my +");
                    bFlag = TRUE;
                    break;
                case VK_NUMLOCK:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my NumLok");
                    bFlag = TRUE;
                    break;
                case VK_SEPARATOR:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my SEPARATOR");
                    bFlag = TRUE;
                    break;*/
                default:
                    //bFlag = FALSE;
                    break;
                }
            //}
            //gbIsWinDown = FALSE;
            //gbWinFlag = FALSE;
        }
        else
        {//pressed
            //gbIsWinDown = TRUE;
            //gbWinFlag = TRUE;
            //QString str;
            //str.Format("%x",kblp->vkCode);
            //char * chr="abcdfg";
            //chr=(char*)str.GetBuffer(str.GetLength());
            //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)chr);//"my NumLok"
                switch(kblp->vkCode)
                {
                case VK_NUMLOCK:
                    g_NumLockSt=GetKeyState(VK_NUMLOCK);
                    bFlag = TRUE;
                    break;
                //case (DWORD)'M':
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"WIN+M");,sizeof(char)
                    //bFlag = TRUE;
                    //break;
                //case VK_NUMPAD0:
                    //SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD,kblp->vkCode,(LPARAM)kblp->scanCode);
                    //bFlag = TRUE;
                    //break;
                case VK_END:
                //case VK_NUMPAD1:
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+1,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_DOWN:
                //case VK_NUMPAD2:
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+2,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_NEXT:
                //case VK_NUMPAD3:
                    bFlag = TRUE;
                    PostMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+3,kblp->vkCode,(LPARAM)kblp->scanCode);
                    break;
                case VK_LEFT:
                //case VK_NUMPAD4:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+4,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_CLEAR:
                //case VK_NUMPAD5:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+5,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_RIGHT:
                //case VK_NUMPAD6:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+6,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_HOME:
                //case VK_NUMPAD7:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+7,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_UP:
                //case VK_NUMPAD8:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+8,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
                case VK_PRIOR:
                //case VK_NUMPAD9:
                    SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+9,kblp->vkCode,(LPARAM)kblp->scanCode);
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)chr);//"myNUM"
                    bFlag = TRUE;
                    break;
                case VK_ADD:
                    //SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+80,kblp->vkCode,(LPARAM)kblp->scanCode);
                    bFlag = TRUE;
                    break;
/*                case VK_SUBTRACT:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my -");
                    //bFlag = TRUE;
                    break;
                case VK_DECIMAL:
                    //SendMessage(glhDisplayWnd , WM_GLOBALKEYBOARD+20,kblp->vkCode,(LPARAM)kblp->scanCode);
                    //bFlag = TRUE;
                    break;
                case VK_DIVIDE:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my /");
                    bFlag = TRUE;
                    break;
                case VK_ADD:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my +");
                    bFlag = TRUE;
                    break;
                case VK_NUMLOCK:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)chr);//"my NumLok"
                    bFlag = TRUE;
                    break;
                case VK_SEPARATOR:
                    //SendMessage(glhDisplayWnd , WM_SETTEXT,0,(LPARAM)"my SEPARATOR");
                    bFlag = TRUE;
                    break;*/
                }
        }
    }
    if(bFlag)
        return 1;
    return CallNextHookEx(hHook,nCode,wparam,lparam); //继续传递消息
}

