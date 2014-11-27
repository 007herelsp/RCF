/*
RcfSession ���Ի�ȡ��ǰ�ͻ������ӵ���Ϣ
*/

/*
�ײ�������������ӻ�ȡ������һ�����Զ��ģ�Ҳ�����ֶ�
client.getClientStub().connect();
client.getClientStub().disconnect();
���Խ�һ���ͻ��˵ĵײ�Э���ƶ�����һ���ͻ���
ClientStub::releaseTransport()    ClientStub::setTransport();
Ӧ�ã��ڲ�ͬ�Ŀͻ���ʵ��ʹ��ͬһ���ײ㴫��Э��
*/


#include <iostream>

using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class MySessionObj
{
public:
	MySessionObj(){ count++; }
	static int count;
};
int MySessionObj::count = 0;

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "Print������: " << s << endl;
		// ��ȡ��ǰ���ӵĻỰ
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		// ����Э��
		RCF::TransportType transportType = session.getTransportType();
		// �ͻ��˵�ַ:�˿�
		const RCF::RemoteAddress &clientAddr = session.getClientAddress();
		cout << clientAddr.string() << endl;
		// ����
		RCF::RemoteCallRequest request = session.getRemoteCallRequest();
		// �Զ�������󸽼�����
		string customRequestData = session.getRequestUserData();
		// ������ȡ�ã�ɾ���Ự����
		session.createSessionObject<MySessionObj>();
		MySessionObj *pObj = session.querySessionObject<MySessionObj>();
		MySessionObj &obj = session.getSessionObject<MySessionObj>(); // �������������׳��쳣����ָ�����캯�� Ϊtrue
		cout << obj.count << endl;
		session.deleteSessionObject<MySessionObj>();
		// �ͻ��˵����ӳɹ���ʱ��㣨ms)
		time_t connectedAt = session.getConnectedAtTime();
		cout << connectedAt << endl;
		// �ͻ������ӵ�ʱ��(S)���ӿͻ��˵��ýӿڿ�ʼ����
		//RCF::sleepMs(2500);
		size_t connectedDurationS = session.getConnectionDuration();
		cout << connectedDurationS << endl;
		// �ͻ��˵ĵ��ô���
		size_t callCount = session.getRemoteCallCount();
		cout << callCount << endl;
		// ���ͺ��յ����ֽ���
		boost::uint64_t totalBytesReceived = session.getTotalBytesReceived();
		cout << totalBytesReceived << endl;
		boost::uint64_t totalBytesSent = session.getTotalBytesSent();
		cout << totalBytesSent << endl;
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
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		
		client.Print("client");
		client.Print("client");

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}