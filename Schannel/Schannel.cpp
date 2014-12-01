/*
RCF::PfxCertificate��  ��ȡ֤��  .pfx / .p12�ļ�
���ߴ�windows֤���������ȡ
RCF::StoreCertificate��
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <RCF/Win32Certificate.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const string &s)
	{
		cout << "������: " << s << endl;
	}
};

int main()
{
	try{
		RCF::init();
		RCF::RcfServer server(RCF::TcpEndpoint(50001));
		// ��ȡ֤���ļ�
		RCF::CertificatePtr serverCertPtr(new RCF::PfxCertificate(string("c:/server.p12"), _T("password"), _T("name")));
		server.setCertificate(serverCertPtr);
		// ��֤���������ȡ
		RCF::CertificatePtr serverCerPtr1(new RCF::StoreCertificate(RCF::Cl_LocalMachine, RCF::Cs_My, _T("name")));


		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		client.Print("���ǿͻ���"); 
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
	}

	cout << "������˳�..." << endl;
	cin.get();

	return 0;
}