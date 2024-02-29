#include "pch.h"
#include "framework.h"
#include "Server_MFC.h"
#include "Server_MFCDlg.h"
#include "afxdialogex.h"
#include "ServerSocket.h"
#include <chrono>
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 타이머 식별자와 간격을 정의합니다. 여기서는 100ms(0.1초) 간격으로 설정합니다.
#define TIMER_ID 1
#define TIMER_INTERVAL 100 

// CServerMFCDlg 대화 상자 클래스 구현 시작
CServerMFCDlg::CServerMFCDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SERVER_MFC_DIALOG, pParent) // 대화 상자 리소스 ID와 부모 윈도우를 지정합니다.
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 애플리케이션의 아이콘을 로드합니다.
}

void CServerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX); // 데이터 교환(DDX/DDV)을 위한 기본 구현을 호출합니다.
}

BEGIN_MESSAGE_MAP(CServerMFCDlg, CDialogEx)
    // 메시지 맵에서 Windows 메시지에 대응하는 핸들러 함수를 지정합니다.
    ON_WM_PAINT() // WM_PAINT 메시지 처리를 위한 핸들러 함수를 지정합니다.
    ON_WM_QUERYDRAGICON() // 최소화된 창의 아이콘을 요청하는 메시지 처리를 위한 핸들러 함수를 지정합니다.
    ON_WM_TIMER() // WM_TIMER 타이머 메시지 처리를 위한 핸들러 함수를 지정합니다.
    ON_BN_CLICKED(IDC_START_BUTTON, &CServerMFCDlg::OnBnClickedStartServer) // 시작 버튼 클릭 이벤트 처리를 위한 핸들러 함수를 지정합니다.
    ON_BN_CLICKED(IDC_STOP_BUTTON, &CServerMFCDlg::OnBnClickedStopServer) // 정지 버튼 클릭 이벤트 처리를 위한 핸들러 함수를 지정합니다.
    ON_STN_CLICKED(VS_VERSION_INFO, &CServerMFCDlg::OnStnClickedVersionInfo) // 버전 정보 클릭 이벤트 처리를 위한 핸들러 함수를 지정합니다.
    ON_STN_CLICKED(AFX_ID_PREVIEW_CLOSE, &CServerMFCDlg::OnStnClickedIdPreviewClose) // 프리뷰 닫기 클릭 이벤트 처리를 위한 핸들러 함수를 지정합니다.
END_MESSAGE_MAP()

// 대화 상자가 초기화될 때 호출되는 함수입니다.
BOOL CServerMFCDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog(); // 기본 클래스의 OnInitDialog 함수를 호출하여 기본 초기화를 수행합니다.

    // 소켓 라이브러리 초기화에 실패하면 경고 메시지를 표시하고 초기화를 중단합니다.
    if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    // 대화 상자의 제목을 "Server"로 설정합니다.
    SetWindowText(_T("Server")); 

    // 대화 상자의 아이콘을 설정합니다. 큰 아이콘과 작은 아이콘 모두를 설정할 수 있습니다.
    SetIcon(m_hIcon, TRUE); // 큰 아이콘
    SetIcon(m_hIcon, FALSE); // 작은 아이콘

    return TRUE; // TRUE를 반환하여 포커스를 기본 컨트롤로 설정하지 않음을 나타냅니다.
}

// 대화 상자의 WM_PAINT 이벤트를 처리합니다. 창이 다시 그려질 필요가 있을 때 호출됩니다.
void CServerMFCDlg::OnPaint()
{
    if (IsIconic()) // 대화 상자가 최소화된 상태인지 확인합니다.
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트를 생성합니다.

        // 최소화된 창의 배경을 지우는 메시지를 보냅니다.
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 최소화된 창에 아이콘을 그립니다. 아이콘을 창의 가운데에 위치시킵니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon); // 아이콘을 그립니다.
    }
    else
    {
        CDialogEx::OnPaint(); // 대화 상자가 최소화되지 않은 경우 기본 클래스의 OnPaint 함수를 호출합니다.
    }
}

// 대화 상자가 최소화된 상태에서 사용자가 아이콘을 드래그할 때 커서를 제공합니다.
HCURSOR CServerMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon); // 대화 상자의 아이콘을 반환합니다.
}

