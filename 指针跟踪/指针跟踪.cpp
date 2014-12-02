/*
SF ���л�ָ��ָ��Ķ����ϵ���� ���л�ָ����ͬ����ʱ��ָ��ָ����ǲ�ͬ�Ķ���
���ϣ��ָ��ָ��ͬһ����������Ҫʹ��ָ����ٹ���
������нϴ��������ʧ����Ϊ��Ҫ��������ָ��
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <SF/shared_ptr_boost.hpp>
#include <SF/vector.hpp>

typedef vector<boost::shared_ptr<int> > TwoPointers;
RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(TwoPointers, Print, const TwoPointers &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	TwoPointers Print(TwoPointers &x)
	{
		// ����ָ�����
		RCF::getCurrentRcfSession().setEnableSfPointerTracking(true);
		cout << "�����ڼ䣺 " << x[0] << ":" << x[1] << endl;
		return x;
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
		// ����ָ����٣���������յ�����Ȼ��Ҫ����
		client.getClientStub().setEnableSfPointerTracking(true);
		boost::shared_ptr<int> spn1(new int(1));
		boost::shared_ptr<int> spn2(spn1);
		cout << "�������ڣ� " << spn1 << ":" << spn2 << endl;
		TwoPointers tw;
		tw.push_back(spn1);
		tw.push_back(spn2);
		cout << "���������� " << tw[0] << ":" << tw[1] << endl;
		TwoPointers ret = client.Print(tw);
		cout << "���ù��� " << ret[0] << ":" << ret[1] << endl;

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}