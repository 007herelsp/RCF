/*
��Ҫ��Ԥ�������ж��� RCF_USE_ZLIB
��ʹ�þ�̬�� ���趨�� RCF_ZLIB_STATIC(���Խ�������ã�
ClientStub::setEnableCompression()
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "Print������: " << s << endl;
		// ��ȡ��ǰ���ӵĻỰ
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		// ���ͺ��յ����ֽ���
		boost::uint64_t totalBytesReceived = session.getTotalBytesReceived();
		cout << totalBytesReceived << endl;
		boost::uint64_t totalBytesSent = session.getTotalBytesSent();
		cout << totalBytesSent << endl;
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
		client.getClientStub().setTransportProtocol(RCF::Tp_Ntlm);
		client.Print("δѹ��");
		client.getClientStub().setEnableCompression(true);
		client.Print("ѹ��");

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}