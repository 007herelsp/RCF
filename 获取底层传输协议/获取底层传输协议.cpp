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
	RCF_METHOD_V1(void, AnotherPrint, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "Print������: " << s << endl;
	}
	void AnotherPrint(const string &s)
	{
		cout << "AnotherPrint������: " << s << endl;
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
		RCF::ClientStub &clientStub = client.getClientStub();
		RcfClient<I_HelloWorld> client2(client.getClientStub().releaseTransport());
		client2.AnotherPrint("client2");
		clientStub.setTransport(client2.getClientStub().releaseTransport());
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