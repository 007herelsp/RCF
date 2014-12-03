/*
�ͻ��ˣ���Ҫ��Ч���ӣ���Ҫ����RcfServer��������RcfServer::createCallbackConnection()
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

RCF_BEGIN(I_IdentifyClient, "I_IdentifyClient")
	RCF_METHOD_V1(void, SetClientName, const string &)
RCF_END(I_IdentifyClient)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << s << endl;
	}
};

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer callbackServer((RCF::TcpEndpoint())); // Ϊʲô��Ҫ�յ� ������������Ϊ����c���ݣ���Ϊ����������RCF::TcpEndpoint()Ϊ��������
		HelloWorld helloWorld;
		callbackServer.bind<I_HelloWorld>(helloWorld);
		callbackServer.start();

		RcfClient<I_IdentifyClient> client(RCF::TcpEndpoint(50004));
		// �����ص�
		client.SetClientName("wangh");
		RCF::createCallbackConnection(client, callbackServer);
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