// WM_TIMER 이벤트를 처리합니다. 설정된 타이머 간격이 지날 때마다 호출됩니다.
void CServerMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TIMER_ID에 해당하는 타이머 이벤트인 경우 처리합니다.
    if (nIDEvent == TIMER_ID)
    {
        // 현재 시간을 가져와서 밀리초 단위로 포맷팅합니다.
        using namespace std::chrono;
        auto now = system_clock::now();
        auto now_ms = time_point_cast<milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = duration_cast<milliseconds>(epoch);
        time_t tt = system_clock::to_time_t(now);
        tm t;
        localtime_s(&t, &tt);
        CString strTime;
        strTime.Format(L"%02d:%02d:%02d.%03lld", t.tm_hour, t.tm_min, t.tm_sec, value.count() % 1000);
        
        // 포맷팅된 시간 문자열을 대화 상자의 정적 텍스트 컨트롤에 출력합니다.
        SetDlgItemText(IDC_TIME_DISPLAY, strTime);
    }

    CDialogEx::OnTimer(nIDEvent); // 기본 클래스의 타이머 이벤트 처리기를 호출합니다.
}

// "시작" 버튼 클릭 이벤트를 처리합니다.
void CServerMFCDlg::OnBnClickedStartServer()
{
    // 타이머를 설정하여 정해진 간격으로 OnTimer 함수가 호출되도록 합니다.
    m_nTimerID = SetTimer(TIMER_ID, TIMER_INTERVAL, nullptr);

    // 서버 소켓을 생성하고, 지정된 포트에서 클라이언트의 연결을 기다립니다.
    if (m_ServerSocket.Create(12345)) // 포트 번호 12345 사용
    {
        if (m_ServerSocket.Listen()) // 클라이언트의 연결 수신 대기
        {
            // 클라이언트 연결 수신 대기 상태가 설정되었음을 알립니다.
            SetDlgItemText(IDC_L_PORT, _T("클라이언트 연결을 수신 대기중입니다."));
            InitializeServerSocket(); // 서버 소켓 초기화
        }
        else
        {
            // Listen 호출이 실패한 경우 오류 메시지를 표시합니다.
            SetDlgItemText(IDC_L_PORT, _T("클라이언트 연결 수신 대기 설정에 실패했습니다."));
            AfxMessageBox(_T("클라이언트 연결 수신 대기 설정에 실패했습니다."), MB_ICONERROR);
        }
    }
    else
    {
        // 서버 소켓 생성에 실패한 경우 오류 메시지를 표시합니다.
        SetDlgItemText(IDC_L_PORT, _T("서버 소켓 생성에 실패했습니다."));
        AfxMessageBox(_T("서버 소켓 생성에 실패했습니다."), MB_ICONERROR);
    }
    UpdateWindow(); // 대화 상자의 UI를 갱신합니다.
}

// 서버 소켓 초기화 함수입니다. 이 함수는 서버 소켓과 대화 상자 간의 연결을 설정합니다.
void CServerMFCDlg::InitializeServerSocket()
{
    m_ServerSocket.m_pParentDialog = this; // 서버 소켓에 대화 상자의 this 포인터를 전달합니다.
}

// "정지" 버튼 클릭 이벤트를 처리합니다.
void CServerMFCDlg::OnBnClickedStopServer()
{
    KillTimer(m_nTimerID); // 설정된 타이머를 중지합니다.
    m_ServerSocket.Close(); // 서버 소켓을 닫습니다.
}

// 버전 정보 클릭 이벤트를 처리합니다. 구현이 필요한 경우 여기에 코드를 추가합니다.
void CServerMFCDlg::OnStnClickedVersionInfo()
{
    // 버전 정보 클릭 시 실행할 코드를 여기에 작성합니다.
}

// 프리뷰 닫기 클릭 이벤트를 처리합니다. 구현이 필요한 경우 여기에 코드를 추가합니다.
void CServerMFCDlg::OnStnClickedIdPreviewClose()
{
    // 프리뷰 닫기 클릭 시 실행할 코드를 여기에 작성합니다.
}
