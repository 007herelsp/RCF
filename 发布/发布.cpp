/*
�������ɷ���������⣬ΨһҪ���ǣ����ķ��ܷ����������ӣ��������Ӳ��붩�ķ���������
RcfServer::createPublisher<>();
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(int, Print, const string &)
RCF_END(I_HelloWorld)

//class HelloWorld
//{
//public:
//	int Print(const string &s)
//	{
//		cout << s << endl;
//		return s.size();
//	}
//};

// ���ʿ���
bool onSubscriberConnect(RCF::RcfSession &session, const string &topicName)
{
	cout << "���ķ�����" << topicName << ":" << session.getClientAddress().string() << endl;
	return true;
}

void onSubscriberDisconnect(RCF::RcfSession &session, const string &topicName)
{
	// �Ͽ����� session ����ʹ�ã�����
	cout << "���ķ��Ͽ�����" << topicName << ":" << session.getClientAddress().string() << endl;
}

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer pubServer(RCF::TcpEndpoint(50001));
		pubServer.start();
		typedef boost::shared_ptr<RCF::Publisher<I_HelloWorld> > HelloWorldPubPtr;
		// ����Ĭ�����Ƶ�Publisher(����ʱ��ʾ"I_HelloWorld"
		RCF::PublisherParms pubParms;
		pubParms.setOnSubscriberConnect(&onSubscriberConnect);
		pubParms.setOnSubscriberDisconnect(&onSubscriberDisconnect);
		HelloWorldPubPtr pubPtr = pubServer.createPublisher<I_HelloWorld>(pubParms);
		// �ֶ�����������
		// RCF::PublisherParms pubParms;
		pubParms.setTopicName("HelloWorld_Topic_1");
		HelloWorldPubPtr pub1Ptr = pubServer.createPublisher<I_HelloWorld>(pubParms);

		while (true)
		{
			RCF::sleepMs(1000);
			pubPtr->publish().Print("HelloWorld"); // OneWay��ʽ
		}
		// �ر�
		pub1Ptr->close();
		pubPtr->close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();



	return 0;
}