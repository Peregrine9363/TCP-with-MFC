#include "pch.h"
#include "ClientSocket.h"
#include <chrono>
#include <thread>
#include "Resource.h"
#include "Server_MFCDlg.h"

// 소멸자: 클라이언트 소켓 객체가 소멸될 때 필요한 정리 작업을 수행합니다.
ClientSocket::~ClientSocket() {}

// 데이터를 받았을 때 호출되는 함수입니다.
void ClientSocket::OnReceive(int nErrorCode)
{
    // 오류 코드가 0이고, 대화 상자에 대한 포인터가 유효한 경우에만 처리를 계속합니다.
    if (nErrorCode == 0 && m_pDialog != nullptr)
    {
        int nReceivedSignal = 0;
        // 소켓으로부터 데이터를 받아옵니다. 이 예제에서는 정수 형태의 신호를 받아옵니다.
        int nBytesReceived = Receive(&nReceivedSignal, sizeof(nReceivedSignal));
        // 데이터 수신이 성공적이면 처리를 계속합니다.
        if (nBytesReceived > 0)
        {
            // 수신된 신호가 1인 경우, 특정 작업을 수행합니다.
            if (nReceivedSignal == 1)
            {
                // 대화 상자의 특정 컨트롤에 메시지를 표시합니다.
                m_pDialog->SetDlgItemText(IDC_L_PORT, _T("클라이언트로 데이터를 전송중입니다."));
                // 현재 시간을 클라이언트에 전송하는 함수를 호출합니다.
                StartSendingCurrentTime();
            }
            else
            {
                // 수신된 신호가 1이 아닌 경우, 오류 메시지를 표시합니다.
                AfxMessageBox(_T("클라이언트로부터 메세지를 수신하지 못했습니다."));
            }
        }
    }
}

// 현재 시간을 포맷에 맞춰 문자열로 변환하는 함수입니다.
CString ClientSocket::GetCurrentFormattedTime()
{
    using namespace std::chrono;

    // 현재 시간을 얻습니다.
    auto now = system_clock::now();
    // 밀리초 단위로 현재 시간을 변환합니다.
    auto now_ms = time_point_cast<milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = duration_cast<milliseconds>(epoch);

    // 현재 시간을 "시:분:초.밀리초" 형태의 문자열로 포맷팅합니다.
    time_t tt = system_clock::to_time_t(now);
    tm t;
    localtime_s(&t, &tt);
    CString strTime;
    strTime.Format(_T("%02d:%02d:%02d.%03lld"), t.tm_hour, t.tm_min, t.tm_sec, value.count() % 1000);

    return strTime;
}

// 별도의 스레드에서 현재 시간을 주기적으로 클라이언트에 전송하는 함수입니다.
void ClientSocket::StartSendingCurrentTime()
{
    // 멤버 함수에 접근하기 위해 'this' 포인터를 사용합니다.
    std::thread([this]() {
        while (true)
        {
            // 현재 시간을 포맷에 맞춰 가져옵니다.
            CString strTime = this->GetCurrentFormattedTime();
            // 가져온 시간을 클라이언트에 전송합니다.
            int iSendResult = this->Send(strTime.GetBuffer(), strTime.GetLength() * sizeof(TCHAR));

            // 전송에 실패하면 루프를 종료합니다.
            if (iSendResult == SOCKET_ERROR)
            {
                break;
            }

            // 일정 시간(0.1초) 대기합니다.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).detach(); // 생성된 스레드를 분리하여 독립적으로 실행되게 합니다.
}
