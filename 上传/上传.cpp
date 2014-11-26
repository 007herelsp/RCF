/*
�ϴ��� RCF::FileUpload ��Ҫ��ΪԶ�̷����Ĳ�����ʵ��
�����Ĭ���ǲ��������书�ܣ���Ҫ��ȷ����
*/

#include <iostream>
using namespace std;

//#define RCF_USE_BOOST_FILESYSTEM // �������ع�����Ҫ������ Ԥ������������  
// ��Ҫ���ȱ���filesystem�� boost filesystem��֧��
#include <RCF/RCF.hpp>
#include <RCF/FileUpload.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V2(void, PrintAndUpload, const string &, RCF::FileUpload)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void PrintAndUpload(const string &s, RCF::FileUpload fileUpload)
	{
		cout << "������: " << s << endl;
		string pathToUpload = fileUpload.getLocalPath();
		RCF::FileManifest &uploadManifest = fileUpload.getManifest();
		cout << "�ϴ�·��:" << pathToUpload << endl;
		cout << "�ļ�����:" << uploadManifest.mFiles.size() << endl;
		cout << "��һ���ļ���С: " << uploadManifest.mFiles[0].mFileSize << endl;
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
		RCF::FileUpload fileUpload("E:/works/RCF/RCF.sdf");
		// �ϴ��ļ�
		client.PrintAndUpload("wang", fileUpload);

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