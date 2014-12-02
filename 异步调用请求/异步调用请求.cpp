

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(int, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	int Print(const string &s)
	{
		cout << "������: " << s << endl;
		RCF::sleepMs(10000);
		//throw runtime_error("�첽���ô���");
		return s.size();
	}
};

// ���������߳����жϵȴ��첽���ý���
typedef boost::shared_ptr<RcfClient<I_HelloWorld> > HelloWorldPtr;
void onPrintCompleted(RCF::Future<int> fRet, HelloWorldPtr clientPtr)
{
	std::auto_ptr<RCF::Exception> ePtr = clientPtr->getClientStub().getAsyncException();
	if (ePtr.get())
	{
		// �д���
		cout << ePtr->getErrorString() << endl;
	}
	else
	{
		cout << __FUNCTION__ << *fRet << endl;
	}
}

int main()
{
	RCF::init();

	RCF::RcfServer server(RCF::TcpEndpoint(50001));
	HelloWorld helloWorld;
	server.bind<I_HelloWorld>(helloWorld);
	server.start();

	//RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
	RCF::Future<int> fRet; // �첽���÷���ֵ
	// �����첽����
//	fRet = client.Print("����");

	// �ص�ģʽ�첽����
	HelloWorldPtr clientPtr(new RcfClient<I_HelloWorld>(RCF::TcpEndpoint(50001)));
	//fRet = clientPtr->Print(RCF::AsyncTwoway(boost::bind(&onPrintCompleted, fRet, clientPtr)), "����");
	fRet = clientPtr->Print("hehe");
	// �ȴ��������
	fRet.wait();
// 	while (!fRet.ready())
// 	{
// 		RCF::sleepMs(10);
// 		cout << "��..." << endl;
// 	}
	// ������
// 	std::auto_ptr<RCF::Exception> ePtr = client.getClientStub().getAsyncException();
// 	if (ePtr.get())
// 	{
// 		// �д���
// 		cout << ePtr->getErrorString() << endl;
// 	}
// 	else
// 	{
// 		cout << *fRet << endl;
// 	}


	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}