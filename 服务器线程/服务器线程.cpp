/*
	Ĭ��RCF���������߳�Ϊ���̣߳�������Ϊ���߳�  RcfServer::setThreadPool()
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
		cout << "������: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();

		RCF::RcfServer server(RCF::TcpEndpoint(50001));
		RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(5));// �̶��߳���
		// RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(1, 25)); // �����߳���
		server.setThreadPool(tpPtr); // ���ַ�ʽ���õ��̶߳����д��䷽ʽ��Ч

		// �ɵ���Ϊĳ�ַ�ʽ�����߳�
		// 	RCF::ServerTransport &udpTransport = server.addEndpoint(RCF::UdpEndpoint(50001));
		// 	RCF::ServerTransport &pipeTransport = server.addEndpoint(RCF::Win32NamedPipeEndpoint(_T("PIPE")));
		// 	RCF::ThreadPoolPtr udpTpPtr(new RCF::ThreadPool(1, 5));
		// 	udpTransport.setThreadPool(udpTpPtr);
		// 	RCF::ThreadPoolPtr pipeTpPtr(new RCF::ThreadPool(1));
		// 	pipeTransport.setThreadPool(pipeTpPtr);

		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		cout << "���÷������˵� Print ����" << endl;

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		client.Print("���ǿͻ���1");
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}