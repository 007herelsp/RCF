
/*
	����������þ����ͳһ����
	�����󵽴ʹ���̳߳ص�һ���̻߳�ȡ�������һ���̴߳����첽����
	RCF::RemoteCallContext<> ����Զ�̵��÷���������ģ������к�����������
	RemoteCallContext::parameters() ����ÿһ�����õĲ���
	RemoteCallContext::commit() ���͵��û�Ӧ
*/

#include <iostream>
#include <deque>
using namespace std;

#include <RCF/RCF.hpp>
#include <RCF/RemoteCallContext.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
RCF_METHOD_R1(int, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	typedef RCF::RemoteCallContext<int, const std::string &> PrintCall;

	int Print(const string &s)
	{
		
	}
private:
	RCF::Mutex mPrintCallsMutex;
	deque<PrintCall> mPrintCalls;
	RCF::ThreadPtr mPrintThreadPtr;
	volatile bool mStopFlag;
};

int main()
{
	RCF::init();
	HelloWorld helloWorld;
	RCF::RcfServer server(RCF::TcpEndpoint(50001));
	server.bind<I_HelloWorld>(helloWorld);
	server.start();

	RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
	int ret = client.Print("���");
	cout << ret << endl;

	return 0;
}