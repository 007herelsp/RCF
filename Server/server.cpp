/*
��Ҫ��� RCF/include   boost/include
������Ҫ���� RCF.cpp
�����win32  ��Ҫ����Ŀ���Զ��� WIN32_LEAN_AND_MEAN
*/


#include <iostream>
#include <vector>
using namespace std;

#include <RCF/RCF.hpp>
#include <SF/vector.hpp> // vector�����л�ʵ����
/************************************************************************/
// �ӿڶ���
/************************************************************************/
RCF_BEGIN(I_HelloWorld, "I_HelloWorld") // (����ʱ�ӿڱ�ʶ, ����ʱ�ӿڱ�ʶ)
	RCF_METHOD_V1(void, Print, const string &) // ����һ������������ֵΪvoid�Ľӿ�
	RCF_METHOD_R1(int, Print, const vector<string> &)// ����һ������������ֵΪstring�Ľӿ�
	RCF_METHOD_V2(void, Print, const vector<string> &, int &)
RCF_END(I_HelloWorld)
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// �Ự��Ϣ 
/************************************************************************/
class HelloWorldSession
{
public:
	HelloWorldSession(){ m_CallCount = 0; cout << "Created HelloWorldSession object." << endl; }
	~HelloWorldSession() { cout << "Destroyed HelloWorldSession object." << endl; }
	size_t m_CallCount;
};
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
// �ӿ�ʵ�� 
/************************************************************************/
class HelloWorld
{
public:
	void Print(const string &s)
	{
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		HelloWorldSession &hwSession = session.getSessionObject<HelloWorldSession>(true);
		++hwSession.m_CallCount;

		//throw runtime_error("Print() can't be called");
		cout << "������: " << s << endl;
		cout << "�ܹ����ô���: " << hwSession.m_CallCount << endl;
	}
	int Print(const vector<string> &v)
	{
		int howManyChars = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			cout << "������: " << v[i] << endl;
			howManyChars += v[i].size();
		}
		return howManyChars;
	}
	void Print(const vector<string> &v, int &howManyChars)
	{
		howManyChars = 0;
		for (size_t i = 0; i < v.size(); ++i)
		{
			cout << "������: " << v[i] << endl;
			howManyChars += v[i].size();
		}
	}
};
//////////////////////////////////////////////////////////////////////////

int main()
{
	RCF::RcfInitDeinit rcfInit;
	HelloWorld helloWorld;
	RCF::RcfServer server(RCF::TcpEndpoint(50001));
	server.bind<I_HelloWorld>(helloWorld);  // �������󶨵��ӿ�ʵ�ֶ���
	server.start();

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}