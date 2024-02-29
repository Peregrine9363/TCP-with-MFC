#include "pch.h"
#include "ClientSocket.h"
#include "Client_MFCDlg.h" // 메인 대화 상자 헤더 파일을 포함하여 대화 상자와의 상호 작용이 가능하도록 합니다.
#include "Resource.h"

// CClientSocket의 기본 생성자입니다.
CClientSocket::CClientSocket() {}

// 데이터를 수신할 때 호출되는 함수입니다.
void CClientSocket::OnReceive(int nErrorCode)
{
    // 오류 코드가 0인 경우, 즉 수신 중 오류가 발생하지 않았을 경우에만 처리를 진행합니다.
    if (nErrorCode == 0)
    {
        // 서버로부터 수신된 데이터를 저장할 버퍼를 선언합니다.
        TCHAR szBuffer[1024] = { 0 };
        // Receive 함수를 사용하여 데이터를 수신하고, 수신된 바이트 수를 반환받습니다.
        int iRecvSize = Receive(szBuffer, (1024 - 1) * sizeof(TCHAR));
        // 수신된 데이터가 있을 경우
        if (iRecvSize > 0)
        {
            // 수신된 데이터의 마지막에 널 종료 문자를 추가합니다.
            szBuffer[iRecvSize / sizeof(TCHAR)] = _T('\0');

            // 메인 대화 상자의 포인터를 가져옵니다.
            CClientMFCDlg* pDialog = (CClientMFCDlg*)AfxGetMainWnd();
            if (pDialog != nullptr)
            {
                // 대화 상자의 특정 컨트롤(IDC_SERVER_TIME_DISPLAY)에 수신된 데이터를 표시합니다.
                pDialog->SetDlgItemText(IDC_SERVER_TIME_DISPLAY, szBuffer);
            }
        }
    }

    // 기본 클래스의 OnReceive 함수를 호출하여 추가적인 처리를 수행합니다.
    CSocket::OnReceive(nErrorCode);
}

// 소켓이 닫히는 경우 호출되는 함수입니다.
void CClientSocket::OnClose(int nErrorCode)
{
    // 소켓이 닫힐 때 필요한 처리를 여기에 작성할 수 있습니다.

    // 기본 클래스의 OnClose 함수를 호출하여 소켓 닫힘 처리를 완료합니다.
    CSocket::OnClose(nErrorCode);
}

// CClientSocket의 소멸자입니다.
CClientSocket::~CClientSocket() {}
