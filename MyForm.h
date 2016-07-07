#pragma once
#include "PCIE.h"
#include "converter.h"
#include <string>
#include <iostream>
#include <fstream>

namespace app {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	using System::Runtime::InteropServices::Marshal;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

			//
			//TODO: Add the constructor code here
			//

			lib_handle = PCIE_Load();
			if (!lib_handle){
				MessageBox::Show("PCIE_Load failed!", "PCIE_Load failed!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			}
			else
			{
				outTextStatus->Text = "PCIE Load successful";
			}

			hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
			if (!hPCIE){
				MessageBox::Show("PCIE_Open failed!", "PCIE_Load failed!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			}
			else
			{
				PCIE_Close(hPCIE);
			}

			//file handling
			ptr_file = new ifstream();
			ptr_file->open(DATABASE_FILENAME);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::StatusStrip^  outStatus;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  inQuery;
	private: System::Windows::Forms::RichTextBox^  outSubject;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  outSubjectID;
	private: System::Windows::Forms::Label^  outSubjectLength;
	private: System::Windows::Forms::ToolStripStatusLabel^  outTextStatus;

			 //Binh variable declaration

			 static void *lib_handle;
			 PCIE_HANDLE hPCIE;

			 //query area
			 static array<Char>^ QueryHex = gcnew array<Char>(MAX_QUERY_SIZE);
			 static array<Char>^ MemoryBlock = gcnew array<Char>(MEM_SIZE);
			 boolean end_subject = false;

			 ifstream  * ptr_file;

			 boolean first = true;

			 int  linecounter = 0;
			 char^ startMaker = '>';

			 //database area
			 boolean full_buffer = false;

			 int  subject_ID = 0;
			 int  subject_length;

			 // Command
			 int command = 0;

	private: System::Windows::Forms::ListView^  outHitScore;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::RichTextBox^  outDumpMemory;



			 //
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		boolean  CovertQuery2Bit1(char indata[], int inQueryNuclearLength, char outdata[], int beginIdx)
		{
			int i;
			int temp_base;
			int temp_offset;
			int temp_value;

			temp_base = beginIdx;
			temp_offset = 0;
			// = indata->ToUpper();
			//clean outdata
			for (i = 0; i < inQueryNuclearLength * 3 / 8; i++)
			{
				outdata[i] = 0;
			}
			for (i = 0; i < inQueryNuclearLength && indata[i] != 0 && indata[i] != 10; i++)
			{

				if (indata[i] == 'a')      { indata[i] = 'A'; }
				else if (indata[i] == 'g') { indata[i] = 'G'; }
				else if (indata[i] == 't') { indata[i] = 'T'; }
				else if (indata[i] == 'c') { indata[i] = 'C'; }
				else if (indata[i] == 'n') { indata[i] = 'N'; }

				if (indata[i] == 'A')      { temp_value = A_ADN << temp_offset; }
				else if (indata[i] == 'G') { temp_value = G_ADN << temp_offset; }
				else if (indata[i] == 'T') { temp_value = T_ADN << temp_offset; }
				else if (indata[i] == 'C') { temp_value = C_ADN << temp_offset; }
				else if (indata[i] == 'N') { temp_value = N_ADN << temp_offset; }
				else {
					Console::WriteLine("00:invalid query at index %d, content [%d]", i, indata[i]);
					return false;
				}
				outdata[temp_base] |= temp_value;
				//printf("[%d][%c][BASE %d][offset %d][CONVERT %d][TEMP VALUE %d]\n",i, indata[i],temp_base, temp_offset, outdata[temp_base], temp_value );
				if (temp_offset == 6){
					temp_base++;
					temp_offset = 1;
					if (indata[i] == 'A')      { temp_value = A_ADN >> 2; }
					else if (indata[i] == 'G') { temp_value = G_ADN >> 2; }
					else if (indata[i] == 'T') { temp_value = T_ADN >> 2; }
					else if (indata[i] == 'C') { temp_value = C_ADN >> 2; }
					else if (indata[i] == 'N') { temp_value = N_ADN >> 2; }
					else {
						Console::WriteLine("01:invalid query at index %d, content [%c]", i, indata[i]);
						return false;
					}
					outdata[temp_base] |= temp_value;
				}
				else if (temp_offset == 7){
					temp_base++;
					temp_offset = 2;
					if (indata[i] == 'A')      { temp_value = A_ADN >> 1; }
					else if (indata[i] == 'G') { temp_value = G_ADN >> 1; }
					else if (indata[i] == 'T') { temp_value = T_ADN >> 1; }
					else if (indata[i] == 'C') { temp_value = C_ADN >> 1; }
					else if (indata[i] == 'N') { temp_value = N_ADN >> 1; }
					else {
						Console::WriteLine("02:invalid query at index %d, content [%c]", i, indata[i]);
						return false;
					}
					outdata[temp_base] |= temp_value;
				}
				else{
					temp_offset += 3;
					if (temp_offset == 8){
						temp_base++;
						temp_offset = 0;
					}
				}
				Console::WriteLine(indata[i]);
				Console::WriteLine("{0:X}", outdata[temp_base]);
				//printf("  [%d][%c][BASE %d][offset %d][CONVERT %d][TEMP VALUE %d]\n",i, indata[i],temp_base, temp_offset, outdata[temp_base], temp_value );
			}
			return true;
		}

		boolean readSubject()
		{
			static char outSubject[MAX_QUERY_SIZE];
			char inSubject[MAX_QUERY_NUCLEAR_SIZE];
			char buff_line[DATABASE_LINE_WIDTH];
			const char * outSubject_temp = &outSubject[8];
			const char * inSubject_temp = inSubject;
			int Slength = 0;
			while (!end_subject)
			{
				if (ptr_file->eof())
				{
					MessageBox::Show("Cant read file!", "", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					return false;
				}
				ptr_file->getline(buff_line, DATABASE_LINE_WIDTH);
				//String^ newlinee = Convert::ToString(buff_line[0]);
				//Console::WriteLine(newlinee);
				if (buff_line[0].Equals(startMaker))
				{
					if (first)
					{
						first = false;
					}
					else
					{
						end_subject = true;
						CovertQuery2Bit1(inSubject, strlen(inSubject), outSubject, 8);
						subject_length = strlen(inSubject) - 8;
						outSubject[7] = subject_ID & 0xFF;
						outSubject[6] = (subject_ID >> 8) & 0xFF;
						outSubject[5] = (subject_ID >> 16) & 0xFF;
						outSubject[4] = (subject_ID >> 24) & 0xFF;
						outSubject[3] = subject_length & 0xFF;
						outSubject[2] = (subject_length >> 8) & 0xFF;
						outSubject[1] = (subject_length >> 16) & 0xFF;
						outSubject[0] = (subject_length >> 24) & 0xFF;
					}
					subject_ID++;
					Console::WriteLine("new SUBJECT: " + subject_ID);
				}
				else
				{
					Console::WriteLine("data SUBJECT");
					strncat(inSubject, buff_line, strlen(buff_line));
				}
			}
			end_subject = true;
			return true;
		}

		void InitializeComponent(void)
		{
			System::Windows::Forms::ListViewItem^  listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(L"Hit Address Query UnGap"));
			System::Windows::Forms::ListViewItem^  listViewItem2 = (gcnew System::Windows::Forms::ListViewItem(L"Hit Address Subject UnGap"));
			System::Windows::Forms::ListViewItem^  listViewItem3 = (gcnew System::Windows::Forms::ListViewItem(L"Length UnGap"));
			System::Windows::Forms::ListViewItem^  listViewItem4 = (gcnew System::Windows::Forms::ListViewItem(L"Hit and Score"));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->outStatus = (gcnew System::Windows::Forms::StatusStrip());
			this->outTextStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->inQuery = (gcnew System::Windows::Forms::TextBox());
			this->outSubject = (gcnew System::Windows::Forms::RichTextBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->outHitScore = (gcnew System::Windows::Forms::ListView());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->outSubjectID = (gcnew System::Windows::Forms::Label());
			this->outSubjectLength = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->outDumpMemory = (gcnew System::Windows::Forms::RichTextBox());
			this->outStatus->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(502, 6);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(92, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Query";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// outStatus
			// 
			this->outStatus->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->outTextStatus });
			this->outStatus->Location = System::Drawing::Point(0, 378);
			this->outStatus->Name = L"outStatus";
			this->outStatus->Size = System::Drawing::Size(606, 22);
			this->outStatus->TabIndex = 1;
			this->outStatus->Text = L"status here";
			this->outStatus->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MyForm::statusStrip1_ItemClicked);
			// 
			// outTextStatus
			// 
			this->outTextStatus->Name = L"outTextStatus";
			this->outTextStatus->Size = System::Drawing::Size(0, 17);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(61, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Your query:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 32);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Subject";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 53);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(21, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"ID:";
			// 
			// inQuery
			// 
			this->inQuery->Location = System::Drawing::Point(79, 6);
			this->inQuery->Name = L"inQuery";
			this->inQuery->Size = System::Drawing::Size(417, 20);
			this->inQuery->TabIndex = 5;
			this->inQuery->Text = L"ATGACGTCTATTTTCGTGAAACAATTTTCGTGAAAACGTCTAAACCAAGTCAAAAATTCGTGAATACGTCTATTTTCGTGA"
				L"AACAATGTTAACGGATAACTAAACCAAGTCAAAAATTTTCGTGAATT";
			// 
			// outSubject
			// 
			this->outSubject->Location = System::Drawing::Point(62, 32);
			this->outSubject->Name = L"outSubject";
			this->outSubject->Size = System::Drawing::Size(434, 92);
			this->outSubject->TabIndex = 6;
			this->outSubject->Text = L"";
			this->outSubject->TextChanged += gcnew System::EventHandler(this, &MyForm::outSubject_TextChanged);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(502, 130);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(92, 23);
			this->button5->TabIndex = 2;
			this->button5->Text = L"button5";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// outHitScore
			// 
			this->outHitScore->GridLines = true;
			this->outHitScore->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(4) {
				listViewItem1, listViewItem2,
					listViewItem3, listViewItem4
			});
			this->outHitScore->Location = System::Drawing::Point(6, 6);
			this->outHitScore->Name = L"outHitScore";
			this->outHitScore->Size = System::Drawing::Size(562, 207);
			this->outHitScore->TabIndex = 1;
			this->outHitScore->UseCompatibleStateImageBehavior = false;
			this->outHitScore->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::outHitScore_SelectedIndexChanged);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(502, 30);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(92, 23);
			this->button2->TabIndex = 0;
			this->button2->Text = L"Subject";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(502, 101);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(92, 23);
			this->button3->TabIndex = 8;
			this->button3->Text = L"Exit";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(13, 82);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(43, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Length:";
			this->label4->Click += gcnew System::EventHandler(this, &MyForm::label4_Click);
			// 
			// outSubjectID
			// 
			this->outSubjectID->AutoSize = true;
			this->outSubjectID->Location = System::Drawing::Point(40, 53);
			this->outSubjectID->Name = L"outSubjectID";
			this->outSubjectID->Size = System::Drawing::Size(0, 13);
			this->outSubjectID->TabIndex = 10;
			// 
			// outSubjectLength
			// 
			this->outSubjectLength->AutoSize = true;
			this->outSubjectLength->Location = System::Drawing::Point(52, 82);
			this->outSubjectLength->Name = L"outSubjectLength";
			this->outSubjectLength->Size = System::Drawing::Size(0, 13);
			this->outSubjectLength->TabIndex = 11;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(502, 53);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(92, 23);
			this->button4->TabIndex = 12;
			this->button4->Text = L"Hit Score";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(502, 77);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(92, 23);
			this->button6->TabIndex = 13;
			this->button6->Text = L"Dump memory";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 130);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(582, 245);
			this->tabControl1->TabIndex = 14;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->outHitScore);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(574, 219);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Hit and score pairs";
			this->tabPage1->UseVisualStyleBackColor = true;
			this->tabPage1->Click += gcnew System::EventHandler(this, &MyForm::tabPage1_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->outDumpMemory);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(574, 219);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Dump memory";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// outDumpMemory
			// 
			this->outDumpMemory->Location = System::Drawing::Point(6, 6);
			this->outDumpMemory->Name = L"outDumpMemory";
			this->outDumpMemory->Size = System::Drawing::Size(562, 210);
			this->outDumpMemory->TabIndex = 0;
			this->outDumpMemory->Text = L"";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(606, 400);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->outSubjectLength);
			this->Controls->Add(this->outSubjectID);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->outSubject);
			this->Controls->Add(this->inQuery);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->outStatus);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->outStatus->ResumeLayout(false);
			this->outStatus->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 boolean returnvalue = true;
				 static char QueryHex[MAX_QUERY_SIZE];

				 static char inQueryNuclear[MAX_QUERY_NUCLEAR_SIZE];
				 strcpy(inQueryNuclear, (char*)Marshal::StringToHGlobalAnsi(inQuery->Text->Trim()).ToPointer());
				 const PCIE_LOCAL_ADDRESS LocalAddr = PCIE_MEM_QUERY_ADDR;

				 if (inQuery->Text->Trim()->Length == 0)
				 {
					 MessageBox::Show("The NULL nuclear is NOT allowed!", "", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				 }
				 else
				 {
					 //MessageBox::Show("pass!", "", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);

					 returnvalue = CovertQuery2Bit1(inQueryNuclear, MAX_QUERY_NUCLEAR_SIZE, QueryHex, 0);

					 //Console::WriteLine(inQueryNuclear);
					 if (!returnvalue){
						 MessageBox::Show("Invalid input nuclear string!", "", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					 }
					 else
					 {
						 for (int i = 0; i < MAX_QUERY_SIZE; i++)
						 {
							 Console::Write("{0:X} _", QueryHex[i]);
						 }
						 boolean pass = PCIE_DmaWrite(hPCIE, LocalAddr, QueryHex, MAX_QUERY_SIZE);
						 if (pass)
						 {
							 outTextStatus->Text = "Write Query successful";
						 }
						 else
						 {
							 MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
						 }
					 }
				 }
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 PCIE_Unload(lib_handle);
				 ptr_file->close();
				 this->Close();
	}

	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void outSubject_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void outHitScorePairs_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void statusStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void outHitScore_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
				 readSubject();
	}
	private: System::Void tabPage1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
				 char MemoryBlock[100000];
				 const PCIE_LOCAL_ADDRESS LocalAddr = PCIE_MEM_QUERY_ADDR;
				 boolean pass;
				 try
				 {
					 pass = PCIE_DmaRead(hPCIE, LocalAddr, MemoryBlock, MEM_SIZE);
				 }
				 catch (exception& e)
				 {
					 Console::WriteLine("06:DMA Memory:{");
				 }
				 
				 if (!pass)
				 {
					 Console::WriteLine("07:DMA Memory:PCIE_DmaRead failed");
				 }
				 else
				 {
					 outTextStatus->Text = "Dump all memory data successful!";
					 outDumpMemory->Text = "";
					 for (int i = 0; i < MEM_SIZE; i++)
					 {
						 outDumpMemory->AppendText(MemoryBlock[i] + " ");
						 if (i % 8 == 0)
						 {
							 outDumpMemory->AppendText("\n");
						 }
					 }
				 }

	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {

				 static char HitScore[MAX_HIT_SCORE_SIZE];
				 const PCIE_LOCAL_ADDRESS LocalAddr = PCIE_MEM_SCORE_ADDR;
				 if (!PCIE_DmaRead(hPCIE, LocalAddr, HitScore, MAX_HIT_SCORE_SIZE))
				 {
					 Console::WriteLine("07:DMA Memory:PCIE_DmaRead failed");
				 }
				 else
				 {
					 outTextStatus->Text = "Read hit and score pairs successful!";
					 outDumpMemory->Text = "";
					 for (int i = 0; i < MAX_HIT_SCORE_SIZE; i++)
					 {
						 outDumpMemory->AppendText(HitScore[i] + " ");
						 if (i % 8 == 0)
						 {
							 outDumpMemory->AppendText("\n");
						 }
					 }
				 }
	}


	};
};