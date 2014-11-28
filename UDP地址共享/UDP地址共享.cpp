/*
udp�����Թ���󶨵ĵ�ַ����˶��server���Լ���ͬһ���˿ڣ��ڶಥģʽ��Ĭ�Ͽ����ģ��Ƕಥģʽ�������ÿ���
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
		cout << "������HelloWorld: " << s << endl;
	}
};

class HelloWorld2
{
public:
	void Print(const string &s)
	{
		cout << "������HelloWorld2: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();
		// ��ַ����
		RCF::RcfServer server(RCF::UdpEndpoint("0.0.0.0", 50001).enableSharedAddressBinding());
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RCF::RcfServer server2(RCF::UdpEndpoint("0.0.0.0", 50001).enableSharedAddressBinding());
		HelloWorld2 helloWorld2;
		server2.bind<I_HelloWorld>(helloWorld2);		
		server2.start();
		

		// ��ʱ�㲥�����������������յ���Ϣ���ǹ㲥���һ�������ķ��������յ���Ϣ������Ƕ��������
		RcfClient<I_HelloWorld> client(RCF::UdpEndpoint("255.255.255.255", 50001)); // �㲥
		client.getClientStub().setRemoteCallSemantics(RCF::Oneway);
		client.Print(/*RCF::Oneway, */"���ǿͻ���1"); // UDP��Ҫָ��Oneway �����ȴ���Ӧ
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}