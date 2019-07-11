#include "MyForm.h"
#include <iostream>
#include <string>
#include <msclr/marshal_cppstd.h>

using std::string;
using std::cout;
using std::endl;


using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;


namespace Win32 {
	[DllImport("kernel32.dll", CallingConvention = CallingConvention::StdCall)]
	int AllocConsole();
	[DllImport("kernel32.dll", CallingConvention = CallingConvention::StdCall)]
	int FreeConsole();
}

//#define CONSOLE



[STAThread]
void main(array<String^>^ argv) {
#ifdef CONSOLE
	Win32::AllocConsole();
#endif // CONSOLE


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	CLRTest::MyForm form; //WinFormsTest - имя вашего проекта

	Console::WriteLine("constructor fired");
	printf("fdbdfb");
	array<Process^>^ procList = Process::GetProcesses();

	Application::Run(%form);
	//Console::WriteLine("HEllo");

	
#ifdef CONSOLE
	Win32::FreeConsole();
#endif // CONSOLE
}