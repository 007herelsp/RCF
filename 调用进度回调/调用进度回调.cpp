/*
����Twoway���õĽ��ȵĻص���Ӧ�ã�չʾ���������ػ洰�ڣ���ȡ������
������ó�ʱ����ᴥ���ص�
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

void onRemoteCallProgress()
{
	cout << "���Ȼص�" << endl;
}

int main()
{
	RCF::init();
	cout << "���÷������˵� Print ����" << endl;
	try {
		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));

		// ��ȡ�ͻ��˴���
		RCF::ClientStub &clientStub = client.getClientStub();
		// ���ý��ȵĻص�
		clientStub.setRemoteCallProgressCallback(boost::bind(&onRemoteCallProgress), 500);// ÿ500ms�ص�һ��

		client.Print("���ǿͻ���1");
	}
	catch (RCF::Exception &e)
	{
		cout << "ErrorId: " << e.getErrorId() << endl; // ������
		cout << "Error: " << e.getErrorString() << endl; // ������Ϣ
	}

	cin.get();
	return 0;
}