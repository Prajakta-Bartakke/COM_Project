//fields-Name,Birthdate,education,Degree,company


#include"dialouge.h"
#include <iostream>
#include <fstream>
using namespace std;


//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
char name[50];
int day;
int month;
int year;
char education[50];
char degree[50];
char company[50];
IFileReader* pIFileReader = NULL;

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdshow)
{
	//local variable dw\eclarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("WINDEV");
	
	//code
	//WNDCLASSEX initialisation
	CoInitialize(NULL);

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	//Regitsration of WNDCLASSEX
	RegisterClassEx(&wndclass);

	//creating window
	hwnd = CreateWindow(
		szAppName,
		TEXT("PAB:my first window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	//show the window

	//ShowWindow(hwnd, iCmdshow);

	//update the window
	UpdateWindow(hwnd);


	//message loop

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();
	return ((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{
	//local variable declarations
	static HINSTANCE hInstance;
	//code
	switch (iMsg)
	{
	case WM_CREATE:
		//call to DialougeBox
		hInstance = ((LPCREATESTRUCT)lparam)->hInstance;
		DialogBox(hInstance, TEXT("ABOUTDIALOUGE"), hwnd, (DLGPROC)DlgProc);
#if 0
		if (DialogBox(hInstance, TEXT("ABOUTDIALOUGE"), hwnd, (DLGPROC)DlgProc))
		{
			//MessageBox(NULL, TEXT("Dialouge box created "), TEXT("Success"), MB_OK);
		}
#endif
		break;
	
	case WM_DESTROY:
		//MessageBox(NULL, TEXT("destroying window"), TEXT("Success"), MB_OK);
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wparam, lparam));
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hDlgbrush;
	FILE* fp = NULL;
	HRESULT hr;
	HWND parentHandle;
	ofstream fileContainment;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		//MessageBox(NULL, TEXT("Init dialouge box"), TEXT("Success"), MB_OK);
		SetWindowText(hwnd, TEXT("Data Entry"));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case ID_Win32:
			//MessageBox(NULL, TEXT("IDOK"), TEXT("Success"), MB_OK);
			//considering all fields are filled properly then only add and enable the button
			GetDlgItemText(hwnd, ID_ETName, name, 50);
			day = GetDlgItemInt(hwnd, ID_ETDate, NULL, NULL);
			month = GetDlgItemInt(hwnd, ID_ETMonth, NULL, NULL);
			year = GetDlgItemInt(hwnd, ID_ETYear, NULL, NULL);
			GetDlgItemText(hwnd, ID_ETEducation, education, 50);
			GetDlgItemText(hwnd, ID_ETDegree, degree, 50);
			GetDlgItemText(hwnd, ID_ETCompany, company, 50);
			fp = fopen("C:\\Projects\\01_Dialouge_box\\Dialouge_version5\\info_all.txt", "a");
			fprintf(fp, "\nName\t\t:%s\nBirthdate\t:%d-%d-%d\nEducation\t:%s\nDegree\t\t:%s\nCompany\t\t:%s\n", name,day,month,year,education,degree,company);
			fclose(fp);
			fileContainment.open("example.txt");
			fileContainment << day << "-" << month << "-" <<year;
			fileContainment.close();


			//EnableWindow(hwnd, FALSE);
#if 0
			parentHandle = GetParent(hwnd);
			DestroyWindow(parentHandle);
			DeleteObject(hDlgbrush);
			EndDialog(hwnd, 0);
#endif
			break;

		case ID_ClassFactory:
			//cocreateinstance and pass the argument as file path
			MessageBox(NULL, TEXT("IClassFactory button clicked"), TEXT("Success"), MB_OK);
			hr = CoCreateInstance(CLSID_CFileReader, NULL, CLSCTX_INPROC_SERVER, IID_IFileReader, (void**)&pIFileReader);
			if (FAILED(hr))
			{
				MessageBox(NULL, TEXT("IFileReader instance can not be get"), TEXT("Failure"), MB_OK);
				parentHandle = GetParent(hwnd);
				DestroyWindow(parentHandle);
				EndDialog(hwnd, 0);
			}
			MessageBox(NULL, TEXT("IFileReader instance found"), TEXT("Success"), MB_OK);
			hr = pIFileReader->ReadFileData("C:\\Projects\\01_Dialouge_box\\Dialouge_version5\\info_all.txt");
			if (FAILED(hr))
			{
				MessageBox(NULL, TEXT("File data not readed successfully"), TEXT("Failure"), MB_OK);
				parentHandle = GetParent(hwnd);
				DestroyWindow(parentHandle);
				EndDialog(hwnd, 0);
			}
			pIFileReader->Release();
			pIFileReader = NULL;
			//EnableWindow(hwnd, FALSE);
			parentHandle = GetParent(hwnd);
			DestroyWindow(parentHandle);
			DeleteObject(hDlgbrush);
			EndDialog(hwnd, 0);
			break;
		}
		return TRUE;

	case WM_CTLCOLORDLG:
		//hDlgbrush = CreateSolidBrush(RGB(135, 206, 235));
		hDlgbrush = CreateSolidBrush(RGB(0, 0, 0));
		return ((BOOL)hDlgbrush);
		break;
	}

	

	return FALSE;
}