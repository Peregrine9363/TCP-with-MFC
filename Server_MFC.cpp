#include "pch.h"
#include "framework.h"
#include "Server_MFC.h"
#include "Server_MFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerMFCApp 클래스는 CWinApp 클래스를 상속받아 애플리케이션의 메인 클래스로 동작합니다.
BEGIN_MESSAGE_MAP(CServerMFCApp, CWinApp)
    // 메시지 맵에서 ID_HELP 명령에 대한 처리를 CWinApp의 OnHelp 함수로 지정합니다.
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
    // IDC_START_BUTTON 버튼 클릭 시 CServerMFCApp의 OnBnClickedStartButton 함수를 호출합니다.
    ON_BN_CLICKED(IDC_START_BUTTON, &CServerMFCApp::OnBnClickedStartButton)
    // IDC_STOP_BUTTON 버튼 클릭 시 CServerMFCApp의 OnBnClickedStopButton 함수를 호출합니다.
    ON_BN_CLICKED(IDC_STOP_BUTTON, &CServerMFCApp::OnBnClickedStopButton)
END_MESSAGE_MAP()

// CServerMFCApp의 생성자입니다. 여기서는 멤버 변수의 초기화 등 초기 설정을 수행합니다.
CServerMFCApp::CServerMFCApp() 
{
    // 생성자에서의 초기화 코드는 여기에 작성합니다.
}

// 애플리케이션 인스턴스를 생성합니다.
CServerMFCApp theApp;

// 애플리케이션 초기화를 담당하는 InitInstance() 함수입니다.
BOOL CServerMFCApp::InitInstance() 
{
    // 상속받은 CWinApp의 InitInstance 함수를 호출하여 기본적인 애플리케이션 초기화를 수행합니다.
    CWinApp::InitInstance(); 

    // CShellManager를 사용하여 파일 및 폴더 선택 대화 상자 등의 셸 기능을 관리합니다.
    CShellManager* pShellManager = new CShellManager; 

    // 애플리케이션의 UI 테마와 스타일을 설정합니다.
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows)); 

    // 애플리케이션의 레지스트리 키를 설정합니다. 사용자 설정이나 상태 정보 저장에 사용됩니다.
    SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션")); 

    // 메인 대화 상자를 생성하고 모달 대화 상자로 표시합니다.
    CServerMFCDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // OK 버튼 클릭 시의 처리 로직을 여기에 작성합니다.
    }
    else if (nResponse == IDCANCEL)
    {
        // Cancel 버튼 클릭 시의 처리 로직을 여기에 작성합니다.
    }
    else if (nResponse == -1)
    {
        // 대화 상자 생성 실패 시의 처리 로직을 여기에 작성합니다.
        TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
        TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
    }

    // CShellManager 객체를 삭제합니다.
    if (pShellManager != nullptr)
    {
        delete pShellManager;
    }

    // MFC 컨트롤 바 정리를 수행합니다. 동적 연결 라이브러리와 관련이 없는 경우에만 필요합니다.
#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif
    return FALSE; // 애플리케이션의 메시지 루프를 시작하지 않음을 나타냅니다.
}

// 시작 버튼 클릭 시 호출되는 함수입니다.
void CServerMFCApp::OnBnClickedStartButton()
{
    // 시작 버튼 클릭 시 실행될 코드를 여기에 작성합니다.
}

// 정지 버튼 클릭 시 호출되는 함수입니다.
void CServerMFCApp::OnBnClickedStopButton()
{
    // 정지 버튼 클릭 시 실행될 코드를 여기에 작성합니다.
}
