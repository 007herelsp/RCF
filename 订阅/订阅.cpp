/*
����
RcfServer::createSubscription<>();
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(int, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	int Print(const string &s)
	{
		cout << s << endl;
		return s.size();
	}
};

// ���ķ��Ͽ����ӻص�����
void onSubscriptionDisconnected(RCF::RcfSession &session)
{
	cout << "�Ͽ�����" << endl;

	cout << "������˳�..." << endl;
	cin.get();
}

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer subServer((RCF::TcpEndpoint())); // �˴�������˿ں�
		subServer.start();
		HelloWorld helloWorld;
		RCF::SubscriptionParms subParms;
		subParms.setPublisherEndpoint(RCF::TcpEndpoint(50001));
		subParms.setOnSubscriptionDisconnect(&onSubscriptionDisconnected);
		// Ĭ��������  I_HelloWorld
		RCF::SubscriptionPtr subPtr = subServer.createSubscription<I_HelloWorld>(
			helloWorld, // ָ���Ǹ�ʵ������ܵ��ã��ڶ�����������ʱ���˶����ܱ�����
			subParms);
		// �ֶ�ָ��
		subParms.setTopicName("HelloWorld_Topic_1");

		cout << "������˳�..." << endl;
		cin.get();
		// �ر�
		subPtr->close();
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