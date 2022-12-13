#include "critical_section.h"

CriticalSectionDlg* CriticalSectionDlg::ptr = NULL;

CRITICAL_SECTION cs;
TCHAR str[100];

CriticalSectionDlg::CriticalSectionDlg(void)
{
	ptr = this;
}
CriticalSectionDlg::~CriticalSectionDlg(void)
{
	DeleteCriticalSection(&cs);
}
void CriticalSectionDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}
BOOL CriticalSectionDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	InitializeCriticalSection(&cs);
	return TRUE;
}

DWORD WINAPI WriteToFiles(LPVOID lp)
{
	srand(time(0));
	EnterCriticalSection(&cs);

	ofstream res(TEXT("Result.txt"));

	ofstream create2(TEXT("Copy2.txt"));
	CopyFile(L"C:\\Пользователи\\ADMIN\\Downloads\\Критические секции\\CriticalSection\\CriticalSection\\Copy.txt",
		L"C:\\Пользователи\\ADMIN\\Downloads\\Критические секции\\CriticalSection\\CriticalSection\\Copy2.txt", 0);
	if (create2.is_open()) {
		MessageBox(0, TEXT("Copy 2 create"), TEXT(""), MB_OK);
	}
	create2.close();

	ofstream create3(TEXT("Copy3.txt"));
	CopyFile(L"C:\\Пользователи\\ADMIN\\Downloads\\Критические секции\\CriticalSection\\CriticalSection\\Copy.txt",
		L"C:\\Пользователи\\ADMIN\\Downloads\\Критические секции\\CriticalSection\\CriticalSection\\Copy2.txt", 0);
	if (create3.is_open()) {
		MessageBox(0, TEXT("Copy 3 create"), TEXT(""), MB_OK);
	}
	create3.close();

	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI ReadFromFiles(LPVOID lp)
{
	EnterCriticalSection(&cs);
	ifstream in2(TEXT("Copy2.txt"));
	if (!in2.is_open()) {
		MessageBox(0, TEXT("Error"), TEXT("Error"), MB_OK);
		return 1;
	}
	in2.close();

	ifstream in3(TEXT("Copy3.txt"));
	if (!in3.is_open()) {
		MessageBox(0, TEXT("Error"), TEXT("Error"), MB_OK);
		return 1;
	}
	in3.close();

	CopyFile(L"Copy2.txt", L"Result.txt", 0);
	CopyFile(L"Copy3.txt", L"Result.txt", 0);

	LeaveCriticalSection(&cs);
	return 0;
}


void CriticalSectionDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		HANDLE hThread = CreateThread(NULL, 0, WriteToFiles, 0, 0, NULL);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ReadFromFiles, 0, 0, NULL);
		CloseHandle(hThread);
	}
}

BOOL CALLBACK CriticalSectionDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}