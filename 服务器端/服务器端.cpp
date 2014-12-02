/*
��������  RcfServer::setOnCallbackConnectionCreated() ���ڿ��ƿͻ��˽����Ļص�����
*/

#include <iostream>
#include <vector>
#include <map>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

RCF_BEGIN(I_IdentifyClient, "I_IdentifyClient")
	RCF_METHOD_V1(void, SetClientName, const string &)
RCF_END(I_IdentifyClient)

// ���ÿͻ������֣�����ʶ��
class IdentifyClient
{
public:
	void SetClientName(const string &clientName)
	{
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		string &sessionClientName = session.createSessionObject<string>();
		sessionClientName = clientName;
	}
};

typedef RcfClient<I_HelloWorld> helloWorldClient;
typedef boost::shared_ptr<helloWorldClient > HelloWorldPtr;
RCF::Mutex gCallbackClientMutex;
//vector<HelloWorldPtr> gCallbackClients;
map<string, HelloWorldPtr> gCallbackClients;

// �ص����Ӵ�����ϵ���
void onCallbackConnectionCreated(RCF::RcfSessionPtr sessionPtr, RCF::ClientTransportAutoPtr transportPtr)
{
	string &clientName = sessionPtr->getSessionObject<string>();
	cout << "clientName" << clientName << endl;

	// ����
	HelloWorldPtr helloWorldPtr(new RcfClient<I_HelloWorld>(transportPtr));
	RCF::Lock lock(gCallbackClientMutex);
	//gCallbackClients.push_back(helloWorldPtr);
	gCallbackClients[clientName] = helloWorldPtr;
}

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer server((RCF::TcpEndpoint(50004)));
		server.setOnCallbackConnectionCreated(boost::bind(&onCallbackConnectionCreated, _1, _2));
		IdentifyClient identifyClient;
		server.bind<I_IdentifyClient>(identifyClient);
		server.start();

		// �ȴ����ӣ��ɵ��������ȴ��߳�
		while (gCallbackClients.empty())
		{
			RCF::sleepMs(1000);
		}

		// ��ʼ�ص����˴��ɵ������������߳�
		//vector<HelloWorldPtr> clients;
		map<string, HelloWorldPtr> clients;
		{
			RCF::Lock lock(gCallbackClientMutex);
			clients.swap(gCallbackClients);
		}
		// ���ÿͻ��˺���
		for (size_t i = 0; i < clients.size(); ++i)
		{
			HelloWorldPtr client = clients["wangh"];
			//HelloWorldPtr client = clients[i];
			client->Print("hello��");
		}

		cout << "������˳�..." << endl;
		cin.get();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		cout << "������˳�..." << endl;
		cin.get();
	}
	return 0;
}