
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
		// �����������������Զ��ͷ�
		RCF::Lock lock(mPrintCallsMutex);
		// ����������������
		mPrintCalls.push_back(PrintCall(RCF::getCurrentRcfSession()));
		return 0;
	}

	HelloWorld()
	{
		mStopFlag = false;
		// ���������߳�
		mPrintThreadPtr.reset(new RCF::Thread(boost::bind(&HelloWorld::processPrintCalls, this)));
	}
	~HelloWorld()
	{
		// ֹͣ�첽����
		mStopFlag = true;
		// �ȴ��߳̽���
		mPrintThreadPtr->join();
	}

private:
	RCF::Mutex mPrintCallsMutex;
	deque<PrintCall> mPrintCalls;
	RCF::ThreadPtr mPrintThreadPtr;
	volatile bool mStopFlag;
	
	void processPrintCalls()
	{
		// ÿ�봦�����м��������
		while (!mStopFlag)
		{
			RCF::sleepMs(1000);
			// ��ȡ��������
			deque<PrintCall> printCalls;
			{ // �������Զ��ͷ�
				RCF::Lock lock(mPrintCallsMutex);
				printCalls.swap(mPrintCalls);
			}
			// ����
			for (size_t i = 0; i < printCalls.size(); ++i)
			{
				PrintCall &printCall = printCalls[i];
				// Print��������һ������
				const string &stringToPrint = printCall.parameters().a1.get();
				// ��ӡ����Ĳ���
				cout << "������: " << stringToPrint << endl;
				// ���÷���ֵ
				printCall.parameters().r.set(stringToPrint.size());
				// �ύ��������ֵ
				printCall.commit();
			}
		}
	}
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

	cout << "������˳�..." << endl;
	cin.get();
 
	return 0;
}