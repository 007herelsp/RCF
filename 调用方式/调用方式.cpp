/*
�ͻ�����ϸ��
ÿ���ͻ�����RCF::ClientStub������������Ե�����    client.getClientStub()
�ͻ��������������Ӧ��ʽ��	Oneway - ��������Ӧ��
						Twoway - �ͻ�����ȴ�������Ӧ��Ĭ�ϣ�����ʹ���첽����ʽ�����Ӧ��������
*/

#include <iostream>
#include <time.h>
using namespace std;

#include <RCF/RCF.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const string &)
RCF_END(I_HelloWorld)

int main()
{
	RCF::init();
	cout << "���÷������˵� Print ����" << endl;
	try {
		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));

		// ��ȡ�ͻ��˴���
		RCF::ClientStub &clientStub = client.getClientStub();

		clock_t start = clock();
		// Զ�̵���
		client.Print("����Twoway");
		clock_t end = clock();
		cout << "Twoway��ʱ:" << end - start << "ms" << endl;

		// ����Զ�̵��÷�ʽ
		// ����ֱ���ڵ���ʱָ��
		clientStub.setRemoteCallSemantics(RCF::Oneway);
		start = clock();
		client.Print("����Oneway");
		end = clock();
		cout << "Oneway��ʱ:" << end - start << "ms" << endl;

		//����д������ã������ʹ�� ��Oneway �������������ΪOneway��ÿһ�����ö��ᷢ��server������д������ã����ʱ��
		clientStub.enableBatching(); // ����Oneway������
		clientStub.setMaxBatchMessageLength(1025 * 50); // �ﵽ50K����Ϣ�Զ�����
		for (size_t i = 0; i < 100; ++i)
		{
			client.Print("����");
		}
		clientStub.flushBatch(); // �ֶ�����

		client.Print(RCF::Oneway, "����Oneway");
	}
	catch (RCF::Exception &e)
	{
		cout << "ErrorId: " << e.getErrorId() << endl; // ������
		cout << "Error: " << e.getErrorString() << endl; // ������Ϣ
	}

	cin.get();
	return 0;
}