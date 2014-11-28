/*
����IPv6��Ҫ����� RCF_USE_IPv6
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
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		cout << "IPv4/IPv6: " << session.getClientAddress().string() << endl;
		cout << "������: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();

		RCF::RcfServer server;
		//server.addEndpoint(RCF::TcpEndpoint("127.0.0.1", 50001)); // IPv4
		//server.addEndpoint(RCF::TcpEndpoint("::1", 50001)); // IPv6
		// ��������ģʽ
		server.addEndpoint(RCF::TcpEndpoint("0.0.0.0", 50001));
		server.addEndpoint(RCF::TcpEndpoint("::0", 50001));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		//RcfClient<I_HelloWorld> client(RCF::TcpEndpoint("127.0.0.1", 50001));
		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint("::1", 50001));
		// RCFʹ��POSIX getaddrinfo()��ȡIP��ַ�����ʹ��������ʽ���ַ������������IPv4��IPv6��ȡ����ϵͳ����
		// RcfClient<I_HelloWorld> client(RCF::TcpEndpoint("machin.domain", 50001));
		// Ҳ����ǿ�ƿͻ���ʹ������Э��
//		RCF::IpAddressV4 addr_4("machin.domin", 50001);
//		RcfClient<I_HelloWorld> client((RCF::TcpEndpoint(addr_4))); // ΪʲôҪ���һ�����ţ���
// 		RCF::IpAddressV6 addr_6("machin.domin", 50001);
// 		RcfClient<I_HelloWorld> client((RCF::TcpEndpoint(addr_6)));
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