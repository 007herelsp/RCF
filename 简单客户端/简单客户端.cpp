/*
�ͻ��ˣ����ڴ�������
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

int main()
{
	RCF::init();
	try
	{
		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		client.Print("�ͻ���");
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