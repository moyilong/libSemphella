//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace cryptutils_windows_app;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

MainPage::MainPage()
{
	InitializeComponent();
	String ^data = L"lib:";
	wchar_t buff[128];
	int len = 128;
	MultiByteToWideChar(CP_ACP, 0, get_api_ver().data(), get_api_ver().size(), buff, len);
	system_tips->Text = data + String(buff).ToString();
	
}


void cryptutils_windows_app::MainPage::execute_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (password->Text->Length() == 0)
	{
		system_tips->Text = L"请输入密码";
		return;
	}
	if (input->Text->Length() == 0)
	{
		system_tips->Text = L"请指定来源";
		return;
	}
	

}
