/*
��� protobuf   google�Ŀ�Դ���ݽ�����ʽ   https://code.google.com/p/protobuf/
���� .proto�ļ���protoc������ļ��������ļ������protoc, �Ϳ��ļ�libprotobuf.lib ע����ҪDebug��Release�汾��Ӧ)
// person.proto
message Person
{
	required int32 id = 1;
	required string name = 2;
	optional string email = 3;
}

message PbEmpty
{
	optional string log = 1;
}

protoc Person.proto --cpp_out=.   (����.cc �� .h�ļ���

��Ҫ������ Ԥ���������� RCF_USE_PROTOBUF
vs2013����� error C4996: 'std::_Copy_impl' ����  ��Ҫ��C/C++ ������ - ����������� -D_SCL_SECURE_NO_WARNINGS
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include "person.pb.h"
#ifdef _DEBUG
#pragma comment(lib, "libprotobufD.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#endif

RCF_BEGIN(I_HelloWorld, "I_HelloWorld")
	RCF_METHOD_V1(void, Print, const Person &)
RCF_END(I_HelloWorld)

class HelloWorld
{
public:
	void Print(const Person &person)
	{
		cout << "������: " << endl;
		cout << person.name() << endl;
		cout << person.email() << endl;
		cout << person.id() << endl;
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
		
		Person person;
		person.set_name("abd"); // ��������ģ���Ҫת��ΪUTF-8
		person.set_id(110);
		person.set_email("hey-xiaohao@163.com");
		client.Print(person);

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

