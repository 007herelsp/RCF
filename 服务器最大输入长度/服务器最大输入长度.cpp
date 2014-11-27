/*
��������ÿ���ͻ���������������������Ϣ�ֽ���
��������ã������������������ֽ�������ɿͻ��˷�����ڴ�ռ����
Ĭ�ϵ��� 1Mb	I_ServerTransport::setMaxIncomingMessageLength() �ı����С
			I_ClientTransport::setMaxIncomingMessageLength()
�ͻ���ֻ��һ�������������ݣ���ȷ������ϸɾ�
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
		cout << "Print������: " << s << endl;
		return s;
	}
};

void close()
{
	cout << "������˳�..." << endl;
	cin.get();
}

int main()
{
	try
	{
		RCF::init();
		HelloWorld helloWorld;

		RCF::RcfServer server(RCF::TcpEndpoint(50001));
		// ��������������������ݳ���
		server.getServerTransport().setMaxIncomingMessageLength(5 * 1024 * 1024); // 5Mb
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// �ͻ���
		client.getClientStub().getTransport().setMaxIncomingMessageLength(5 * 1024 * 1024);
		string s = client.Print("���ǿͻ���");
		cout << "�ͻ���: " << s << endl;

		// ��ȡǰһ�����õ��������Ӧ�����ݴ�С
		RCF::ClientTransport &transport = client.getClientStub().getTransport();
		size_t requestSize = transport.getLastRequestSize();
		size_t responseSize = transport.getLastResponseSize();
		cout << "requestSize: " << requestSize << endl;
		cout << "responseSize: " << responseSize << endl;
		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}

	return 0;
}