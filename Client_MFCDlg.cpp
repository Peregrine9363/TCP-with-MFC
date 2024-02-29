#include "pch.h"
#include "framework.h"
#include "Client_MFC.h"
#include "Client_MFCDlg.h"
#include "afxdialogex.h"
#include <chrono>
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 타이머 식별자와 간격을 정의합니다. 여기서는 0.1초(100ms)로 설정합니다.
#define TIMER_ID 1
#define TIMER_INTERVAL 100 

// CClientMFCDlg 대화 상자 클래스 구현 시작
CClientMFCDlg::CClientMFCDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CLIENT_MFC_DIALOG, pParent) // 부모 윈도우와 함께 대화 상자를 초기화합니다.
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 애플리케이션 아이콘을 로드합니다.
}

void CClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX); // 데이터 교환 메커니즘을 위한 MFC 기본 구현을 호출합니다.
}

// 메시지 맵을 시작합니다. 이곳에서 메시지와 핸들러 함수를 연결합니다.
BEGIN_MESSAGE_MAP(CClientMFCDlg, CDialogEx)
    ON_WM_PAINT() // WM_PAINT 메시지에 대한 핸들러 함수 지정
    ON_WM_QUERYDRAGICON() // 최소화된 창의 아이콘 요청에 대한 핸들러 함수 지정
    ON_WM_TIMER() // 타이머 메시지에 대한 핸들러 함수 지정
    // 시작 및 정지 버튼 클릭 이벤트에 대한 핸들러 함수 연결
    ON_BN_CLICKED(IDC_CLIENT_START_BUTTON, &CClientMFCDlg::OnBnClickedStart)
    ON_BN_CLICKED(IDC_CLIENT_STOP_BUTTON, &CClientMFCDlg::OnBnClickedStop)
    // 서버 시작 및 정지 버튼 클릭 이벤트에 대한 핸들러 함수 연결
    ON_BN_CLICKED(IDC_SERVER_START_BUTTON, &CClientMFCDlg::OnBnClickedStartClient)
    ON_BN_CLICKED(IDC_SERVER_STOP_BUTTON, &CClientMFCDlg::OnBnClickedStopClient)
END_MESSAGE_MAP()

// 대화 상자가 초기화될 때 호출되는 함수입니다.
BOOL CClientMFCDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog(); // 기본 클래스의 초기화 함수를 호출합니다.

    // 소켓 라이브러리를 초기화합니다. 실패할 경우 오류 메시지를 표시합니다.
    if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    // 대화 상자의 제목을 설정합니다.
    SetWindowText(_T("Client"));

    // 대화 상자의 아이콘을 설정합니다.
    SetIcon(m_hIcon, TRUE);  // 큰 아이콘
    SetIcon(m_hIcon, FALSE); // 작은 아이콘

    return TRUE; // 초기화가 성공적으로 완료되었음을 나타냅니다.
}

// 대화 상자가 그려질 때 호출되는 함수입니다.
void CClientMFCDlg::OnPaint()
{
    if (IsIconic()) // 대화 상자가 최소화된 상태인 경우
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트를 생성합니다.

        // 최소화된 창의 배경을 지우는 메시지를 처리합니다.
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 아이콘을 클라이언트 영역의 가운데에 그립니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint(); // 최소화되지 않은 상태에서의 기본 그리기 작업을 수행합니다.
    }
}

// 최소화된 창의 아이콘을 드래그할 때 사용되는 커서를 제공합니다.
HCURSOR CClientMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 설정된 타이머 간격마다 호출되는 함수입니다. 여기서는 현재 시간을 표시합니다.
void CClientMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TIMER_ID) // 타이머 ID가 일치하는 경우에만 처리합니다.
    {
        using namespace std::chrono;

        // 현재 시간을 시스템 시계로부터 얻고, 밀리초 단위로 포맷합니다.
        auto now = system_clock::now();
        auto now_ms = time_point_cast<milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = duration_cast<milliseconds>(epoch);

        time_t tt = system_clock::to_time_t(now);
        tm t;
        localtime_s(&t, &tt);
        CString strTime;
        strTime.Format(L"%02d:%02d:%02d.%03lld", t.tm_hour, t.tm_min, t.tm_sec, value.count() % 1000);

        // 현재 시간을 대화 상자의 텍스트 컨트롤에 표시합니다.
        SetDlgItemText(IDC_CLIENT_TIME_DISPLAY, strTime);
    }

    CDialogEx::OnTimer(nIDEvent);
}

// 타이머를 시작하는 버튼 클릭 이벤트 핸들러입니다.
void CClientMFCDlg::OnBnClickedStart()
{
    // 타이머를 시작합니다. TIMER_ID와 간격(TIMER_INTERVAL)을 지정합니다.
    m_nTimerID = SetTimer(TIMER_ID, TIMER_INTERVAL, nullptr);
}

// 타이머를 중지하는 버튼 클릭 이벤트 핸들러입니다.
void CClientMFCDlg::OnBnClickedStop()
{
    // 타이머를 중지합니다.
    KillTimer(m_nTimerID);
}

// 서버에 연결을 시작하는 버튼 클릭 이벤트 핸들러입니다.
void CClientMFCDlg::OnBnClickedStartClient()
{
    // 클라이언트 소켓을 생성하고 서버에 연결을 시도합니다.
    if (m_ClientSocket.Create())
    {
        if (m_ClientSocket.Connect(_T("127.0.0.1"), 12345)) // 지정된 주소와 포트로 연결을 시도합니다.
        {
            AfxMessageBox(_T("서버로 연결을 요청하는 데 성공했습니다."));
            Sleep(1000); // 잠시 대기

            // 서버에 "Start" 신호를 보냅니다.
            int nStartSignal = 1;
            int iSendResult = m_ClientSocket.Send(&nStartSignal, sizeof(nStartSignal));
            if (iSendResult == SOCKET_ERROR)
            {
                AfxMessageBox(_T("서버로 신호를 보내는 데 실패했습니다."));
                m_ClientSocket.Close();
            }
            else
            {
                AfxMessageBox(_T("서버로 신호를 보내는 데 성공했습니다."));
            }
        }
        else
        {
            AfxMessageBox(_T("서버에 연결할 수 없습니다."));
            m_ClientSocket.Close();
        }
    }
}

// 서버와의 연결을 중지하는 버튼 클릭 이벤트 핸들러입니다.
void CClientMFCDlg::OnBnClickedStopClient()
{
    // 클라이언트 소켓을 닫아 서버와의 연결을 종료합니다.
    m_ClientSocket.Close();
}
