/*
�ڽ�Json-RPC��������Javascript�ɷ���C++���������ܣ�֧��HTTP,HTTPS
RCFʹ��JSON Spirit���ȡ��д��JSON�������Ҫ���غͱ��� JSON Spirit�⣬���� ���� Ԥ������ ����� RCF_FEATURE_JSON
����JSON-RPC �˵㣬I_ServerTransport::setRpcProtocol()
*/

#include <iostream>
using namespace std;

#include <RCF/RCF.hpp>
#include <RCF/JsonRpc.hpp>
#include <json_spirit.h>
#ifdef _DEBUG
#pragma comment(lib, "json_spirit_libD.lib")
#else
#pragma comment(lib, "json_spirit_lib.lib")
#endif

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

	void JsonPrint(const RCF::JsonRpcRequest &request, RCF::JsonRpcResponse &response)
	{
		int charsPrinted = 0;
		const json_spirit::Array &params = request.getJsonParams();
		for (size_t i = 0; i < params.size(); ++i)
		{
			const string &s = params[i].get_str();
			cout << "������:" << s << endl;
			charsPrinted += s.size();
		}

		json_spirit::mObject &responseObj = response.getJsonResponse();
		responseObj["result"] = charsPrinted;
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
		server.bindJsonRpc(boost::bind(&HelloWorld::JsonPrint, &helloWorld, _1, _2), "JsonPrint");
		server.addEndpoint(RCF::HttpEndpoint(80)).setRpcProtocol(RCF::Rp_JsonRpc);
		server.start();

		// RCF �ͻ��˿��Ե���Print �ڶ˿�50001
		// Json-RPC�ͻ��˿��Ե���JsonPrintͨ��HTTP��80�˿�

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