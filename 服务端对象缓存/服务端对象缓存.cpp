/*
���л��ͷ����л�������ƿ����ʹ�÷���˶��󻺴����������һ�ε��û���仺�棬�������ò����ٷ���
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(string, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	string Print(const string &s)
	{
		cout << "������: " << s << endl;
		return s;
	}
};

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer server((RCF::TcpEndpoint(50001)));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		// ��������
		RCF::ObjectPool &cache = RCF::getObjectPool();
		// Ϊstring����壬���岻Ҫ����10��string�����ڷ��뻺��ǰ����string.clear
		cache.enableCaching<string>(10, boost::bind(&string::clear, _1));

		string s1 = "asdfghjkl;";
		string s2;

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// ��һ�ε��û���仺�����ڴ�
		s2 = client.Print(s1);
		// ���ͬһ�����ӵ��ã�������仺��
		for (size_t i = 0; i < 100; ++i)
		{
			s2 = client.Print(s1);
		}

		// ɾ������
		cache.disableCaching<string>();

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