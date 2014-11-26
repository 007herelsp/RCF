/*
���أ� RCF::FileDownload ��Ҫ��ΪԶ�̷����Ĳ�����ʵ��
�����Ĭ���ǲ��������书�ܣ���Ҫ��ȷ����
*/

#include <iostream>
using namespace std;

//#define RCF_USE_BOOST_FILESYSTEM // �������ع�����Ҫ������ Ԥ������������  
// ��Ҫ���ȱ���filesystem�� boost filesystem��֧��
#include <RCF/RCF.hpp>
#include <RCF/FileDownload.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V2(void, PrintAndDownload, const string &, RCF::FileDownload) 
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void PrintAndDownload(const string &s, RCF::FileDownload fileDownload)
	{
		cout << "������: " << s << endl;
		fileDownload = RCF::FileDownload("E:/works/RCF/Client/client.cpp");
	}
};

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer server((RCF::TcpEndpoint(50001)));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		RCF::FileDownload fileDownload(".");
		// �����ļ�
		client.PrintAndDownload("wang", fileDownload);
		// ��ȡ�����ļ����·��
		string pathToDownload = fileDownload.getLocalPath();
		// ��ȡ�ļ�
		RCF::FileManifest &downloadManifest = fileDownload.getManifest();
		cout << "�����ļ����·����" << pathToDownload << endl;
		cout << "�ļ�������" << downloadManifest.mFiles.size() << endl;
		cout << "��һ���ļ���С��" << downloadManifest.mFiles[0].mFileSize << endl;

		cout << "������˳�..." << endl;
		cin.get();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		cout << "������˳�..." << endl;
		cin.get();
	}

	return 0;
}