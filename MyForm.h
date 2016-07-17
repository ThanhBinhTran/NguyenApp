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
				MessageBox::Show("PCIE_Load failed!", "PCIE_Load failed!", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else
			{
				outTextStatus->Text = "PCIE Load successful";
			}

			hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
			if (!hPCIE){
				MessageBox::Show("PCIE_Open failed!", "PCIE_Load failed!",
						MessageBoxButtons::OK, MessageBoxIcon::Error);
						
				PCIE_Close(hPCIE);
				//button1->Enabled = false;
				
			}


			//file handling
			ptr_file = new ifstream();
			ptr_file->open(DATABASE_FILENAME);


			cout << "PCIE_MEM_QUERY_ADDR " << PCIE_MEM_QUERY_ADDR << endl;
			cout << "PCIE_MEM_SUBJECT_ADDR " << PCIE_MEM_SUBJECT_ADDR << endl;
			cout << "PCIE_MEM_SCORE_ADDR " << PCIE_MEM_SCORE_ADDR << endl;

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



	private: System::Windows::Forms::TextBox^  inQuery;
	private: System::Windows::Forms::RichTextBox^  outSubject;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;

	private: System::Windows::Forms::Label^  outSubjectID;
	private: System::Windows::Forms::Label^  outSubjectLength;
	private: System::Windows::Forms::ToolStripStatusLabel^  outTextStatus;

			 //Binh variable declaration

			 static void *lib_handle;
			 static PCIE_HANDLE hPCIE =0;

			 //query area
			 //static array<Char>^ QueryHex = gcnew array<Char>(MAX_QUERY_SIZE);
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


	private: System::Windows::Forms::Button^  button4;

	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::TabControl^  tabControl1;

	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::RichTextBox^  outDumpMemory;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::DataGridView^  outHitScore_Grid;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::GroupBox^  gb_query;





private: System::Windows::Forms::GroupBox^  gb_subject;
private: System::Windows::Forms::GroupBox^  gb_control;


private: System::Windows::Forms::DataGridViewTextBoxColumn^  hit_add_inQ_UnGap;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  hit_add_inS_UnGap;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  hit_length_UnGap;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  hit_add_score;
private: System::Windows::Forms::TextBox^  hitscore_length;

private: System::Windows::Forms::TextBox^  hitscore_ID;

private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Button^  button5;

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
			cout <<endl <<"Convert instring:" << indata << endl;
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
					Console::WriteLine("00:invalid query at index {0}, content:{1:X}", i, indata[i]);
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
						Console::WriteLine("01:invalid query at index {0}, content {1:X}", i, indata[i]);
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
						
						//Console::WriteLine("02:invalid query at index {0}, content {1:X}", i, indata[i]);
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
				//Console::WriteLine(indata[i]);
				//Console::WriteLine("{0:X}", outdata[temp_base]);
				//printf("  [%d][%c][BASE %d][offset %d][CONVERT %d][TEMP VALUE %d]\n",i, indata[i],temp_base, temp_offset, outdata[temp_base], temp_value );
			}
			return true;
		}

		boolean readSubject(char outSubjectBYTE[])
		{
			
			static char inSubject[MAX_QUERY_NUCLEAR_SIZE] = {""};
			char buff_line[DATABASE_LINE_WIDTH] = { "" };
			int Slength = 0;
			//reset inSubject string
			strncpy(inSubject, "",1);

			while (!end_subject)
			{
				if (ptr_file->eof())
				{
					MessageBox::Show("Cant read file!", "", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return false;
				}
				//strcpy(buff_line, "");
				ptr_file->getline(buff_line, DATABASE_LINE_WIDTH);
				cout << endl<<  "LINE: " << buff_line << endl;
				if (buff_line[0].Equals(startMaker) || ptr_file->eof())
				{
					cout << "head subject" << endl;
					if (first)
					{
						first = false;
					}
					else
					{
						subject_ID++;
						CovertQuery2Bit1(inSubject, strlen(inSubject), outSubjectBYTE, 8);
						subject_length = strlen(&outSubjectBYTE[8]);
						outSubjectBYTE[7] = subject_ID & 0xFF;
						outSubjectBYTE[6] = (subject_ID >> 8) & 0xFF;
						outSubjectBYTE[5] = (subject_ID >> 16) & 0xFF;
						outSubjectBYTE[4] = (subject_ID >> 24) & 0xFF;
						outSubjectBYTE[3] = subject_length & 0xFF;
						outSubjectBYTE[2] = (subject_length >> 8) & 0xFF;
						outSubjectBYTE[1] = (subject_length >> 16) & 0xFF;
						outSubjectBYTE[0] = (subject_length >> 24) & 0xFF;
						end_subject = true;
						outSubject->Text = gcnew String(inSubject);
					}
					
					
					//cout << "SUBJECT INHEX:" << outSubject << endl;
				}
				else
				{
					strncat(inSubject, buff_line, strlen(buff_line));
					//add end of string
					int i = strlen(inSubject);
					if (inSubject[i - 1] == 10){
						inSubject[i - 1] = 0;
					}
					cout << "data line: " << buff_line << endl;
					//cout << "insubject: " << inSubject << endl;
				}
			}
			end_subject = false;
			return true;
		}
		void PrintStringInHex(char indata[], int SIZE)
		{
			for (int i = 0; i < SIZE; i++)
			{
				Console::Write("{0:X} ", indata[i]);
			}
		}
		boolean getHitScore()
		{
			static char HitScore[MAX_HIT_SCORE_SIZE];
			FILE * fp;
			int hs_ID = 0;
			int hs_length = 0;
			static int hs_ID_writen = 0;
			boolean pass = false;
			const PCIE_LOCAL_ADDRESS LocalAddr = PCIE_MEM_SCORE_ADDR;

			if (!PCIE_DmaRead(hPCIE, LocalAddr, HitScore, MAX_HIT_SCORE_SIZE))
			{
				MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else
			{
				outTextStatus->Text = "Read hit and score pairs successful!";
				outHitScore_Grid->Rows->Clear();
				//display hit score 
				hs_length = (int)HitScore[3] + (int)HitScore[2] * 256 + (int)HitScore[1] * 256 * 256 + (int)HitScore[0] * 256 * 256 * 256;
				hs_ID = (int)HitScore[7] + (int)HitScore[6] * 256 + (int)HitScore[5] * 256 * 256 + (int)HitScore[4] * 256 * 256 * 256;

				hitscore_ID->Text = hs_ID + "";
				hitscore_length->Text = hs_length + "";
				cout << endl << "Hit score report: hs length[" << hs_length << "][" << hs_ID << "]" << endl;
				for (int i = 0; i < MAX_HIT_SCORE_SIZE; i = i + 1)
				{
					Console::Write("{0:x} ", HitScore[i]);
				}
				for (int i = 8; i < MAX_HIT_SCORE_SIZE && i < hs_length * 8; i = i + 8)
				{
					outHitScore_Grid->Rows->Add(
						HitScore[i + 0], HitScore[i + 1], HitScore[i + 2], HitScore[i + 3]);
				}
				if (hs_ID == subject_ID)
				{
					if (subject_ID == 1)
					{
						fp = fopen("HIT_SCORE.HEX", "w");
						hs_ID_writen = 1;
						pass = true;
					}
					else if (subject_ID != hs_ID_writen)
					{
						fp = fopen("HIT_SCORE.HEX", "a+");
						hs_ID_writen = subject_ID;
						pass = true;
					}
					if (pass)
					{
						fprintf(fp, "ID %d length %d\n", hs_ID, hs_length);
						for (int i = 8; i < MAX_HIT_SCORE_SIZE && i < hs_length * 8; i = i + 8)
						{
							fprintf(fp, "%d %d %d %d\n", HitScore[i + 0], HitScore[i + 1], HitScore[i + 2], HitScore[i + 3]);
						}
						fclose(fp);
					}
				}
				else
				{
					return false;
				}
				

			}
			return true;
		}
		boolean sendSubject()
		{
			static char outSubjectByte[MAX_SUBJECT_SIZE];
			boolean pass;
			pass = readSubject(outSubjectByte);
			if (pass)
			{
				DisplaySubjectID(subject_ID, subject_length);
				cout << "OUT SUBJECT: ";
				PrintStringInHex(outSubjectByte, subject_length + 8);
				//write query data
				pass = PCIE_DmaWrite(hPCIE, PCIE_MEM_SUBJECT_ADDR, outSubjectByte, subject_length + 8);
				if (pass)
				{
					//write trigger command 
					pass = writeTriggerCommand(WRITE_SUBJECT);
					if (pass)
					{
						outTextStatus->Text = "Write Subject successful";
					}
					else
					{
						MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
					}
				}
				else
				{
					MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		boolean writeTriggerCommand(int command)
		{
			return PCIE_Write32(hPCIE, PCIE_USER_BAR, PCIE_CMD_ADDR, command);
		}

		void DisplaySubjectID(int id, int length)
		{
			gb_subject->Text = "SUBJECT [ID:" + id + "][length:" + length + "]";
		}
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->outStatus = (gcnew System::Windows::Forms::StatusStrip());
			this->outTextStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->inQuery = (gcnew System::Windows::Forms::TextBox());
			this->outSubject = (gcnew System::Windows::Forms::RichTextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->outSubjectID = (gcnew System::Windows::Forms::Label());
			this->outSubjectLength = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->hitscore_length = (gcnew System::Windows::Forms::TextBox());
			this->hitscore_ID = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->outHitScore_Grid = (gcnew System::Windows::Forms::DataGridView());
			this->hit_add_inQ_UnGap = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hit_add_inS_UnGap = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hit_length_UnGap = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hit_add_score = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->outDumpMemory = (gcnew System::Windows::Forms::RichTextBox());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->gb_query = (gcnew System::Windows::Forms::GroupBox());
			this->gb_subject = (gcnew System::Windows::Forms::GroupBox());
			this->gb_control = (gcnew System::Windows::Forms::GroupBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->outStatus->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->outHitScore_Grid))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->gb_query->SuspendLayout();
			this->gb_subject->SuspendLayout();
			this->gb_control->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(5, 15);
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
			this->outStatus->Location = System::Drawing::Point(0, 457);
			this->outStatus->Name = L"outStatus";
			this->outStatus->Size = System::Drawing::Size(806, 22);
			this->outStatus->TabIndex = 1;
			this->outStatus->Text = L"status here";
			this->outStatus->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MyForm::statusStrip1_ItemClicked);
			// 
			// outTextStatus
			// 
			this->outTextStatus->Name = L"outTextStatus";
			this->outTextStatus->Size = System::Drawing::Size(0, 17);
			// 
			// inQuery
			// 
			this->inQuery->Location = System::Drawing::Point(6, 16);
			this->inQuery->Name = L"inQuery";
			this->inQuery->Size = System::Drawing::Size(671, 20);
			this->inQuery->TabIndex = 5;
			this->inQuery->Text = L"ATGACGTCTATTTTCGTGAAACAATTTTCGTGAAAACGTCTAAACCAAGTCAAAAATTCGTGAATACGTCTATTTTCGTGA"
				L"AACAATGTTAACGGATAACTAAACCAAGTCAAAAATTTTCGTGAATT";
			// 
			// outSubject
			// 
			this->outSubject->Enabled = false;
			this->outSubject->Location = System::Drawing::Point(6, 18);
			this->outSubject->Name = L"outSubject";
			this->outSubject->Size = System::Drawing::Size(670, 102);
			this->outSubject->TabIndex = 6;
			this->outSubject->Text = L"";
			this->outSubject->TextChanged += gcnew System::EventHandler(this, &MyForm::outSubject_TextChanged);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(5, 44);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(92, 23);
			this->button2->TabIndex = 0;
			this->button2->Text = L"Subject";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(5, 158);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(92, 23);
			this->button3->TabIndex = 8;
			this->button3->Text = L"Exit";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
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
			this->button4->Location = System::Drawing::Point(5, 73);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(92, 23);
			this->button4->TabIndex = 12;
			this->button4->Text = L"Hit Score";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(5, 102);
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
			this->tabControl1->Location = System::Drawing::Point(7, 187);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(793, 267);
			this->tabControl1->TabIndex = 14;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->hitscore_length);
			this->tabPage1->Controls->Add(this->hitscore_ID);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->outHitScore_Grid);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(785, 241);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Hit and score pairs";
			this->tabPage1->UseVisualStyleBackColor = true;
			this->tabPage1->Click += gcnew System::EventHandler(this, &MyForm::tabPage1_Click);
			// 
			// hitscore_length
			// 
			this->hitscore_length->Enabled = false;
			this->hitscore_length->Location = System::Drawing::Point(366, 6);
			this->hitscore_length->Name = L"hitscore_length";
			this->hitscore_length->Size = System::Drawing::Size(144, 20);
			this->hitscore_length->TabIndex = 4;
			// 
			// hitscore_ID
			// 
			this->hitscore_ID->Enabled = false;
			this->hitscore_ID->Location = System::Drawing::Point(84, 6);
			this->hitscore_ID->Name = L"hitscore_ID";
			this->hitscore_ID->Size = System::Drawing::Size(100, 20);
			this->hitscore_ID->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(190, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(170, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"NUMBER OF HIT SCORE PAIRS:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(72, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"SUBJECT ID:";
			// 
			// outHitScore_Grid
			// 
			this->outHitScore_Grid->AllowUserToDeleteRows = false;
			this->outHitScore_Grid->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->outHitScore_Grid->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {
				this->hit_add_inQ_UnGap,
					this->hit_add_inS_UnGap, this->hit_length_UnGap, this->hit_add_score
			});
			this->outHitScore_Grid->Location = System::Drawing::Point(3, 34);
			this->outHitScore_Grid->Name = L"outHitScore_Grid";
			this->outHitScore_Grid->ReadOnly = true;
			this->outHitScore_Grid->Size = System::Drawing::Size(779, 204);
			this->outHitScore_Grid->TabIndex = 0;
			this->outHitScore_Grid->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::outHitScore_Grid_CellContentClick);
			// 
			// hit_add_inQ_UnGap
			// 
			this->hit_add_inQ_UnGap->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->hit_add_inQ_UnGap->HeaderText = L"hit_add_inQ_UnGap";
			this->hit_add_inQ_UnGap->Name = L"hit_add_inQ_UnGap";
			this->hit_add_inQ_UnGap->ReadOnly = true;
			// 
			// hit_add_inS_UnGap
			// 
			this->hit_add_inS_UnGap->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->hit_add_inS_UnGap->HeaderText = L"hit_add_inS_UnGap";
			this->hit_add_inS_UnGap->Name = L"hit_add_inS_UnGap";
			this->hit_add_inS_UnGap->ReadOnly = true;
			// 
			// hit_length_UnGap
			// 
			this->hit_length_UnGap->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->hit_length_UnGap->HeaderText = L"hit_length_UnGap";
			this->hit_length_UnGap->Name = L"hit_length_UnGap";
			this->hit_length_UnGap->ReadOnly = true;
			// 
			// hit_add_score
			// 
			this->hit_add_score->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->hit_add_score->HeaderText = L"hit_add_score";
			this->hit_add_score->Name = L"hit_add_score";
			this->hit_add_score->ReadOnly = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->outDumpMemory);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(785, 241);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Dump memory";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// outDumpMemory
			// 
			this->outDumpMemory->Dock = System::Windows::Forms::DockStyle::Fill;
			this->outDumpMemory->Location = System::Drawing::Point(3, 3);
			this->outDumpMemory->Name = L"outDumpMemory";
			this->outDumpMemory->Size = System::Drawing::Size(779, 235);
			this->outDumpMemory->TabIndex = 0;
			this->outDumpMemory->Text = L"";
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"Column4";
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"Column3";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"Column2";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"Column1";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			// 
			// gb_query
			// 
			this->gb_query->Controls->Add(this->inQuery);
			this->gb_query->Location = System::Drawing::Point(6, 3);
			this->gb_query->Name = L"gb_query";
			this->gb_query->Size = System::Drawing::Size(683, 51);
			this->gb_query->TabIndex = 15;
			this->gb_query->TabStop = false;
			this->gb_query->Text = L"QUERY";
			// 
			// gb_subject
			// 
			this->gb_subject->Controls->Add(this->outSubject);
			this->gb_subject->Location = System::Drawing::Point(7, 56);
			this->gb_subject->Name = L"gb_subject";
			this->gb_subject->Size = System::Drawing::Size(682, 128);
			this->gb_subject->TabIndex = 6;
			this->gb_subject->TabStop = false;
			this->gb_subject->Text = L"SUBJECT";
			this->gb_subject->Enter += gcnew System::EventHandler(this, &MyForm::groupBox2_Enter);
			// 
			// gb_control
			// 
			this->gb_control->Controls->Add(this->button5);
			this->gb_control->Controls->Add(this->button1);
			this->gb_control->Controls->Add(this->button2);
			this->gb_control->Controls->Add(this->button4);
			this->gb_control->Controls->Add(this->button6);
			this->gb_control->Controls->Add(this->button3);
			this->gb_control->Location = System::Drawing::Point(695, 3);
			this->gb_control->Name = L"gb_control";
			this->gb_control->Size = System::Drawing::Size(105, 200);
			this->gb_control->TabIndex = 16;
			this->gb_control->TabStop = false;
			this->gb_control->Text = L"Control Panel";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(5, 131);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(92, 23);
			this->button5->TabIndex = 14;
			this->button5->Text = L"Subject Full";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click_1);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(806, 479);
			this->Controls->Add(this->gb_control);
			this->Controls->Add(this->gb_subject);
			this->Controls->Add(this->gb_query);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->outSubjectLength);
			this->Controls->Add(this->outSubjectID);
			this->Controls->Add(this->outStatus);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->outStatus->ResumeLayout(false);
			this->outStatus->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->outHitScore_Grid))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->gb_query->ResumeLayout(false);
			this->gb_query->PerformLayout();
			this->gb_subject->ResumeLayout(false);
			this->gb_control->ResumeLayout(false);
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
				 
				 //tobe updated
				 ptr_file->close();
				 ptr_file->open(DATABASE_FILENAME);
				 subject_ID = 0;
				 first = true; 

				 if (inQuery->Text->Trim()->Length == 0)
				 {
					 MessageBox::Show("The NULL nuclear is NOT allowed!", "", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 }
				 else
				 {
					 //MessageBox::Show("pass!", "", MessageBoxButtons::OK, MessageBoxIcon::Error);

					 returnvalue = CovertQuery2Bit1(inQueryNuclear, MAX_QUERY_NUCLEAR_SIZE, QueryHex, 0);
					 PrintStringInHex(QueryHex, MAX_QUERY_SIZE);
					 //Console::WriteLine(inQueryNuclear);
					 if (!returnvalue){
						 MessageBox::Show("Invalid input nuclear string!", "", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 }
					 else
					 {
                      
						 //write query data
						 boolean pass = PCIE_DmaWrite(hPCIE, PCIE_MEM_QUERY_ADDR, QueryHex, MAX_QUERY_SIZE);
						 if (pass)
						 {
							 //write trigger command 
							 pass = writeTriggerCommand(WRITE_QUERY);
							 if (pass)
							 {
								 outTextStatus->Text = "Write Query successful";
								 subject_ID = 0;
								 subject_length = 0;
								 DisplaySubjectID(subject_ID, subject_length);
							 }
							 else
							 {
								 MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
							 }
						 }
						 else
						 {
							 MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
						 }


					 }
				 }
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 int result = 0;
				 result = (int)MessageBox::Show("Do you want to exit?", "NGUYEN APPLICATION", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);
				 switch (result)
				 {
				 case IDYES:
					 PCIE_Close(hPCIE);
					 PCIE_Unload(lib_handle);
					 ptr_file->close();
					 this->Close();

					 break;
				 case IDNO:
					 // Do something
					 break;
				 case IDCANCEL:
					 // Do something
					 break;
				 }

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
				 sendSubject();
				 
	}
	private: System::Void outHitScore_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void tabPage1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
				 char MemoryBlock[MEM_SIZE];
				 FILE * fp;
				 const PCIE_LOCAL_ADDRESS LocalAddr = PCIE_MEM_QUERY_ADDR;
				 boolean pass;
				 pass = PCIE_DmaRead(hPCIE, LocalAddr, MemoryBlock, MEM_SIZE);

				 if (!pass)
				 {
					 MessageBox::Show("PCIE DMA Memory Write failed!\n\nPlease reboot your PC", "DMA Memory ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 }
				 else
				 {
					 fp = fopen("ALL_MEMORY.HEX", "w+");
					 for (int i = 0; i < MEM_SIZE; i++) {
						 if (i % 24 == 0 && i != 0) {
							 fprintf(fp, "\n");
						 }
						 else if (i % 8 == 0 && i != 0) {
							 fprintf(fp, "_ ");
						 }
						 fprintf(fp, "%02X ", (unsigned char)MemoryBlock[i]);
					 }
					 fclose(fp);
#ifdef DEBUG
					 outDumpMemory->Text = "";
					 for (int i = 0; i < MEM_SIZE; i++)
					 {
						 outDumpMemory->AppendText(MemoryBlock[i] + " ");
						 if (i % 24 == 0 && i > 0)
						 {
							 outDumpMemory->AppendText("\n");
						 }
						 else if (i % 8 == 0 && i > 0)
						 {
							 outDumpMemory->AppendText(" _ ");
						 }
					 }
#endif // DEBUG

					 outTextStatus->Text = "Dump all memory data successful!";
				 }

	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
				 getHitScore();
	}


	private: System::Void outHitScore_Grid_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
	}
private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void HitScorePairs_Display_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button5_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 while (sendSubject())
			 {
				 while (!getHitScore())
				 {
					 ;
				 }
			 }
}
};
};