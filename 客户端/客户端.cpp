/*
�ͻ��ˣ���Ҫ��Ч���ӣ���Ҫ����RcfServer��������RcfServer::createCallbackConnection()
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
		cout << s << endl;
	}
};

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer callbackServer((RCF::TcpEndpoint())); // Ϊʲô��Ҫ�յ� ��Ҫ����������
		HelloWorld helloWorld;
		callbackServer.bind<I_HelloWorld>(helloWorld);
		callbackServer.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// �����ص�
		RCF::createCallbackConnection(client, callbackServer);
		cout << "������˳�..." << endl;
		cin.get();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}
	return 0;
}