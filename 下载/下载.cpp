/*
���أ� RCF::FileDownload ��Ҫ��ΪԶ�̷����Ĳ�����ʵ��
�����Ĭ���ǲ��������书�ܣ���Ҫ��ȷ����
���ָ��ΪĿ¼����Ŀ¼�������ļ���������
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
		fileDownload = RCF::FileDownload("E:/works/RCF/RCF.sdf");
	}
};

void onFileTransferProgress(const RCF::FileTransferProgress &progress)
{
	// ʹ��boost��Ľ�����ʾ

	//cout << "�ļ���С�� " << progress.mBytesTotalToTransfer << endl;
	//cout << "�Ѿ����صĴ�С: " << progress.mBytesTransferredSoFar << endl;
	//cout << "��������������: " << progress.mServerLimitBps << endl;
}

// �������˼���������Ϣ�����׳��쳣�жϴ���
void onServerFileDown(RCF::RcfSession &session, const RCF::FileDownloadInfo &downloadInfo)
{
	// �ĸ��ļ��ڴ���
	const RCF::FileManifest &fileManifest = downloadInfo.mManifest;
	// �������
	boost::uint32_t currentFile = downloadInfo.mCurrentFile; // ��ǰ��һ���ļ�
	boost::uint64_t currentFilePos = downloadInfo.mCurrentPos; // ���ϴ��Ĵ�С
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
		server.setOnFileDownloadProgress(&onServerFileDown);
		server.start();

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		RCF::FileDownload fileDownload(".");
		// �����ļ���������ع����жϿ����ӣ�������ٴε����Լ������أ�
		client.getClientStub().setAutoReconnect(true);
		// ���������ؽ��Ȼص���ʾ 
		client.getClientStub().setFileProgressCallback(&onFileTransferProgress);
		// �ɶ��ļ�������м���
		client.getClientStub().setTransportProtocol(RCF::Tp_Ntlm);
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