/*
RCF Ŀǰ֧�� NTML Kerberos Negotiate �� SSL ǰ����ֻ����windowsƽ̨��SSL����������ƽ̨
֧�� zlib ѹ��
�ͻ������ã�ClientStub::setTransportProtocol();
(��ѡ�����������ã�RcfServer::setSupportedTransportProtocols(); // ����������ʱ��������һ����
��������ȡ��RcfSession::getTransportPtotocol();
*/

/*
NTML��windows���ڰ汾�ı�׼��ȫЭ�飬win2000���Ժ�Ĭ��ʹ��Kerberos
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(string, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	string Print(const string &s)
	{
		cout << "������: " << s << endl;
		// ����ȡ�ؿͻ���ʹ�õ�Э��
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		RCF::TransportProtocol protocol = session.getTransportProtocol();
		if (protocol == RCF::Tp_Ntlm)
		{
			RCF::tstring clientUserName = session.getClientUsername();
			wcout << "�ͻ��˵�����: " << clientUserName << endl;
			// ģ��ͻ��� ��Ҫ��RCF.hpp ��� SspiFilter.hpp
			RCF::SspiImpersonator impersonator(session);
			// ...
		}
		return s;
	}
};

int main()
{
	try{
		RCF::init();
		RCF::RcfServer server(RCF::TcpEndpoint(50002));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();


		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50002));
		client.getClientStub().setTransportProtocol(RCF::Tp_Ntlm);
		string s = client.Print("���ǿͻ���1");
		cout << "�ͻ���: " << s << endl;
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}