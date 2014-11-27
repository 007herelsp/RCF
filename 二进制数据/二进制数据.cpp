/*
����Ҫ���ʹ����Ķ���������ʱ��ʹ��RCF::ByteBuffer �� string �� vector�����л����м�����Ч���ر��������˶�û�����ݵĿ���
*/


#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

#include <RCF/RCF.hpp>
#include <SF/vector.hpp>

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_R1(RCF::ByteBuffer, Print, RCF::ByteBuffer)
	RCF_METHOD_R1(vector<char>, Print, const vector<char> &)
	RCF_METHOD_R1(string, Print, const string &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	RCF::ByteBuffer Print(RCF::ByteBuffer byteBuffer)
	{
		cout << "Print������: " << endl;
		return byteBuffer;
	}
	vector<char> Print(vector<char> &verChar)
	{
		cout << "Print������: " << endl;
		return verChar;
	}
	string Print(string &str)
	{
		cout << "Print������: " << endl;
		return str;
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

		RCF::setDefaultMaxMessageLength(600 * 1024); // 600k

		RcfClient<I_HelloWorld> client(RCF::TcpEndpoint(50001));
		// ����500k������
		RCF::ByteBuffer byteBuffer(500 * 1024);
		vector<char> vecChar;
		string str;
		for (size_t i = 0; i < byteBuffer.getLength(); ++i)
		{
			char ch = char(i % 256);
			byteBuffer.getPtr()[i] = ch;
			vecChar.push_back(ch);
			str.push_back(ch);
		}
		clock_t start = clock();
		RCF::ByteBuffer b = client.Print(byteBuffer);
		clock_t end = clock();
		cout << "RCF::ByteBuffer��ʱ:" << end - start << "ms" << endl;

		start = clock();
		vector<char> v = client.Print(vecChar);
		end = clock();
		cout << "vector��ʱ:" << end - start << "ms" << endl;

		start = clock();
		string s = client.Print(str);
		end = clock();
		cout << "string��ʱ:" << end - start << "ms" << endl;

		close();
	}
	catch (RCF::Exception &e)
	{
		cout << e.getErrorString() << endl;
		close();
	}


	return 0;
}