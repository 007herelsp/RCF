/*
ʵ����(servant object)����ʵ�� RCF ����Ľӿڣ�ÿһ��ʵ���඼��һ��������ָʾ��
Ĭ�ϵİ�����������ʱ��ʶ��Ҳ��ʶI_HelloWorld�ӿڣ�
Ҳ�����ڵ���bind����ʱ��ȷ��ָ�������֣���ʱ�ͻ��˾ͱ���ʹ�ô��Զ���İ����ֽ�������
*/


#include <iostream>

using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld") // (����ʱ�ӿڱ�ʶ, ����ʱ�ӿڱ�ʶ)
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "Print������: " << s << endl;
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
		server.bind<I_HelloWorld>(helloWorld, "CustomBindingName");
		server.start();

		close();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001), "CustomBindingName");
		RCF::ClientStub &clientStub = client.getClientStub();
		
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