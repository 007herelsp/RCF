/*
RcfServer ����1����������������㣬���ͻ���ֻ����һ��
Ϊ�˻�ȡ�������Ĵ���㣬��Ҫ����addEndpoint()�ķ���ֵ
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "Print������: " << s << endl;
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

		RCF::RcfServer server;
		// �������˵Ĵ����
		RCF::ServerTransport &tcpTransport = server.addEndpoint(RCF::TcpEndpoint(50001));
		RCF::ServerTransport &udpTransport = server.addEndpoint(RCF::UdpEndpoint(50001));
		// ��������ֻ��һ������㣬�����ʹ��getServerTransport()
		// RCF::ServerTransport &transport = server.getServerTransport();
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// �ͻ��˻�ȡ�����
		RCF::ClientTransport &clientTransport = client.getClientStub().getTransport();
		client.Print("���ǿͻ���");

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}