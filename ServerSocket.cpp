#include "pch.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

// ServerSocket 클래스의 생성자입니다.
ServerSocket::ServerSocket()
{
    // 여기서 필요한 초기화 작업을 수행합니다.
    // 예를 들어, 소켓 옵션 설정, 내부 변수 초기화 등을 할 수 있습니다.
}

// ServerSocket 클래스의 소멸자입니다.
ServerSocket::~ServerSocket()
{
    // 클래스 소멸 시 모든 클라이언트 소켓을 닫고 메모리를 해제합니다.
    CloseClientSockets();
}

// 새 클라이언트의 연결 수락을 처리하는 메서드입니다.
void ServerSocket::OnAccept(int nErrorCode)
{
    // nErrorCode가 0이면 오류 없이 연결 수락이 가능한 상태입니다.
    if (nErrorCode == 0)
    {
        // 새 클라이언트 소켓 객체를 생성합니다.
        ClientSocket* pClientSocket = new ClientSocket();
        
        // Accept 메서드를 호출하여 클라이언트 연결을 수락합니다.
        if (Accept(*pClientSocket))
        {
            // 연결이 성공적으로 수락되면 클라이언트 소켓을 관리 목록에 추가합니다.
            pClientSocket->m_pDialog = m_pParentDialog; // 대화 상자 인스턴스를 클라이언트 소켓에 연결합니다.
            m_ClientSockets.push_back(pClientSocket); // 소켓 목록에 새 소켓을 추가합니다.
            AfxMessageBox(_T("연결된 클라이언트 소켓을 리스트에 추가합니다."));
        }
        else
        {
            // 연결 수락에 실패한 경우, 생성된 클라이언트 소켓 객체를 삭제합니다.
            delete pClientSocket;
        }
    }
}

// 모든 클라이언트 소켓을 닫고 메모리를 해제하는 메서드입니다.
void ServerSocket::CloseClientSockets()
{
    // m_ClientSockets 벡터에 저장된 모든 클라이언트 소켓에 대해 반복합니다.
    for (auto& socket : m_ClientSockets)
    {
        socket->ShutDown(2); // 소켓의 데이터 송수신을 중단합니다. 인자 2는 송수신 모두를 의미합니다.
        socket->Close(); // 소켓을 닫습니다.
        delete socket; // 소켓 객체를 메모리에서 해제합니다.
    }
    m_ClientSockets.clear(); // 클라이언트 소켓 목록을 비웁니다.
}
