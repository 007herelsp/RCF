/*
TCP: RCF::TcpEndpoint()
UDP: RCF::UdpEndpoint()
����UDP���ṩ��Ϊ�Ĺ��ܣ�����ಥ�͹㲥
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
		cout << "������: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();
		// ��������Ȼ��Ҫ�ƶ����������ڼ���
		// RCF::RcfServer server(RCF::UdpEndpoint("0.0.0.0", 50001).listenOnMulticast("232.5.5.5"));
		RCF::RcfServer server(RCF::UdpEndpoint("0.0.0.0", 50001));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		//RcfClient<I_HelloWorld> client(RCF::UdpEndpoint("232.5.5.5", 50001));
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