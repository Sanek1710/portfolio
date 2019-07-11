#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


namespace CLRTest {

	/// <summary>
	/// —водка дл€ MyComponent
	/// </summary>
	public ref class MyComponent :  public System::ComponentModel::Component
	{
	public:
		MyComponent(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}
		MyComponent(System::ComponentModel::IContainer ^container)
		{
			/// <summary>
			/// “ребуетс€ дл€ поддержки конструктора композиции классов Windows.Forms
			/// </summary>

			container->Add(this);
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyComponent()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			components = gcnew System::ComponentModel::Container();
		}
#pragma endregion
	};
}
