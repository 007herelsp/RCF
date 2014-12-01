/*
��ȡ�ײ�Socket  �����Զ���socket����
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <RCF/TcpAsioServerTransport.hpp>
#include <RCF/TcpClientTransport.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		cout << "IPv4/IPv6: " << session.getClientAddress().string() << endl;

		// �������˻�ȡ���ӵ�socket
		RCF::SessionState &sessionState = RCF::getCurrentRcfSession().getSessionState();
		RCF::TcpAsioSessionState &tcpSessionState = dynamic_cast<RCF::TcpAsioSessionState &>(sessionState);
		cout << "��������socket: " << tcpSessionState.getNativeHandle() << endl;

		cout << "������: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();
	
		RCF::RcfServer server;
		server.addEndpoint(RCF::TcpEndpoint("0.0.0.0", 50001));
		server.addEndpoint(RCF::TcpEndpoint("::0", 50001));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		//client.getClientStub().connect(); // ���������connect() ʵ�����ӻ��Ƴٵ���һ�νӿڵ���
		
		client.Print("���ǿͻ���1");

		// �ͻ��˻�ȡ���ӵ�socket
		RCF::TcpClientTransport &transport = dynamic_cast<RCF::TcpClientTransport &>(client.getClientStub().getTransport());
		cout << "�ͻ���Socket: " << transport.getNativeHandle() << endl;
		
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}