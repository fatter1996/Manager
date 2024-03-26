#pragma once

#include <set>
#include <string>
#include <vector>

#define MachineFault 0x11
#define TimeLimitFault 0x22
#define Sucess 0x33
#ifdef MYPROTECT_EXPORTS
#define MYPROTECT_API __declspec(dllexport)
#else
#define MYPROTECT_API __declspec(dllimport)
#endif

template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
template class __declspec(dllexport)  std::vector<std::string, std::allocator<std::string>>;
class MYPROTECT_API CMyProtect
{
public:
	CMyProtect();
	~CMyProtect();

	// 获取机器的软件运行许可
	// 返回值 BOOL 是否许可	
	long int GetPermissionOfMachine();
	//添加设备数组
	bool insert_machine(const std::string& cur_machine);
	void AddLoggerCloseTime(); //软件关闭记录时间

private:
	void InsetLocalMachineCode();
	bool SerchLocal2MACAdress();
	std::string ReadMachineInfo();
	std::string Encrypt(std::string mStr);
	std::string GetCPUInfo();
	std::string GetBoardInfo();
	bool bMatchMachineInfo(const std::string& toolMachCode);
	long int TimePermitFun();
	void LoggerFileFun(int type);

	std::string utf8_to_gb2312(const std::string& str_utf8) const;

private:
	std::set<std::string> cur_machines_;

	std::string m_strCPU_ = std::string("");
	std::string m_strBoard_ = std::string("");
	std::vector<std::string> m_arrMAC_; //MAC地址数组
	std::vector<std::string> m_arrMACAll_; //MAC地址数组All
	std::string m_strMchCode_ = std::string(""); //设备号
};