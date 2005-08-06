; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "ZViewer"
!define PRODUCT_VERSION "0.4.0"
!define PRODUCT_PUBLISHER "RhLab"
!define PRODUCT_WEB_SITE "http://www.wimy.com"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor bzip2

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
;!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "ReadMe.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Korean"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}Setup${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite on

; Preview Extension DLLs
  IfFileExists "$INSTDIR\ZViewerAgent.dll" ZViewerDllCheck ZViewerCopyDll

ZViewerDllCheck:
  Delete "$INSTDIR\ZViewerAgent.dll"
  IfErrors ZViewerNeedUninstallAndReboot ZViewerCopyDll

ZViewerNeedUninstallAndReboot:
  MessageBox MB_OK "To install new dll, the installer will kill explorer."
  KillProcDLL::KillProc "explorer.exe"

; explorer 가 바로 죽어도 dll 을 복사하는 속도가 더 빠를 수 있으므로 잠시 대기
  Sleep 1000

ZViewerCopyDll:

  File "ZViewerIcons.dll"
  File "ZViewer.exe"
  File "_ZViewer.png"
  File "LoadError.png"
  File "License.txt"
  File "ReadMe.txt"

  File "FreeImage.dll"
  File "FreeImagePlus.dll"
  File "ZViewerAgent.dll"

; Language Files 
  SetOutPath "$INSTDIR\language"
  File "language\korean.dll"

; Register DLL
  SetOutPath "$INSTDIR" ; RegDll 을 하기 위해서는 그 폴더로 가야한다.
  RegDll "$INSTDIR\ZViewerAgent.dll"
  ExecWait "$INSTDIR\ZViewer.exe /fileext"

; explorer.exe 가 없으면 새로 실행 시켜준다.
  FindProcDLL::FindProc "explorer.exe"
  StrCmp $R0 1 ZViewerEnd ZViewerExecExplorer

ZViewerExecExplorer:
  Exec "explorer.exe"
ZViewerEnd:

SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\ZViewerAgent.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\ZViewer.lnk" "$INSTDIR\ZViewer.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) is uninstalled."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Uninstall $(^Name)?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  UnRegDll "$INSTDIR\ZViewerAgent.dll"

  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\ZViewer.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\ZViewerAgent.lnk"

  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  RMDir /rebootok /r "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd
