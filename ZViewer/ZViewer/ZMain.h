#pragma once

#include "src/ZHistory.h"
#include "src/ZOption.h"
#include "../lib/ZImage.h"

class ZMain
{
public:
	static ZMain & GetInstance();
	~ZMain(void);

	void setInitArg(const std::string & strArg) { m_strInitArg = strArg; }
	void SetProgramFolder();
	void RescanFolder();			// 파일목록을 다시 읽어들인다.
	void OnInit();
	void Draw(bool bEraseBg = true);

	bool NextImage();
	bool PrevImage();

	bool FirstImage();
	bool LastImage();

	HWND GetHWND() { return m_hMainDlg; }
	void SetHWND(HWND hWnd);
	void SetMainMenu(HMENU hMenu)
	{
		m_hMainMenu = hMenu;
	}
	void SetPopupMenu(HMENU hMenu)
	{
		m_hPopupMenu = hMenu;
	}

	long GetCachedKByte();

	void SetStatusHandle(HWND hWnd) { m_hStatus = hWnd; }
	HWND GetStatusHandle() { return m_hStatus; }

	void ToggleFullScreen();

	void ToggleBigToScreenStretch();

	int GetLogCacheHitRate();

	void DeleteThisFile();

	const std::string & GetProgramFolder()	// 프로그램 실행 파일이 있는 폴더를 가져온다.
	{
		return m_strProgramFolder;
	}

	/// 적절한 언어 ini 를 읽어와서 화면을 세팅한다.
	void LoadLanguage();

	void SetInstance(HINSTANCE hInstance) { m_hMainInstance = hInstance; }
	HINSTANCE GetHInstance() const { return m_hMainInstance; }
	void OnChangeCurrentSize(int iWidth, int iHeight);
	void OnDrag(int x, int y);

	void OnFocusLose();
	void OnFocusGet();

	void OnRightTopFirstDraw();

	inline bool IsFullScreen() const { return m_option.m_bFullScreen; }

	void OpenFile(const std::string & strFilename);
	void OpenFileDialog();

	void Undo();
	void Redo();

	bool m_bHandCursor;

	void ShowFileExtDlg();

	void ShellTrayShow();

private:
	ZMain(void);

	void LoadCurrent();
	void SetStatusBarText();
	void SetTitle();

	void InitOpenFileDialog();

	void StartBannerDownload();
	static DWORD WINAPI BannerThreadFunc(LPVOID p);

	void ShellTrayHide();

	void FormShow();
	void FormHide();

	std::string m_strInitArg;		// 프로그램 시작 인자.
	std::string m_strProgramFolder;	// 프로그램 실행 파일이 있는 폴더

	std::string m_strCurrentFolder;	// 현재 폴더
	std::string m_strCurrentFilename;	// 현재 보여주고 있는 파일이름
	std::vector<std::string> m_vFile;
	int m_iCurretFileIndex;

	int m_iCurrentScreenWidth;
	int m_iCurrentScreenHeight;

	fipImage m_bannerImage;

	/// 현재 보여주고 있는 이미지
	ZImage m_currentImage;
	DWORD m_dwLoadingTime;				// 현재 이미지를 로딩하는 데 걸린 시간

	HWND m_hMainDlg;
	HWND m_hStatus;
	HINSTANCE m_hMainInstance;

	HMENU m_hMainMenu;
	HMENU m_hPopupMenu;

	ZOption m_option;
	int m_iRestoreX;
	int m_iRestoreY;
	int m_iRestoreWidth;
	int m_iRestoreHeight;

	int m_iShowingX;			// 그림 중 어디를 찍기 시작하나.
	int m_iShowingY;			// 그림 중 어디를 찍기 시작하나.

	void ZFindFile(const char *path, std::vector<std::string> & foundStorage, bool bFindRecursive = false);
	std::string GetFolderNameFromFullFileName(const std::string & strFullFilename);
	bool ZMain::IsValidImageFileExt(const char * szFilename);

	// For Open File Dialog
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	//bool m_bOpeningFileDialog;	// 전체화면일 때 파일 다이얼로그를 열면 깜빡거림을 막기 위해

	// For Undo/Redo
	ZHistory m_history;

};
