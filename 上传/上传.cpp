/*
�ϴ��� RCF::FileUpload ��Ҫ��ΪԶ�̷����Ĳ�����ʵ��
�����Ĭ���ǲ��������书�ܣ���Ҫ��ȷ����
���ָ��ΪĿ¼����Ŀ¼�������ļ���������
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

void onFileTransferProgress(const RCF::FileTransferProgress &progress)
{
	// ʹ��boost��Ľ�����ʾ

//	cout << "�ļ���С�� " << progress.mBytesTotalToTransfer << endl;
//	cout << "�Ѿ��ϴ��Ĵ�С: " << progress.mBytesTransferredSoFar << endl;
	//cout << "��������������: " << progress.mServerLimitBps << endl;
}

// �������˼����ϴ���Ϣ�����׳��쳣�жϴ���
void onServerFileUpload(RCF::RcfSession &session, const RCF::FileUploadInfo &uploadInfo)
{
	// �ĸ��ļ��ڴ���
	const RCF::FileManifest &fileManifest = uploadInfo.mManifest;
	// �ϴ�����
	boost::uint32_t currentFile = uploadInfo.mCurrentFile; // ��ǰ��һ���ļ�
	boost::uint64_t currentFilePos = uploadInfo.mCurrentPos; // ���ϴ��Ĵ�С
	cout << currentFile << ":" << currentFilePos << endl;
	// �׳��쳣���жϴ���
}

int main()
{
	RCF::init();
	try
	{
		RCF::RcfServer server((RCF::TcpEndpoint(50001)));
		HelloWorld helloWorld;
		server.bind<I_HelloWorld>(helloWorld);
		// Ĭ���ϴ����ļ���������Ӧ��ͬ��Ŀ¼����ָ��
		server.setFileUploadDirectory("C://RCF-Uploads");
		server.setOnFileUploadProgress(&onServerFileUpload);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		RCF::FileUpload fileUpload("E:/works/RCF/RCF.sdf");
		// �ϴ��ļ����ϴ��ڼ�������ӶϿ���������ٴε����Լ������䣬�ɿ����Զ�����
		// �������ϴ����Ȼص���ʾ 
		client.getClientStub().setFileProgressCallback(&onFileTransferProgress);
		client.getClientStub().setAutoReconnect(true);
		// �ɶ��ļ�������м���
		client.getClientStub().setTransportProtocol(RCF::Tp_Ntlm);
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