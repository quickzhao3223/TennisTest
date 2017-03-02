// Test.cpp : 定义控制台应用程序的入口点。
//
#include "../I_Bout.h"

int main(int argc, char* argv[])
{
	
	INIT_PLUG

	I_Bout* i = NEW(Bout);
	i->test();
	DEL(i);
	
	//智能指针方式
	//std::shared_ptr<I_Bout> sp = NEWSP(Bout);

	return 0;
}

