/*
��������  RcfServer::setOnCallbackConnectionCreated() ���ڿ��ƿͻ��˽����Ļص�����
*/

#include <iostream>
#include <vector>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)


typedef boost::shared_ptr<RcfClient<I_HelloWorld> > HelloWorldPtr;
RCF::Mutex gCallbackClientMutex;
vector<HelloWorldPtr> gCallbackClients;

// �ص����Ӵ�����ϵ���
void onCallbackConnectionCreated(RCF::RcfSessionPtr sessionPtr, RCF::ClientTransportAutoPtr transportPtr)
{
	HelloWorldPtr helloWorldPtr(new RcfClient<I_HelloWorld>(transportPtr));
	RCF::Lock lock(gCallbackClientMutex);
	gCallbackClients.push_back(helloWorldPtr);
}

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer server((RCF::TcpEndpoint(50001)));
		server.setOnCallbackConnectionCreated(boost::bind(&onCallbackConnectionCreated, _1, _2));
		server.start();

		// �ȴ����ӣ��ɵ��������ȴ��߳�
		while (gCallbackClients.empty())
		{
			RCF::sleepMs(1000);
		}

		// ��ʼ�ص����˴��ɵ������������߳�
		vector<HelloWorldPtr> clients;
		{
			RCF::Lock lock(gCallbackClientMutex);
			clients.swap(gCallbackClients);
		}
		// ���ÿͻ��˺���
		for (size_t i = 0; i < clients.size(); ++i)
		{
			HelloWorldPtr client = clients[i];
			client->Print("hello��");
		}

		cout << "������˳�..." << endl;
		cin.get();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}
	return 0;
}