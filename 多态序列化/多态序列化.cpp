/*
SF ��������������������Զ����л�
1 ����ʱ��ʶ�ַ������ڶ�̬����
2 ��Ҫ֪������Ķ�̬����
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <SF/SerializeParent.hpp>
#include <SF/shared_ptr_boost.hpp>

class A
{
public:
	A() :mA(){}
	~A(){}
	void serialize(SF::Archive &ar)
	{
		ar &mA;
	}
	int mA;
};
class B : public A
{
public:
	B() :mB(){}
	void serialize(SF::Archive &ar)
	{
		SF::serializeParent<A>(ar, *this);
		ar &mB;
	}
	int mB;
};

class C : public B
{
public:
	C() :mC(){}
	void serialize(SF::Archive &ar)
	{
		SF::serializeParent<B>(ar, *this);
		ar &mC;
	}
	int mC;
};

class X
{
public:
	X() : mpA(){}
	void serialize(SF::Archive &ar)
	{
		ar &mpA;
	}

	typedef boost::shared_ptr<A> APtr;
	APtr mpA;
};

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(X, Print, const X &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	X Print(X &x)
	{
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

		// ��һ����ע������ʱ��ʶ
		SF::registerType<B>("B");
		SF::registerType<C>("C");
		// �ڶ�����ָ�����л��Ļ���
		SF::registerBaseAndDerived<A, B>();
		SF::registerBaseAndDerived<A, C>();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		X x1;
		x1.mpA = boost::shared_ptr<B>(new B());
		X x2 = client.Print(x1);

		x1.mpA = boost::shared_ptr<C>(new C());
		x2 = client.Print(x1);

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}