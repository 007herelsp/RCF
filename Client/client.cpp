/*
��Ҫ��� RCF/include   boost/include
������Ҫ���� RCF.cpp
�����win32  ��Ҫ����Ŀ���Զ��� WIN32_LEAN_AND_MEAN
*/
#include <iostream>
#include <vector>
using namespace std;

#include <RCF/RCF.hpp>

#include <SF/vector.hpp> 

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
	RCF_METHOD_R1(int, Print, const vector<string> &)
	RCF_METHOD_V2(void, Print, const vector<string> &, int &)
RCF_END(I_HelloWorld)

int main()
{
	RCF::RcfInitDeinit rcfInit;
	cout << "���÷������˵� Print ����" << endl;
	try {
		// RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// RcfClient<I_HelloWorld> client(RCF::TcpEndpoint("127.0.0.1", 50001));
		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint("wangh", 50001)); // �����������������IPv6��ַ�������ʹ�÷���������������
		// RcfClient<I_HelloWorld> client(RCF::UdpEndpoint("192.168.0.37", 50001)); // Udp
		// RcfClient<I_HelloWorld> client(RCF::Win32NamedPipeEndpoint(_T("Pipe")));

		// Http
// 		RcfClient<I_HelloWorld> client(RCF::HttpEndpoint("server1.acme.com", 80));
// 		client.getClientStub().setHttpProxy("proxy.acme.com");
// 		client.getClientStub().setHttpProxyPort(8080);

		// Https
// 		RcfClient<I_HelloWorld> client(RCF::HttpEndpoint("server1.acme.com", 443));
// 		client.getClientStub().setHttpProxy("proxy.acme.com");
// 		client.getClientStub().setHttpProxyPort(8080);

		// �ͻ��˴���
		RCF::ClientStub &clientStub = client.getClientStub();
		// ����Ҫ������ ���ӳ�ʱʱ���Զ�̵��ó�ʱʱ��
		clientStub.setConnectTimeoutMs(2000);
		clientStub.setRemoteCallTimeoutMs(2000);
		// ��������������˴���Э�飬��ͻ��˱���ѡ��һ��
		clientStub.setTransportProtocol(RCF::Tp_Ntlm); // ��ʱ�ͻ���ʹ��NTLMЭ���������֤����ݼ����ӽ��мӽ���
		// clientStub.setTransportProtocol(RCF::Tp_Kerberos);
		// NTLM�����ṩȷ�е�֤��
		clientStub.setUsername(_T("wangh"));
		clientStub.setPassword(_T("123456"));
		// KerberosЭ��
		// clientStub.setKerberosSpn(_T("1234"));
		
		// Զ�̵���
		client.Print("���ǿͻ���1");
		client.Print("���ǿͻ���2");
		client.Print("���ǿͻ���3");

		vector<string> strToPrint;
		strToPrint.push_back("AAA");
		strToPrint.push_back("BBB");
		strToPrint.push_back("��");
		int howManyChars = client.Print(strToPrint);
		cout << "howManyChars: " << howManyChars << endl;

		int how = 0;
		client.Print(strToPrint, how);
		cout << how << endl;

	}
	catch (RCF::Exception &e)
	{
		cout << "ErrorId: " << e.getErrorId() << endl; // ������
		cout << "Error: " << e.getErrorString() << endl; // ������Ϣ
	}
	

	cin.get();
	return 0;
}