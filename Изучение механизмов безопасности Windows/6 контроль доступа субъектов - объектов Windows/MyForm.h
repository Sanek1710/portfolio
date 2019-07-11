#pragma once

#include <windows.h>
#include <string>
#include "ProcessList.h"
#include <msclr\marshal_cppstd.h>


namespace CLRTest {

	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System;
	using namespace System::IO;
	using namespace System::Text;
	using namespace System::Diagnostics;
	using namespace System::Runtime::InteropServices;

	using namespace ProcManeger;

	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_Refresh;
	protected:

	private: System::Windows::Forms::TreeView^  treeView_Proc;
	protected:



	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;

	private: array<Process^>^ procList;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox_IntegrityLevel;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;


	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::CheckBox^  ch_ASLR_BottomUpRandom;
	private: System::Windows::Forms::CheckBox^  ch_ASLR_ForceRelocate;
	private: System::Windows::Forms::CheckBox^  ch_ASLR_HighEntropy;
	private: System::Windows::Forms::CheckBox^  ch_DEP_Enable;




	private: System::Windows::Forms::CheckedListBox^  chListBox_Priveleges;

	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;

	private: System::Windows::Forms::Label^  lbl_ProcPath;
	private: System::Windows::Forms::Label^  lbl_ProcType;


	private: System::Windows::Forms::Label^  lbl_ProcName;
	private: System::Windows::Forms::Label^  lbl_ProcId;
	private: System::Windows::Forms::ListBox^  listBox_DLL;



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  lbl_ParentId;
	private: System::Windows::Forms::Label^  lbl_OwnerSid;
	private: System::Windows::Forms::Label^  lbl_ParentName;
	private: System::Windows::Forms::Label^  lbl_OwnerName;
	private: System::Windows::Forms::Label^  label12;


	private: HANDLE hCurProc;
	private: HANDLE tCurProc;
	private: String^ CurObjectName;
	private: System::Windows::Forms::Button^  btn_browseObject;

	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::ComboBox^  comboBox_objectType;
	private: System::Windows::Forms::Label^  lbl_ObjectOwnerSid;
	private: System::Windows::Forms::Label^  lbl_ObjectOwnerName;

