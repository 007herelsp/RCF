/*
ʹ��win32�����ܵ��������׵���֤�ͻ��˵���ݣ�ͨ�� RCF::Win32NamedPipeImpersonator�� ��ʹ��Win32API ImpersonateNamedPipeClient()ģ��ͻ���
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF/Win32NamedPipeServerTransport.hpp>
#include <RCF/Win32Username.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(string, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	string Print(const string &s)
	{
		cout << "������: " << s << endl;
		// ģ��ͻ���
		RCF::Win32NamedPipeImpersonator impersonator;
		wcout << "�ͻ��˵�����: " << RCF::getMyUserName() << endl;
		return s;
	}
};

int main()
{
	try{
		RCF::init();
		RCF::RcfServer server(RCF::Win32NamedPipeEndpoint(_T("PiPe")));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();


		RcfClient<I_HelloWorld> client(RCF::Win32NamedPipeEndpoint(_T("PiPe")));
		string s = client.Print("���ǿͻ���1");
		cout << "�ͻ���: " << s << endl;
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}