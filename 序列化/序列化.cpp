/*
Զ�̵�����Ҫ�����л�Ϊ�����������������紫�䣬���ն���Ҫ��ϵ�л�ΪC++����
RCF �� SF ���ṩ���л���ܣ�ʹ�ô˿�ܿ����Զ����л�C++���������ͣ��Լ��Զ�������
����������RCF�Զ����л�������������Ҫ������ص�ͷ�ļ�
һ�㣬���C++�����Ͷ���Ϊ <xyz> �� Boost������ <boost/xyz.hpp>�������Ӧ�����л����� <SF/xyz.hpp>
C++��enmu�����Զ����л�Ϊint   C++11��enum class ���� ��Ҫ�����

*/


#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

// �Զ����л�Ϊint
enum Suit
{
	Heart = 1,
	Diamond = 2,
	Club = 2
};

// C++11 enum class
enum class Colors : int8_t
{
	Red = 1,
	Green = 2,
	Blue = 3
};
SF_SERIALIZE_ENUM_CLASS(Colors, int8_t) // 

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
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
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));

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