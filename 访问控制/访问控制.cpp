/*
���ʿ���ʹ���û��Զ���ص����������ھ����ͻ����Ƿ��ܷ���ʵ����
ÿһ���ͻ��˳��Ե��÷������з������ص����������������Ա�����֤״̬�Ա���������
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

class LoginInfo
{
public:

	void serialize(SF::Archive &ar)
	{

	}
};

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Login, const LoginInfo &)
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorldAuthenticationState
{
public:
	HelloWorldAuthenticationState()
		: mAuthenticated(false)
	{

	}

	bool mAuthenticated;
	RCF::tstring mClientUsername;
	LoginInfo mLoginInfo;
};


class HelloWorld
{
public:
	void Login(const LoginInfo &loginInfo)
	{
		cout << "Login" << endl;
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		// ����������򴴽�
		HelloWorldAuthenticationState &authState = session.getSessionObject<HelloWorldAuthenticationState>(true);
		if (!authState.mAuthenticated)
		{
			RCF::TransportProtocol tp = session.getTransportProtocol();
			if (tp == RCF::Tp_Ntlm || tp == RCF::Tp_Kerberos)
			{
				authState.mAuthenticated = true;
				authState.mClientUsername = session.getClientUsername();
				authState.mLoginInfo = loginInfo;
			}
		}
	}

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

bool onHelloWorldAccess(int dispatchId)
{
	cout << "DispatchId: " << dispatchId << endl;
	if (dispatchId == 0)
	{
		return true;
	}
	else
	{
		RCF::RcfSession &session = RCF::getCurrentRcfSession();
		HelloWorldAuthenticationState &authState = session.getSessionObject<HelloWorldAuthenticationState>(true);
		return authState.mAuthenticated;
	}
}

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
		// ��ȡ�󶨶���
		RCF::ServerBindingPtr bindPtr = server.bind<I_HelloWorld>(helloWorld);
		bindPtr->setAccessControl(boost::bind(&onHelloWorldAccess, _1));
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		client.getClientStub().setTransportProtocol(RCF::Tp_Ntlm);
		client.Login(LoginInfo());
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