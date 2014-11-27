/*
�������˷ַ� �ͻ��˵�Զ�̵��� ��Ӧ

��Ҫ��� RCF/include   boost/include
������Ҫ���� RCF.cpp
�����win32  ��Ҫ����Ŀ���Զ��� WIN32_LEAN_AND_MEAN
*/


#include <iostream>
#include <vector>
using namespace std;

#include <RCF/RCF.hpp>
#include <SF/vector.hpp> // vector�����л�ʵ����
/************************************************************************/
// �ӿڶ���
/************************************************************************/
RCF_BEGIN(I_HelloWorld, "I_HelloWorld") // (����ʱ�ӿڱ�ʶ, ����ʱ�ӿڱ�ʶ)
	RCF_METHOD_V1(void, Print, const string &) // ����һ������������ֵΪvoid�Ľӿ�
	RCF_METHOD_R1(int, Print, const vector<string> &)// ����һ������������ֵΪstring�Ľӿ�
	RCF_METHOD_V2(void, Print, const vector<string> &, int &)
RCF_END(I_HelloWorld)
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// �Ự��Ϣ 
/************************************************************************/
class HelloWorldSession
{
public:
	HelloWorldSession(){ m_CallCount = 0; cout << "Created HelloWorldSession object." << endl; }
	~HelloWorldSession() { cout << "Destroyed HelloWorldSession object." << endl; }
	size_t m_CallCount;
};
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// �ӿ�ʵ�� 
/************************************************************************/
class HelloWorld
{
public:
	void Print(const string &s)
	{
		// ÿһ���ͻ��˶����Լ���session��������Ϊ�ײ�Э���������
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		// ����ȡ�ؿͻ���ʹ�õ�Э��
		RCF::TransportProtocol protocol = session.getTransportProtocol();
		if (protocol == RCF::Tp_Ntlm || protocol == RCF::Tp_Kerberos)
		{
			RCF::tstring clientUserName = session.getClientUsername();
			// ģ��ͻ��� ��Ҫ��RCF.hpp ��� SspiFilter.hpp
			RCF::SspiImpersonator impersonator(session);
			// ...
		}

		HelloWorldSession &hwSession = session.getSessionObject<HelloWorldSession>(true);
		++hwSession.m_CallCount;

		//throw runtime_error("Print() can't be called");
		cout << "������: " << s << endl;
		cout << "�ܹ����ô���: " << hwSession.m_CallCount << endl;
	}
	int Print(const vector<string> &v)
	{
		int howManyChars = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			cout << "������: " << v[i] << endl;
			howManyChars += v[i].size();
		}
		return howManyChars;
	}
	void Print(const vector<string> &v, int &howManyChars)
	{
		howManyChars = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			cout << "������: " << v[i] << endl;
			howManyChars += v[i].size();
		}
	}
};
//////////////////////////////////////////////////////////////////////////

int main()
{
	RCF::RcfInitDeinit rcfInit;
	HelloWorld helloWorld;
	// RCF::RcfServer server(RCF::TcpEndpoint(50001)); // ������ṩ��ַ����Ĭ��ʹ��127.0.0.1
	// RCF::RcfServer server(RCF::TcpEndpoint("127.0.0.1", 50001));
	// RCF::RcfServer server(RCF::TcpEndpoint("0.0.0.0", 50001)); // �������е�ַ��IPv4   RCF::RcfServer server(RCF::TcpEndpoint("::0", 50001)); // IPv6
	// RCF::RcfServer server(RCF::UdpEndpoint("0.0.0.0", 50001)); // Udp  ����ʹ�� twoway ��request/response��
	// RCF::RcfServer server(RCF::Win32NamedPipeEndpoint(_T("Pipe"))); // ��Ҫ��ͷ�ļ�RCF.hpp ��windowsƽ̨ʹ��windows�����ܵ��� ��Unixƽ̨ʹ�ñ����׽��� 

	RCF::RcfServer server;
	// ͬʱָ��
	server.addEndpoint(RCF::TcpEndpoint("0.0.0.0", 50001));
//	server.addEndpoint(RCF::TcpEndpoint("::0", 50001)); // ����Щϵͳ���˷�ʽ��ͬʱ����IPv4��IPv6
	// ������ģʽ
	server.addEndpoint(RCF::TcpEndpoint("::0", 50001));
	server.addEndpoint(RCF::UdpEndpoint("::0", 50002));
	server.addEndpoint(RCF::Win32NamedPipeEndpoint(_T("Pipe")));

	// ��ѡ�����ô���Э�飬��Ӧ�Ŀͻ�����Ҫ��������һ��Э��
// 	vector<RCF::TransportProtocol> protocols;
// 	protocols.push_back(RCF::Tp_Ntlm);
// 	protocols.push_back(RCF::Tp_Kerberos);
// 	server.setSupportedTransportProtocols(protocols);

	// RCF::RcfServer server(RCF::HttpEndpoint("0.0.0.0", 80)); // http
	
	// Https  ��Ҫ��RCF.hpp ����� Win32Certificate.hpp
	// RCF::RcfServer server(RCF::HttpsEndpoint("0.0.0.0", 443));
	server.bind<I_HelloWorld>(helloWorld);  // �������󶨵��ӿ�ʵ�ֶ���
	// RCF::CertificatePtr serverCertPtr(new RCF::PfxCertificate("path/to/certificate.p12", _T("password"), _T("CertificateName")));
	// server.setCertificate(serverCertPtr);
	
	server.start();

	cout << "������˳�..." << endl;
	cin.get();

	server.stop(); // server�ڳ������������Զ�stop
	return 0;
}

/************************************************************************/
// RCF֧�� NTLM Kerberos SSL ����Э�飬ǰ���߽�֧�� windows, SSL֧������ƽ̨
/************************************************************************/