/*
����������Ƕ�̬�İ󶨶˿ڣ���ಥ�͹㲥���ڿͻ���������������Ӿͺ�������
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

// �㲥TCP�˿� �ӿڣ����Թ㲥��������ַô��
RCF_BEGIN(I_Broadcast, "I_Broadcast")
	RCF_METHOD_V1(void, serverIsRunningOnPort, int)
RCF_END(I_Broadcast)
// ���ܹ㲥���ݵ�ʵ����
class Broadcast
{
public:
	Broadcast() :m_Port(){}
	void serverIsRunningOnPort(int port)
	{
		m_Port = port;
	}
	int m_Port;
};

// �������̣߳�ÿ��㲥һ�η�����λ�ã�����ಥ��ַ�����ã���Ҳ����ʹ�ù㲥��ַ����
void broadcastThread(int port, const string &multicastIp, int multicastPort)
{
	RcfClient<I_Broadcast> client(RCF::UdpEndpoint(multicastIp, multicastPort));
	client.getClientStub().setRemoteCallSemantics(RCF::Oneway);
	// ��ʼ�㲥
	while (true)
	{
		client.serverIsRunningOnPort(port);
		RCF::sleepMs(1000);
	}
	
}

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

		// ������̬�˿ڵķ�����
		RCF::RcfServer server(RCF::TcpEndpoint(0));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();
		// ��ȡ�������˿�
		int port = server.getIpServerTransport().getPort();
		// ��ʼ�㲥�˿�
		RCF::ThreadPtr broadcastThreadPtr(new RCF::Thread(boost::bind(&broadcastThread, port, "232.5.5.5", 50001)));

		// �ͻ����ڿ�ʼ����ǰ��Ҫ�Ȼ�ȡ�㲥��Ϣ
		Broadcast broadcast;
		RCF::RcfServer clientSizeBroadcastListener(RCF::UdpEndpoint("0.0.0.0", 50001).listenOnMulticast("232.5.5.5"));
		clientSizeBroadcastListener.bind<I_Broadcast>(broadcast);
		clientSizeBroadcastListener.start();
		// �ȴ��㲥
		while (!broadcast.m_Port)
		{
			RCF::sleepMs(1000);
		}

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(broadcast.m_Port));
		client.Print("���ǿͻ���");
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}