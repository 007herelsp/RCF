/*
һ�����ݶ�����Ϊ�������ݣ����ɶ��⸽�����ݵ�����������û������򣬷������������� Ӧ�� ������������
Ӧ�ã�����Ϊ�Զ������֤
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

		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		string customRequestData = session.getRequestUserData();
		cout << "�����û�����: " << customRequestData << endl;

		string customResponseData = "f81d4fae-7dec-11d0-a765-00a0c91e6bf6";
		session.setResponseUserData(customResponseData);
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
		clientStub.setRequestUserData("e6a9bb54-da25-102b-9a03-2db401e887ec");
		client.Print("�û�����");
		string responseData = clientStub.getResponseUserData();
		cout << "Ӧ���û�����: " << responseData << endl;

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}
	

	return 0;
}