	private: System::Windows::Forms::Label^  lbl_ObjectPath;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::ComboBox^  comboBox_ObjectIntegrityLevel;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::ListBox^  listBox_RuleMask;
private: System::Windows::Forms::ListBox^  listBox_RuleUser;
private: System::Windows::Forms::Label^  lbl_MaskType;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::Panel^  panel6;
private: System::Windows::Forms::Panel^  panel5;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel5;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel4;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel6;
private: System::Windows::Forms::Button^  btm_FindProc;
private: System::Windows::Forms::TextBox^  textBox_NameProcToFind;






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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->btn_Refresh = (gcnew System::Windows::Forms::Button());
			this->treeView_Proc = (gcnew System::Windows::Forms::TreeView());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->lbl_OwnerSid = (gcnew System::Windows::Forms::Label());
			this->listBox_DLL = (gcnew System::Windows::Forms::ListBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->lbl_OwnerName = (gcnew System::Windows::Forms::Label());
			this->lbl_ProcPath = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->lbl_ProcName = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lbl_ProcId = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->ch_ASLR_HighEntropy = (gcnew System::Windows::Forms::CheckBox());
			this->ch_ASLR_ForceRelocate = (gcnew System::Windows::Forms::CheckBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->ch_ASLR_BottomUpRandom = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->lbl_ParentId = (gcnew System::Windows::Forms::Label());
			this->chListBox_Priveleges = (gcnew System::Windows::Forms::CheckedListBox());
			this->lbl_ParentName = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->comboBox_IntegrityLevel = (gcnew System::Windows::Forms::ComboBox());
			this->lbl_ProcType = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ch_DEP_Enable = (gcnew System::Windows::Forms::CheckBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->tableLayoutPanel5 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->listBox_RuleMask = (gcnew System::Windows::Forms::ListBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->lbl_ObjectOwnerSid = (gcnew System::Windows::Forms::Label());
			this->lbl_MaskType = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->comboBox_ObjectIntegrityLevel = (gcnew System::Windows::Forms::ComboBox());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->comboBox_objectType = (gcnew System::Windows::Forms::ComboBox());
			this->listBox_RuleUser = (gcnew System::Windows::Forms::ListBox());
			this->btn_browseObject = (gcnew System::Windows::Forms::Button());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->lbl_ObjectOwnerName = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->lbl_ObjectPath = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel6 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->btm_FindProc = (gcnew System::Windows::Forms::Button());
			this->textBox_NameProcToFind = (gcnew System::Windows::Forms::TextBox());
			this->panel1->SuspendLayout();
			this->tableLayoutPanel3->SuspendLayout();
			this->panel4->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			this->tableLayoutPanel5->SuspendLayout();
			this->panel6->SuspendLayout();
			this->panel5->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel2->SuspendLayout();
			this->tableLayoutPanel4->SuspendLayout();
			this->tableLayoutPanel6->SuspendLayout();
			this->SuspendLayout();
			// 
			// btn_Refresh
			// 
			this->btn_Refresh->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btn_Refresh->Location = System::Drawing::Point(3, 668);
			this->btn_Refresh->Name = L"btn_Refresh";
			this->btn_Refresh->Size = System::Drawing::Size(292, 52);
			this->btn_Refresh->TabIndex = 0;
			this->btn_Refresh->Text = L" уакуыр";
			this->btn_Refresh->UseVisualStyleBackColor = true;
			this->btn_Refresh->Click += gcnew System::EventHandler(this, &MyForm::btn_Refresh_Click);
			// 
			// treeView_Proc
			// 
			this->treeView_Proc->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->treeView_Proc->Location = System::Drawing::Point(3, 32);
			this->treeView_Proc->Name = L"treeView_Proc";
			this->treeView_Proc->Size = System::Drawing::Size(292, 630);
			this->treeView_Proc->TabIndex = 2;
			this->treeView_Proc->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MyForm::treeView1_AfterSelect);
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->tableLayoutPanel3);
			this->panel1->Location = System::Drawing::Point(3, 3);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(901, 405);
			this->panel1->TabIndex = 4;
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel3->ColumnCount = 2;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel3->Controls->Add(this->panel4, 0, 0);
			this->tableLayoutPanel3->Controls->Add(this->panel3, 1, 0);
			this->tableLayoutPanel3->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel3->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RowCount = 1;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 17.57576F)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(899, 403);
			this->tableLayoutPanel3->TabIndex = 0;
			// 
			// panel4
			// 
			this->panel4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel4->Controls->Add(this->lbl_OwnerSid);
			this->panel4->Controls->Add(this->listBox_DLL);
			this->panel4->Controls->Add(this->label7);
			this->panel4->Controls->Add(this->lbl_OwnerName);
			this->panel4->Controls->Add(this->lbl_ProcPath);
			this->panel4->Controls->Add(this->label2);
			this->panel4->Controls->Add(this->lbl_ProcName);
			this->panel4->Controls->Add(this->label4);
			this->panel4->Controls->Add(this->lbl_ProcId);
			this->panel4->Controls->Add(this->label13);
			this->panel4->Controls->Add(this->label12);
			this->panel4->Controls->Add(this->ch_ASLR_HighEntropy);
			this->panel4->Controls->Add(this->ch_ASLR_ForceRelocate);
			this->panel4->Controls->Add(this->label8);
			this->panel4->Controls->Add(this->label11);
			this->panel4->Controls->Add(this->ch_ASLR_BottomUpRandom);
			this->panel4->Controls->Add(this->label1);
			this->panel4->Location = System::Drawing::Point(3, 3);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(443, 397);
			this->panel4->TabIndex = 3;
			// 
			// lbl_OwnerSid
			// 
			this->lbl_OwnerSid->AutoSize = true;
			this->lbl_OwnerSid->Location = System::Drawing::Point(120, 89);
			this->lbl_OwnerSid->Name = L"lbl_OwnerSid";
			this->lbl_OwnerSid->Size = System::Drawing::Size(20, 17);
			this->lbl_OwnerSid->TabIndex = 15;
			this->lbl_OwnerSid->Text = L"...";
			// 
			// listBox_DLL
			// 
			this->listBox_DLL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->listBox_DLL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->listBox_DLL->FormattingEnabled = true;
			this->listBox_DLL->ItemHeight = 18;
			this->listBox_DLL->Location = System::Drawing::Point(6, 243);
			this->listBox_DLL->Name = L"listBox_DLL";
			this->listBox_DLL->Size = System::Drawing::Size(431, 148);
			this->listBox_DLL->TabIndex = 7;
			// 
			// label7
			// 
			this->label7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 20);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(25, 17);
			this->label7->TabIndex = 6;
			this->label7->Text = L"ID:";
			// 
			// lbl_OwnerName
			// 
			this->lbl_OwnerName->AutoSize = true;
			this->lbl_OwnerName->Location = System::Drawing::Point(120, 66);
			this->lbl_OwnerName->Name = L"lbl_OwnerName";
			this->lbl_OwnerName->Size = System::Drawing::Size(20, 17);
			this->lbl_OwnerName->TabIndex = 15;
			this->lbl_OwnerName->Text = L"...";
			// 
			// lbl_ProcPath
			// 
			this->lbl_ProcPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ProcPath->AutoSize = true;
			this->lbl_ProcPath->Location = System::Drawing::Point(65, 43);
			this->lbl_ProcPath->Name = L"lbl_ProcPath";
			this->lbl_ProcPath->Size = System::Drawing::Size(20, 17);
			this->lbl_ProcPath->TabIndex = 14;
			this->lbl_ProcPath->Text = L"...";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(143, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(49, 17);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Name:";
			// 
			// lbl_ProcName
			// 
			this->lbl_ProcName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ProcName->AutoSize = true;
			this->lbl_ProcName->Location = System::Drawing::Point(198, 20);
			this->lbl_ProcName->Name = L"lbl_ProcName";
			this->lbl_ProcName->Size = System::Drawing::Size(20, 17);
			this->lbl_ProcName->TabIndex = 14;
			this->lbl_ProcName->Text = L"...";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 112);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(48, 17);
			this->label4->TabIndex = 8;
			this->label4->Text = L"ASLR:";
			// 
			// lbl_ProcId
			// 
			this->lbl_ProcId->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ProcId->AutoSize = true;
			this->lbl_ProcId->Location = System::Drawing::Point(65, 20);
			this->lbl_ProcId->Name = L"lbl_ProcId";
			this->lbl_ProcId->Size = System::Drawing::Size(20, 17);
			this->lbl_ProcId->TabIndex = 14;
			this->lbl_ProcId->Text = L"...";
			// 
			// label13
			// 
			this->label13->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(80, 89);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(34, 17);
			this->label13->TabIndex = 6;
			this->label13->Text = L"SID:";
			// 
			// label12
			// 
			this->label12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(65, 66);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(49, 17);
			this->label12->TabIndex = 5;
			this->label12->Text = L"Name:";
			// 
			// ch_ASLR_HighEntropy
			// 
			this->ch_ASLR_HighEntropy->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ch_ASLR_HighEntropy->AutoSize = true;
			this->ch_ASLR_HighEntropy->Location = System::Drawing::Point(6, 189);
			this->ch_ASLR_HighEntropy->Name = L"ch_ASLR_HighEntropy";
			this->ch_ASLR_HighEntropy->Size = System::Drawing::Size(108, 21);
			this->ch_ASLR_HighEntropy->TabIndex = 11;
			this->ch_ASLR_HighEntropy->Text = L"HighEntropy";
			this->ch_ASLR_HighEntropy->UseVisualStyleBackColor = true;
			// 
			// ch_ASLR_ForceRelocate
			// 
			this->ch_ASLR_ForceRelocate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ch_ASLR_ForceRelocate->AutoSize = true;
			this->ch_ASLR_ForceRelocate->Location = System::Drawing::Point(6, 162);
			this->ch_ASLR_ForceRelocate->Name = L"ch_ASLR_ForceRelocate";
			this->ch_ASLR_ForceRelocate->Size = System::Drawing::Size(167, 21);
			this->ch_ASLR_ForceRelocate->TabIndex = 11;
			this->ch_ASLR_ForceRelocate->Text = L"ForceRelocateImages";
			this->ch_ASLR_ForceRelocate->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(6, 43);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(41, 17);
			this->label8->TabIndex = 7;
			this->label8->Text = L"Path:";
			// 
			// label11
			// 
			this->label11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(6, 66);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(53, 17);
			this->label11->TabIndex = 10;
			this->label11->Text = L"Owner:";
			// 
			// ch_ASLR_BottomUpRandom
			// 
			this->ch_ASLR_BottomUpRandom->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ch_ASLR_BottomUpRandom->AutoSize = true;
			this->ch_ASLR_BottomUpRandom->Location = System::Drawing::Point(6, 135);
			this->ch_ASLR_BottomUpRandom->Name = L"ch_ASLR_BottomUpRandom";
			this->ch_ASLR_BottomUpRandom->Size = System::Drawing::Size(186, 21);
			this->ch_ASLR_BottomUpRandom->TabIndex = 11;
			this->ch_ASLR_BottomUpRandom->Text = L"BottomUpRandomization";
			this->ch_ASLR_BottomUpRandom->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 218);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 17);
			this->label1->TabIndex = 4;
			this->label1->Text = L"DLL:";
			// 
			// panel3
			// 
			this->panel3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel3->Controls->Add(this->lbl_ParentId);
			this->panel3->Controls->Add(this->chListBox_Priveleges);
			this->panel3->Controls->Add(this->lbl_ParentName);
			this->panel3->Controls->Add(this->label6);
			this->panel3->Controls->Add(this->comboBox_IntegrityLevel);
			this->panel3->Controls->Add(this->lbl_ProcType);
			this->panel3->Controls->Add(this->label9);
			this->panel3->Controls->Add(this->label3);
			this->panel3->Controls->Add(this->ch_DEP_Enable);
			this->panel3->Controls->Add(this->label14);
			this->panel3->Controls->Add(this->label15);
			this->panel3->Controls->Add(this->label5);
			this->panel3->Controls->Add(this->label10);
			this->panel3->Location = System::Drawing::Point(452, 3);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(444, 397);
			this->panel3->TabIndex = 2;
			// 
			// lbl_ParentId
			// 
			this->lbl_ParentId->AutoSize = true;
			this->lbl_ParentId->Location = System::Drawing::Point(123, 89);
			this->lbl_ParentId->Name = L"lbl_ParentId";
			this->lbl_ParentId->Size = System::Drawing::Size(20, 17);
			this->lbl_ParentId->TabIndex = 15;
			this->lbl_ParentId->Text = L"...";
			// 
			// chListBox_Priveleges
			// 
			this->chListBox_Priveleges->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->chListBox_Priveleges->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->chListBox_Priveleges->FormattingEnabled = true;
			this->chListBox_Priveleges->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L" " });
			this->chListBox_Priveleges->Location = System::Drawing::Point(6, 243);
			this->chListBox_Priveleges->Name = L"chListBox_Priveleges";
			this->chListBox_Priveleges->Size = System::Drawing::Size(435, 148);
			this->chListBox_Priveleges->TabIndex = 13;
			this->chListBox_Priveleges->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::checkedListBox1_SelectedIndexChanged);
			// 
			// lbl_ParentName
			// 
			this->lbl_ParentName->AutoSize = true;
			this->lbl_ParentName->Location = System::Drawing::Point(123, 66);
			this->lbl_ParentName->Name = L"lbl_ParentName";
			this->lbl_ParentName->Size = System::Drawing::Size(20, 17);
			this->lbl_ParentName->TabIndex = 15;
			this->lbl_ParentName->Text = L"...";
			// 
			// label6
			// 
			this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(8, 218);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(78, 17);
			this->label6->TabIndex = 10;
			this->label6->Text = L"Priveleges:";
			// 
			// comboBox_IntegrityLevel
			// 
			this->comboBox_IntegrityLevel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->comboBox_IntegrityLevel->FormattingEnabled = true;
			this->comboBox_IntegrityLevel->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"Low", L"Medium", L"High", L"System" });
			this->comboBox_IntegrityLevel->Location = System::Drawing::Point(114, 39);
			this->comboBox_IntegrityLevel->Name = L"comboBox_IntegrityLevel";
			this->comboBox_IntegrityLevel->Size = System::Drawing::Size(135, 24);
			this->comboBox_IntegrityLevel->TabIndex = 6;
			this->comboBox_IntegrityLevel->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox_IntegrityLevel_SelectedIndexChanged);
			// 
			// lbl_ProcType
			// 
			this->lbl_ProcType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ProcType->AutoSize = true;
			this->lbl_ProcType->Location = System::Drawing::Point(58, 20);
			this->lbl_ProcType->Name = L"lbl_ProcType";
			this->lbl_ProcType->Size = System::Drawing::Size(20, 17);
			this->lbl_ProcType->TabIndex = 14;
			this->lbl_ProcType->Text = L"...";
			// 
			// label9
			// 
			this->label9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(8, 20);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(44, 17);
			this->label9->TabIndex = 8;
			this->label9->Text = L"Type:";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(8, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(100, 17);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Integrity Level:";
			// 
			// ch_DEP_Enable
			// 
			this->ch_DEP_Enable->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ch_DEP_Enable->AutoSize = true;
			this->ch_DEP_Enable->Location = System::Drawing::Point(8, 135);
			this->ch_DEP_Enable->Name = L"ch_DEP_Enable";
			this->ch_DEP_Enable->Size = System::Drawing::Size(74, 21);
			this->ch_DEP_Enable->TabIndex = 12;
			this->ch_DEP_Enable->Text = L"Enable";
			this->ch_DEP_Enable->UseVisualStyleBackColor = true;
			// 
			// label14
			// 
			this->label14->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(68, 66);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(49, 17);
			this->label14->TabIndex = 5;
			this->label14->Text = L"Name:";
			// 
			// label15
			// 
			this->label15->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(92, 89);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(25, 17);
			this->label15->TabIndex = 6;
			this->label15->Text = L"ID:";
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(8, 112);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(40, 17);
			this->label5->TabIndex = 9;
			this->label5->Text = L"DEP:";
			// 
			// label10
			// 
			this->label10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(8, 66);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(54, 17);
			this->label10->TabIndex = 9;
			this->label10->Text = L"Parent:";
			// 
			// label20
			// 
			this->label20->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label20->Location = System::Drawing::Point(256, 265);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(166, 25);
			this->label20->TabIndex = 1;
			this->label20->Text = L"Sanek и Dimon ©";
			// 
			// panel2
			// 
			this->panel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel2->Controls->Add(this->tableLayoutPanel5);
			this->panel2->Location = System::Drawing::Point(3, 414);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(901, 312);
			this->panel2->TabIndex = 5;
			// 
			// tableLayoutPanel5
			// 
			this->tableLayoutPanel5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel5->ColumnCount = 2;
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel5->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel5->Controls->Add(this->panel6, 1, 0);
			this->tableLayoutPanel5->Controls->Add(this->panel5, 0, 0);
			this->tableLayoutPanel5->Location = System::Drawing::Point(4, 5);
			this->tableLayoutPanel5->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel5->Name = L"tableLayoutPanel5";
			this->tableLayoutPanel5->RowCount = 1;
			this->tableLayoutPanel5->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel5->Size = System::Drawing::Size(893, 303);
			this->tableLayoutPanel5->TabIndex = 26;
			// 
			// panel6
			// 
			this->panel6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel6->Controls->Add(this->label20);
			this->panel6->Controls->Add(this->label17);
			this->panel6->Controls->Add(this->label19);
			this->panel6->Controls->Add(this->listBox_RuleMask);
			this->panel6->Controls->Add(this->label21);
			this->panel6->Controls->Add(this->lbl_ObjectOwnerSid);
			this->panel6->Controls->Add(this->lbl_MaskType);
			this->panel6->Controls->Add(this->label22);
			this->panel6->Controls->Add(this->comboBox_ObjectIntegrityLevel);
			this->panel6->Location = System::Drawing::Point(449, 3);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(441, 297);
			this->panel6->TabIndex = 26;
			// 
			// label17
			// 
			this->label17->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(6, 22);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(100, 17);
			this->label17->TabIndex = 7;
			this->label17->Text = L"Integrity Level:";
			// 
			// label19
			// 
			this->label19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(6, 108);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(45, 17);
			this->label19->TabIndex = 21;
			this->label19->Text = L"Mask:";
			// 
			// listBox_RuleMask
			// 
			this->listBox_RuleMask->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->listBox_RuleMask->FormattingEnabled = true;
			this->listBox_RuleMask->ItemHeight = 16;
			this->listBox_RuleMask->Location = System::Drawing::Point(4, 136);
			this->listBox_RuleMask->Name = L"listBox_RuleMask";
			this->listBox_RuleMask->Size = System::Drawing::Size(432, 116);
			this->listBox_RuleMask->TabIndex = 25;
			// 
			// label21
			// 
			this->label21->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(175, 108);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(44, 17);
			this->label21->TabIndex = 21;
			this->label21->Text = L"Type:";
			// 
			// lbl_ObjectOwnerSid
			// 
			this->lbl_ObjectOwnerSid->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ObjectOwnerSid->AutoSize = true;
			this->lbl_ObjectOwnerSid->Location = System::Drawing::Point(49, 74);
			this->lbl_ObjectOwnerSid->Name = L"lbl_ObjectOwnerSid";
			this->lbl_ObjectOwnerSid->Size = System::Drawing::Size(20, 17);
			this->lbl_ObjectOwnerSid->TabIndex = 23;
			this->lbl_ObjectOwnerSid->Text = L"...";
			// 
			// lbl_MaskType
			// 
			this->lbl_MaskType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_MaskType->AutoSize = true;
			this->lbl_MaskType->Location = System::Drawing::Point(225, 108);
			this->lbl_MaskType->Name = L"lbl_MaskType";
			this->lbl_MaskType->Size = System::Drawing::Size(20, 17);
			this->lbl_MaskType->TabIndex = 21;
			this->lbl_MaskType->Text = L"...";
			// 
			// label22
			// 
			this->label22->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(6, 74);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(34, 17);
			this->label22->TabIndex = 19;
			this->label22->Text = L"SID:";
			// 
			// comboBox_ObjectIntegrityLevel
			// 
			this->comboBox_ObjectIntegrityLevel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->comboBox_ObjectIntegrityLevel->FormattingEnabled = true;
			this->comboBox_ObjectIntegrityLevel->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"Low", L"Medium", L"High",
					L"System"
			});
			this->comboBox_ObjectIntegrityLevel->Location = System::Drawing::Point(112, 18);
			this->comboBox_ObjectIntegrityLevel->Name = L"comboBox_ObjectIntegrityLevel";
			this->comboBox_ObjectIntegrityLevel->Size = System::Drawing::Size(136, 24);
			this->comboBox_ObjectIntegrityLevel->TabIndex = 6;
			this->comboBox_ObjectIntegrityLevel->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox_ObjectIntegrityLevel_SelectedIndexChanged);
			// 
			// panel5
			// 
			this->panel5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel5->Controls->Add(this->label16);
			this->panel5->Controls->Add(this->comboBox_objectType);
			this->panel5->Controls->Add(this->listBox_RuleUser);
			this->panel5->Controls->Add(this->btn_browseObject);
			this->panel5->Controls->Add(this->label24);
			this->panel5->Controls->Add(this->lbl_ObjectOwnerName);
			this->panel5->Controls->Add(this->label25);
			this->panel5->Controls->Add(this->lbl_ObjectPath);
			this->panel5->Controls->Add(this->label18);
			this->panel5->Controls->Add(this->label23);
			this->panel5->Location = System::Drawing::Point(3, 3);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(440, 297);
			this->panel5->TabIndex = 26;
			// 
			// label16
			// 
			this->label16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(6, 22);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(53, 17);
			this->label16->TabIndex = 16;
			this->label16->Text = L"Object:";
			// 
			// comboBox_objectType
			// 
			this->comboBox_objectType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBox_objectType->FormattingEnabled = true;
			this->comboBox_objectType->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"File", L"Folder" });
			this->comboBox_objectType->Location = System::Drawing::Point(68, 18);
			this->comboBox_objectType->Name = L"comboBox_objectType";
			this->comboBox_objectType->Size = System::Drawing::Size(269, 24);
			this->comboBox_objectType->TabIndex = 7;
			// 
			// listBox_RuleUser
			// 
			this->listBox_RuleUser->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->listBox_RuleUser->FormattingEnabled = true;
			this->listBox_RuleUser->ItemHeight = 16;
			this->listBox_RuleUser->Location = System::Drawing::Point(6, 137);
			this->listBox_RuleUser->Name = L"listBox_RuleUser";
			this->listBox_RuleUser->Size = System::Drawing::Size(427, 116);
			this->listBox_RuleUser->TabIndex = 25;
			this->listBox_RuleUser->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::listBox_RuleUser_SelectedIndexChanged);
			// 
			// btn_browseObject
			// 
			this->btn_browseObject->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btn_browseObject->Location = System::Drawing::Point(343, 17);
			this->btn_browseObject->Name = L"btn_browseObject";
			this->btn_browseObject->Size = System::Drawing::Size(86, 30);
			this->btn_browseObject->TabIndex = 0;
			this->btn_browseObject->Text = L"browse";
			this->btn_browseObject->UseVisualStyleBackColor = true;
			this->btn_browseObject->Click += gcnew System::EventHandler(this, &MyForm::btn_BrowseClicked);
			// 
			// label24
			// 
			this->label24->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(6, 48);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(41, 17);
			this->label24->TabIndex = 20;
			this->label24->Text = L"Path:";
			// 
			// lbl_ObjectOwnerName
			// 
			this->lbl_ObjectOwnerName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ObjectOwnerName->AutoSize = true;
			this->lbl_ObjectOwnerName->Location = System::Drawing::Point(126, 74);
			this->lbl_ObjectOwnerName->Name = L"lbl_ObjectOwnerName";
			this->lbl_ObjectOwnerName->Size = System::Drawing::Size(20, 17);
			this->lbl_ObjectOwnerName->TabIndex = 24;
			this->lbl_ObjectOwnerName->Text = L"...";
			// 
			// label25
			// 
			this->label25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(6, 74);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(53, 17);
			this->label25->TabIndex = 21;
			this->label25->Text = L"Owner:";
			// 
			// lbl_ObjectPath
			// 
			this->lbl_ObjectPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->lbl_ObjectPath->AutoSize = true;
			this->lbl_ObjectPath->Location = System::Drawing::Point(71, 48);
			this->lbl_ObjectPath->Name = L"lbl_ObjectPath";
			this->lbl_ObjectPath->Size = System::Drawing::Size(20, 17);
			this->lbl_ObjectPath->TabIndex = 22;
			this->lbl_ObjectPath->Text = L"...";
			// 
			// label18
			// 
			this->label18->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(6, 108);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(109, 17);
			this->label18->TabIndex = 21;
			this->label18->Text = L"Account names:";
			// 
			// label23
			// 
			this->label23->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(71, 74);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(49, 17);
			this->label23->TabIndex = 18;
			this->label23->Text = L"Name:";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				25.16199F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				74.83801F)));
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel2, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel4, 0, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 12);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 49)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1211, 729);
			this->tableLayoutPanel1->TabIndex = 6;
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel2->ColumnCount = 1;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel2->Controls->Add(this->panel1, 0, 0);
			this->tableLayoutPanel2->Controls->Add(this->panel2, 0, 1);
			this->tableLayoutPanel2->Location = System::Drawing::Point(304, 0);
			this->tableLayoutPanel2->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 2;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 56.51577F)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 43.48423F)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(907, 729);
			this->tableLayoutPanel2->TabIndex = 0;
			// 
			// tableLayoutPanel4
			// 
			this->tableLayoutPanel4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel4->ColumnCount = 1;
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel6, 0, 0);
			this->tableLayoutPanel4->Controls->Add(this->treeView_Proc, 0, 1);
			this->tableLayoutPanel4->Controls->Add(this->btn_Refresh, 0, 2);
			this->tableLayoutPanel4->Location = System::Drawing::Point(3, 3);
			this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
			this->tableLayoutPanel4->RowCount = 3;
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 4.360902F)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 95.6391F)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 57)));
			this->tableLayoutPanel4->Size = System::Drawing::Size(298, 723);
			this->tableLayoutPanel4->TabIndex = 1;
			// 
			// tableLayoutPanel6
			// 
			this->tableLayoutPanel6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel6->ColumnCount = 2;
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				63.79928F)));
			this->tableLayoutPanel6->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				36.20072F)));
			this->tableLayoutPanel6->Controls->Add(this->btm_FindProc, 1, 0);
			this->tableLayoutPanel6->Controls->Add(this->textBox_NameProcToFind, 0, 0);
			this->tableLayoutPanel6->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel6->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel6->Name = L"tableLayoutPanel6";
			this->tableLayoutPanel6->RowCount = 1;
			this->tableLayoutPanel6->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel6->Size = System::Drawing::Size(298, 28);
			this->tableLayoutPanel6->TabIndex = 18;
			// 
			// btm_FindProc
			// 
			this->btm_FindProc->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btm_FindProc->Location = System::Drawing::Point(193, 3);
			this->btm_FindProc->Name = L"btm_FindProc";
			this->btm_FindProc->Size = System::Drawing::Size(102, 22);
			this->btm_FindProc->TabIndex = 17;
			this->btm_FindProc->Text = L"find";
			this->btm_FindProc->UseVisualStyleBackColor = true;
			this->btm_FindProc->Click += gcnew System::EventHandler(this, &MyForm::btm_FindProc_Click);
			// 
			// textBox_NameProcToFind
			// 
			this->textBox_NameProcToFind->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_NameProcToFind->Location = System::Drawing::Point(3, 3);
			this->textBox_NameProcToFind->Name = L"textBox_NameProcToFind";
			this->textBox_NameProcToFind->Size = System::Drawing::Size(184, 22);
			this->textBox_NameProcToFind->TabIndex = 16;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1235, 753);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"Process Explorer (Sanek и Dimon Production)";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->tableLayoutPanel3->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			this->panel4->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->tableLayoutPanel5->ResumeLayout(false);
			this->panel6->ResumeLayout(false);
			this->panel6->PerformLayout();
			this->panel5->ResumeLayout(false);
			this->panel5->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel2->ResumeLayout(false);
			this->tableLayoutPanel4->ResumeLayout(false);
			this->tableLayoutPanel6->ResumeLayout(false);
			this->tableLayoutPanel6->PerformLayout();
			this->ResumeLayout(false);

		}
	#pragma endregion
	private:
		System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e)
		{
			procList = Process::GetProcesses();

			this->ch_ASLR_BottomUpRandom->AutoCheck = false;
			this->ch_ASLR_ForceRelocate->AutoCheck = false;
			this->ch_ASLR_HighEntropy->AutoCheck = false;
			this->ch_DEP_Enable->AutoCheck = false;
			
			ClearFields();

			for each (Process^ proc in procList)
			{
				//proc->Parent
				//String^ add_node_str;
				//this->treeView1->Nodes->Add(proc->Id.ToString("D5") + ": " + proc->P rocessName + " " + proc->MainModule->FileName);
				
				this->treeView_Proc->Nodes->Add(proc->Id.ToString(), proc->Id.ToString("D5") + ": " + proc->ProcessName);

				/*try
				{
					for each(ProcessModule^ module in proc->Modules)
					{
						//this->treeView1->Nodes->Count
						//this->treeView1->Nodes[this->treeView1->Nodes->Count - 1].Add(module->ModuleName);
						this->treeView1->Nodes[this->treeView1->Nodes->Count - 1]->Nodes->Add(module->ModuleName);
						//Console::WriteLine("  - " + module->ModuleName);
					}
				}
				catch (Exception^)
				{
					Console::WriteLine("");
				}*/
			}
		
			hCurProc = NULL;
			tCurProc = NULL;
			SecurityInfoList.init();
		}
	private:
		System::Void ClearFields()
		{
			this->lbl_OwnerName->Text = "...";
			this->lbl_OwnerSid->Text = "...";
			this->lbl_ParentId->Text = "...";
			this->lbl_ParentName->Text = "...";
			this->lbl_ProcId->Text = "...";
			this->lbl_ProcName->Text = "...";
			this->lbl_ProcPath->Text = "...";
			this->lbl_ProcType->Text = "...";

			this->comboBox_IntegrityLevel->Text = "...";
			this->ch_ASLR_BottomUpRandom->Checked = false;
			this->ch_ASLR_ForceRelocate->Checked = false;
			this->ch_ASLR_HighEntropy->Checked = false;
			this->ch_DEP_Enable->Checked = false;

			this->listBox_DLL->Items->Clear();
		}
	private:
		System::Void ParseMask(int mask, enum ftype ft)
		{
			this->listBox_RuleMask->Items->Clear();
			int cmsk[4];

			if (cmsk[0] = ((mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)) this->listBox_RuleMask->Items->Add("FILE_ALL_ACCESS ");
			if (cmsk[0]) mask &= (~FILE_ALL_ACCESS);
			if (cmsk[1] = ((mask & FILE_GENERIC_READ) == FILE_GENERIC_READ)) this->listBox_RuleMask->Items->Add("FILE_GENERIC_READ ");
			if (cmsk[2] = ((mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE)) this->listBox_RuleMask->Items->Add("FILE_GENERIC_WRITE ");
			if (cmsk[3] = ((mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)) this->listBox_RuleMask->Items->Add("FILE_GENERIC_EXECUTE ");

			if (cmsk[1]) mask &= (~FILE_GENERIC_READ);
			if (cmsk[2]) mask &= (~FILE_GENERIC_WRITE);
			if (cmsk[3]) mask &= (~FILE_GENERIC_EXECUTE);

			if (cmsk[0] = ((mask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)) this->listBox_RuleMask->Items->Add("STANDARD_RIGHTS_REQUIRED ");
			if (cmsk[1] = ((mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)) this->listBox_RuleMask->Items->Add("STANDARD_RIGHTS_ALL ");
			if (cmsk[2] = ((mask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)) this->listBox_RuleMask->Items->Add("SPECIFIC_RIGHTS_ALL ");

			if (cmsk[0]) mask &= (~STANDARD_RIGHTS_REQUIRED);
			if (cmsk[1]) mask &= (~STANDARD_RIGHTS_ALL);
			if (cmsk[2]) mask &= (~SPECIFIC_RIGHTS_ALL);

			if ((mask & DELETE) == DELETE) this->listBox_RuleMask->Items->Add("DELETE ");
			if ((mask & READ_CONTROL) == READ_CONTROL) this->listBox_RuleMask->Items->Add("READ_CONTROL ");
			if ((mask & WRITE_DAC) == WRITE_DAC) this->listBox_RuleMask->Items->Add("WRITE_DAC ");
			if ((mask & WRITE_OWNER) == WRITE_OWNER) this->listBox_RuleMask->Items->Add("WRITE_OWNER ");
			if ((mask & SYNCHRONIZE) == SYNCHRONIZE) this->listBox_RuleMask->Items->Add("SYNCHRONIZE ");

			//for files
			if (ft == ffile)
			{
				if ((mask & FILE_READ_DATA) == FILE_READ_DATA)	this->listBox_RuleMask->Items->Add("FILE_READ_DATA ");
				if ((mask & FILE_WRITE_DATA) == FILE_WRITE_DATA)	this->listBox_RuleMask->Items->Add("FILE_WRITE_DATA ");
				if ((mask & FILE_APPEND_DATA) == FILE_APPEND_DATA)	this->listBox_RuleMask->Items->Add("FILE_APPEND_DATA ");
				if ((mask & FILE_READ_EA) == FILE_READ_EA)	this->listBox_RuleMask->Items->Add("FILE_READ_EA ");
				if ((mask & FILE_WRITE_EA) == FILE_WRITE_EA)	this->listBox_RuleMask->Items->Add("FILE_WRITE_EA ");
				if ((mask & FILE_EXECUTE) == FILE_EXECUTE)	this->listBox_RuleMask->Items->Add("FILE_EXECUTE ");
				if ((mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES)	this->listBox_RuleMask->Items->Add("FILE_READ_ATTRIBUTES ");
				if ((mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES)	this->listBox_RuleMask->Items->Add("FILE_WRITE_ATTRIBUTES ");
				//for directories 
			}
			else
			{
				if ((mask & FILE_LIST_DIRECTORY) == FILE_LIST_DIRECTORY)	this->listBox_RuleMask->Items->Add("FILE_LIST_DIRECTORY ");
				if ((mask & FILE_ADD_FILE) == FILE_ADD_FILE)	this->listBox_RuleMask->Items->Add("FILE_ADD_FILE ");
				if ((mask & FILE_ADD_SUBDIRECTORY) == FILE_ADD_SUBDIRECTORY)	this->listBox_RuleMask->Items->Add("FILE_ADD_SUBDIRECTORY ");
				if ((mask & FILE_READ_EA) == FILE_READ_EA)	this->listBox_RuleMask->Items->Add("FILE_READ_EA ");
				if ((mask & FILE_WRITE_EA) == FILE_WRITE_EA)	this->listBox_RuleMask->Items->Add("FILE_WRITE_EA ");
				if ((mask & FILE_TRAVERSE) == FILE_TRAVERSE)	this->listBox_RuleMask->Items->Add("FILE_TRAVERSE ");
				if ((mask & FILE_DELETE_CHILD) == FILE_DELETE_CHILD)	this->listBox_RuleMask->Items->Add("FILE_DELETE_CHILD ");
				if ((mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES)	this->listBox_RuleMask->Items->Add("FILE_READ_ATTRIBUTES ");
				if ((mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES)	this->listBox_RuleMask->Items->Add("FILE_WRITE_ATTRIBUTES ");
			}

			if (mask & 0xFFE0FE00) this->listBox_RuleMask->Items->Add("FILE_OTHER\n");
		}
	private:
		System::Void btn_Refresh_Click(System::Object^  sender, System::EventArgs^  e)
		{
			//array<Process^>^ procList = Process::GetProcesses();
			this->treeView_Proc->Nodes->Clear();
			for each (Process^ proc in procList)
			{
				this->treeView_Proc->Nodes->Add(proc->Id.ToString(), proc->Id.ToString("D5") + ": " + proc->ProcessName);
			}
			this->treeView_Proc;
		}
	private:
		System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		{
			Process^ SelProc = procList[this->treeView_Proc->SelectedNode->Index];

			this->SuspendLayout();
			ClearFields();
			//
			// Id Name Path Type
			//
			this->lbl_ProcId->Text = SelProc->Id.ToString("D5");

			try
			{
				this->lbl_ProcPath->Text = SelProc->MainModule->FileName;
			}
			catch (...)
			{
				this->lbl_ProcPath->Text = "...";
			}
			this->lbl_ProcName->Text = SelProc->ProcessName;
			this->lbl_ProcType->Text = "x64";


			//
			// Free Old Handle
			//
			if (hCurProc) CloseHandle(hCurProc);
			if (tCurProc) CloseHandle(tCurProc);
			hCurProc = tCurProc = NULL;

			//
			// Handle
			//
			if (hCurProc = GetProcessHandle(SelProc->Id))
			{

				//
				// ASLR DEP
				//
				PROC_POLICY_INFO PPI;
				GetProcPolicyInfo(hCurProc, PPI);


				if (PPI.ASLR.EnableBottomUpRandomization)
				{
					this->ch_ASLR_BottomUpRandom->Checked = true;
				}
				if (PPI.ASLR.EnableForceRelocateImages)
				{
					this->ch_ASLR_ForceRelocate->Checked = true;
				}
				if (PPI.ASLR.EnableHighEntropy)
				{
					this->ch_ASLR_HighEntropy->Checked = true;
				}
				if (PPI.DEP.Enable)
				{
					this->ch_DEP_Enable->Checked = true;
				}
				//
				// Parent
				//
				DWORD ParentId = 0;
				GetProcessParentId(SelProc->Id, ParentId);
				Process^ ParentProc = Process::GetProcessById(ParentId);
				this->lbl_ParentId->Text = ParentProc->Id.ToString();
				this->lbl_ParentName->Text = ParentProc->ProcessName;
				Console::WriteLine(ParentProc->Id.ToString());
				Console::WriteLine(ParentProc->ProcessName);

				//
				// Token
				//
				if (tCurProc = GetProcessToken(hCurProc))
				{
					//
					// Owner
					//
					USER_INFO UI;
					if (GetProcUsername(tCurProc, UI))
					{
						this->lbl_OwnerSid->Text = gcnew String(UI.SidStr);
						this->lbl_OwnerName->Text = gcnew String(UI.Name);
					}

					//
					// Integrity Level
					//
					DWORD IL = 0;
					if (GetProcIntegrityLevel(tCurProc, IL))
					{
						this->comboBox_IntegrityLevel->SelectedIndex = IntegrityLevelToIndex(IL);
					}
					else
					{
						this->comboBox_ObjectIntegrityLevel->Text = "...";
					}
					//
					// Privelege
					//
					PTOKEN_PRIVILEGES pTP;
					DWORD len = MAX_PATH;
					CHAR PrivName[MAX_PATH];

					GetProcessPrivelegeList(tCurProc, pTP);
					this->chListBox_Priveleges->Items->Clear();

					try
					{
						Console::WriteLine(INT(pTP->PrivilegeCount).ToString());
						for (int i = 0; i < pTP->PrivilegeCount; i++)
						{
							len = MAX_PATH - 1;
							if (LookupPrivilegeNameA(NULL, &pTP->Privileges[i].Luid, PrivName, &len))
							{
								int last_index = this->chListBox_Priveleges->Items->Count;
								this->chListBox_Priveleges->Items->Add(gcnew String(PrivName));
								if (pTP->Privileges[i].Attributes)
								{
									this->chListBox_Priveleges->SetItemChecked(last_index, true);
								}
							}
						}
						free(pTP);
					}
					catch (Exception^) {}

				}
			}

			//
			// DLL
			//

			try
			{
				for each(ProcessModule^ module in SelProc->Modules)
				{
					this->listBox_DLL->Items->Add(module->ModuleName);
				}
			}
			catch (Exception^)
			{
				Console::WriteLine("No Dll");
			}

			this->ResumeLayout();
		}
	private:
		System::Void checkedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			Process^ SelProc = procList[this->treeView_Proc->SelectedNode->Index];
			int ch_idx = this->chListBox_Priveleges->SelectedIndex;
			String^ shPrivName = this->chListBox_Priveleges->Items[ch_idx]->ToString();
			std::wstring PrivName = msclr::interop::marshal_as<std::wstring>(shPrivName);

			if (tCurProc)
			{
				Console::WriteLine(SelProc->Id.ToString() + " IDX: " + treeView_Proc->SelectedNode->Index.ToString());
				if (this->chListBox_Priveleges->GetItemCheckState(ch_idx) == CheckState::Checked)
				{
					Console::WriteLine(gcnew String(PrivName.data()));
					EnablePrivilege(tCurProc, PrivName.data());
					Console::WriteLine("Enabled");
				}
				else
				{
					DisablePrivilege(tCurProc, PrivName.data());
					Console::WriteLine("Disnabled");
				}
			}

		}
	private:
		System::Void btn_BrowseClicked(System::Object^  sender, System::EventArgs^  e)
		{
			OpenFileDialog ^ofd = gcnew OpenFileDialog;
			FolderBrowserDialog ^fbd = gcnew FolderBrowserDialog;

			switch (this->comboBox_objectType->SelectedIndex)
			{
			case 0:
				ofd->ShowDialog();
				CurObjectName = ofd->FileName;
				break;
			case 1:
				fbd->ShowDialog();
				CurObjectName = fbd->SelectedPath;
				break;
			default: CurObjectName = nullptr;
				break;
			}

			if (CurObjectName != nullptr)
			{
				Console::WriteLine(CurObjectName);
				this->lbl_ObjectPath->Text = CurObjectName;

				String^ shObjName = CurObjectName;
				std::wstring ObjName = msclr::interop::marshal_as<std::wstring>(shObjName);

				USER_INFO UI;
				if (GetObjectOwnerName(ObjName.data(), UI))
				{
					this->lbl_ObjectOwnerName->Text = gcnew String(UI.Name);
					this->lbl_ObjectOwnerSid->Text = gcnew String(UI.SidStr);
				}
				else
				{
					this->lbl_ObjectOwnerName->Text = "...";
					this->lbl_ObjectOwnerSid->Text = "...";
				}

				DWORD IL;
				if (GetObjectIntegrityLevel(ObjName.data(), IL))
				{
					this->comboBox_ObjectIntegrityLevel->SelectedIndex = IntegrityLevelToIndex(IL);
				}
				else
				{
					this->comboBox_ObjectIntegrityLevel->Text = "...";
				}

				this->listBox_RuleMask->Items->Clear();
				this->lbl_MaskType->Text = "...";

				SecurityInfoList.clear();
				if (GetObjectSecurityInfo(ObjName.data()))
				{
					for (int i = 0; i < SecurityInfoList.count; i++)
					{
						this->listBox_RuleUser->Items->Add(gcnew String(SecurityInfoList.list[i].UI.Name));
					}
				}
				else
				{

				}
			}
			else
			{
				this->lbl_ObjectOwnerName->Text = "...";
				this->lbl_ObjectOwnerSid->Text = "...";
				this->comboBox_ObjectIntegrityLevel->Text = "...";
				this->lbl_MaskType->Text = "...";
			}
		}
	private:
		System::Void comboBox_IntegrityLevel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			DWORD IL = IndexToIntegrityLevel(this->comboBox_IntegrityLevel->SelectedIndex);
			
			if (tCurProc != NULL)
			{
				if (SetProcIntegrityLevel(tCurProc, IL))
				{
					this->comboBox_IntegrityLevel->SelectedIndex;
				}
				else
				{
					if (GetProcIntegrityLevel(tCurProc, IL))
					{
						this->comboBox_IntegrityLevel->SelectedIndex = IntegrityLevelToIndex(IL);
					}
					else
					{
						this->comboBox_ObjectIntegrityLevel->Text = "...";
					}
				}
			}
			else
			{
				this->comboBox_IntegrityLevel->Text = "...";
			}
		}
	private:
		System::Void comboBox_ObjectIntegrityLevel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			DWORD IL = this->comboBox_ObjectIntegrityLevel->SelectedIndex;

			if (CurObjectName != nullptr)
			{
				String^ shObjName = CurObjectName;
				std::wstring ObjName = msclr::interop::marshal_as<std::wstring>(shObjName);

				if (SetObjectIntegrityLevel(ObjName.data(), IL))
				{
					this->comboBox_ObjectIntegrityLevel->SelectedIndex;
				}
				else
				{
					if (GetObjectIntegrityLevel(ObjName.data(), IL))
					{
						this->comboBox_ObjectIntegrityLevel->SelectedIndex = IntegrityLevelToIndex(IL);
					}
					else
					{
						this->comboBox_ObjectIntegrityLevel->Text = "...";
					}
				}
			}
			else
			{
				this->comboBox_ObjectIntegrityLevel->Text = "...";
			}
		}

	private: 
		System::Void listBox_RuleUser_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			int Index = this->listBox_RuleUser->SelectedIndex;
			this->lbl_MaskType->Text = gcnew String(SecurityInfoList.list[Index].Type ? "DENY" : "ALLOW");
			ParseMask(SecurityInfoList.list[Index].Mask, SecurityInfoList.ObjectType);
		}
	private:
		System::Void btm_FindProc_Click(System::Object^  sender, System::EventArgs^  e)
		{
			int index = this->treeView_Proc->Nodes->IndexOfKey(this->textBox_NameProcToFind->Text);
		
			if (index < 0)
			{
				index = -1;
				for each (Process^ proc in procList)
				{
					index++;
					if (proc->ProcessName == this->textBox_NameProcToFind->Text)
					{
						treeView_Proc->SelectedNode = treeView_Proc->Nodes[index];
						return;
					}
				}
			}
			else
			{
				treeView_Proc->SelectedNode = treeView_Proc->Nodes->Find(this->textBox_NameProcToFind->Text, true)[0];
			}
		}

		
};
}
