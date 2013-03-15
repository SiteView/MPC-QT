OutFile "NETGEAR NetgearExtender-Install.exe"

; Define your application name
!define APPNAME "NetgearExtender"
!define APPNAME_NOSPACE "NetgearExtender"
;!include "define.nsh"
!define APPNAMEANDVERSION "${APPNAME}"
!define Version "1.0.0.0"

VIProductVersion ${Version}
VIAddVersionKey ProductName "NETGEAR NetgearExtender"
;VIAddVersionKey Comments "A build of the PortableApps.com Launcher for , allowing it to be run from a removable drive.  For additional details, visit PortableApps.com"
VIAddVersionKey CompanyName "NETGEAR NetgearExtender"
VIAddVersionKey LegalCopyright "Copyright NETGEAR NetgearExtender Inc."
;VIAddVersionKey FileDescription "(PortableApps.com Launcher)"
VIAddVersionKey FileVersion ${SVN_VERSION}
VIAddVersionKey ProductVersion ${Version}
;VIAddVersionKey InternalName "PortableApps.com Launcher"
;VIAddVersionKey LegalTrademarks "PortableApps.com is a Trademark of Rare Ideas, LLC."
;VIAddVersionKey OriginalFilename "dsafda.exe"

XPstyle on
;Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\NETGEAR NetgearExtender"
InstallDirRegKey HKLM "Software\${APPNAME}" "Install_Dir"
;Icon "Graphics\icons\genie_uninstall.ico"
;UninstallIcon "Graphics\icons\genie_uninstall.ico"

BrandingText "${APPNAMEANDVERSION}"
; Use compression
SetCompressor /SOLID LZMA

; Modern interface settings
!include "Sections.nsh"
!include "MUI.nsh"

;Interface Settings
!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "Graphics\HEADERIMAGE_title.bmp"

# MUI defines
!define MUI_ICON "Graphics\logo.ico"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_LICENSEPAGE_RADIOBUTTONS
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_FINISHPAGE_RUN "$INSTDIR\NetgearExtender.exe"

!define MUI_UNICON "Graphics\logo.ico"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

!define MUI_LANGDLL_REGISTRY_ROOT HKLM
!define MUI_LANGDLL_REGISTRY_KEY "Software\${APPNAME}\Installer"
!define MUI_LANGDLL_REGISTRY_VALUENAME InstallerLanguage

; Defines the installer /uninstall logo of Genie
!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "Graphics\HEADERIMAGE_titleend.bmp"
!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "Graphics\HEADERIMAGE_titleend.bmp"

; Set languages (first is default language)
!insertmacro MUI_RESERVEFILE_LANGDLL
ReserveFile "${NSISDIR}\Plugins\AdvSplash.dll"

;--------------------------------
;Configuration

# Licenses

	LicenseLangString license 1033 license\license_enu.rtf ; English
	;LicenseLangString license 1031 license\license_deu.rtf ; German
	;LicenseLangString license 1053 license\license_sve.rtf ; Swedish
	;LicenseLangString license 1036 license\license_fra.rtf ; French
	;LicenseLangString license 1043 license\license_nld.rtf ; Dutch
	;LicenseLangString license 1040 license\license_ita.rtf ; Italian
	;LicenseLangString license 1034 license\license_esp.rtf ; Spanish
	;LicenseLangString license 1030 license\license_dan.rtf ; Danish
	;LicenseLangString license 1035 license\license_fin.rtf ; Finnish
	;LicenseLangString license 1044 license\license_nor.rtf ; Norwegian
	;LicenseLangString license 1032 license\license_ell.rtf ; Greek
	;LicenseLangString license 2070 license\license_ptb.rtf ; Portuguese
	;LicenseLangString license 1049 license\license_rus.rtf ; Russian
	;LicenseLangString license 1045 license\license_plk.rtf ; Polish
	;LicenseLangString license 1038 license\license_hun.rtf ; Hungarian
	;LicenseLangString license 1029 license\license_csy.rtf ; Czech
	;LicenseLangString license 1060 license\license_slv.rtf ; Slovenian
	;LicenseLangString license 1051 license\license_sky.rtf ; Slovak
	;LicenseLangString license 1048 license\license_rom.rtf ; Romanian
	;LicenseLangString license 1026 license\license_bgr.rtf ; Bulgarian
	;LicenseLangString license 1050 license\license_hrv.rtf ; Croatian
	LicenseLangString license 2052 license\license_chs.rtf ; SimpChinese
	;LicenseLangString license 1042 license\license_kor.rtf ; Korean
	;LicenseLangString license 1041 license\license_jpn.rtf ; Japanese
	;LicenseLangString license 1025 license\license_ara.rtf ; Arabic

# Installer pages
	#!insertmacro MUI_PAGE_WELCOME
	;!insertmacro MUI_PAGE_LICENSE $(license)
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_PAGE_FINISH
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
	!define MUI_LANGDLL_ALLLANGUAGES

	!insertmacro MUI_LANGUAGE "English"
	;!insertmacro MUI_LANGUAGE "German"
	;!insertmacro MUI_LANGUAGE "Swedish"
	;!insertmacro MUI_LANGUAGE "French"
	;!insertmacro MUI_LANGUAGE "Dutch"
	;!insertmacro MUI_LANGUAGE "Italian"
	;!insertmacro MUI_LANGUAGE "Spanish"
	;!insertmacro MUI_LANGUAGE "Danish"
	;!insertmacro MUI_LANGUAGE "Finnish"
	;!insertmacro MUI_LANGUAGE "Norwegian"
	;!insertmacro MUI_LANGUAGE "Greek"
	;!insertmacro MUI_LANGUAGE "Portuguese"
	;!insertmacro MUI_LANGUAGE "Russian"
	;!insertmacro MUI_LANGUAGE "Polish"
	;!insertmacro MUI_LANGUAGE "Hungarian"
	;!insertmacro MUI_LANGUAGE "Czech"
	;!insertmacro MUI_LANGUAGE "Slovenian"
	;!insertmacro MUI_LANGUAGE "Slovak"
	;!insertmacro MUI_LANGUAGE "Romanian"
	;!insertmacro MUI_LANGUAGE "Bulgarian"
	;!insertmacro MUI_LANGUAGE "Croatian"
	!insertmacro MUI_LANGUAGE "SimpChinese"
	;!insertmacro MUI_LANGUAGE "Korean"
	;!insertmacro MUI_LANGUAGE "Japanese"
	;!insertmacro MUI_LANGUAGE "Arabic"

;Component-selection page
	;!include "language.nsh"
	
;-------------------------------------------------
;multilanguage selection
;-------------------------------------------------

Function checkAdmin
	;--------------------------------
	;check current user before vista
	;--------------------------------
	;MessageBox MB_OK 'checkadmin'
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentVersion"
;ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CSDVersion"
	StrCmp $1 "4.9" +1 +3
	StrCpy $0 "98"
	Goto Nouac
	StrCmp $1 "5.0" +1 +3
	StrCpy $0 "2000"
	Goto Nouac
	StrCmp $1 "5.1" +1 +3
	StrCpy $0 "XP"
	Goto Nouac
	StrCmp $1 "5.2" +1 +3
	StrCpy $0 "2003"
	Goto Nouac
	StrCmp $1 "6.0" +1 +3
	StrCpy $0 "Vista"
	Goto CheckUserTypeDone
	StrCmp $1 "6.1" +1 +3
	StrCpy $0 "7"
	Goto CheckUserTypeDone
Nouac:
	;MessageBox MB_OK 'no uac'
	UserInfo::GetOriginalAccountType
	Pop $2
	;MessageBox MB_OK "$0 $1 $2" 

	StrCmp $2 "Admin" CheckUserTypeDone
	StrCmp $2 "Power" CheckUserTypeDone

	MessageBox MB_OK "$(check_admin_err)"
	Quit
CheckUserTypeDone:
FunctionEnd

Function un.checkAdmin
		;--------------------------------
	;check current user before vista
	;--------------------------------
	;MessageBox MB_OK 'un.checkadmin'
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentVersion"
;ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CSDVersion"
	StrCmp $1 "4.9" +1 +3
	StrCpy $0 "98"
	Goto Nouac
	StrCmp $1 "5.0" +1 +3
	StrCpy $0 "2000"
	Goto Nouac
	StrCmp $1 "5.1" +1 +3
	StrCpy $0 "XP"
	Goto Nouac
	StrCmp $1 "5.2" +1 +3
	StrCpy $0 "2003"
	Goto Nouac
	StrCmp $1 "6.0" +1 +3
	StrCpy $0 "Vista"
	Goto CheckUserTypeDone
	StrCmp $1 "6.1" +1 +3
	StrCpy $0 "7"
	Goto CheckUserTypeDone
Nouac:
	;MessageBox MB_OK 'no uac'
	UserInfo::GetOriginalAccountType
	Pop $2
	;MessageBox MB_OK "$0 $1 $2" 

	StrCmp $2 "Admin" CheckUserTypeDone
	StrCmp $2 "Power" CheckUserTypeDone

	MessageBox MB_OK "$(check_admin_err)"
	Quit
CheckUserTypeDone:
FunctionEnd

Function .onInit
!ifdef INNER
 
  ; If INNER is defined, then we aren't supposed to do anything except write out
  ; the installer.  This is better than processing a command line option as it means
  ; this entire code path is not present in the final (real) installer.
 
  ;WriteUninstaller "$%TEMP%\uninstall.exe"
  Quit  ; just bail out quickly when running the "inner" installer
!endif
SetShellVarContext all
	;MessageBox MB_OK 'onInit'
	Call checkAdmin


	;--------------------------------
	;check if the program is running
	;--------------------------------

	InitPluginsDir	
	FindProcDLL::FindProc "NetgearExtender.exe"
    StrCmp $R0 1 +3 +9
	FindProcDLL::FindProc "NetgearExtender.exe"
    StrCmp $R0 1 +1 +7
     MessageBox MB_OKCANCEL "$(install_info)" IDOK OK IDCANCEL Cancel
     OK:
	 KillProcDLL::KillProc "NetgearExtender.exe"
	 Sleep 1000
	 Goto Continue
     Cancel:
     Quit
	;--------------------------------------------
	;welcome flash
	;---------------------------------------------
	Continue:
	
	;KillProcDLL::KillProc "genie_tray.exe"
	;KillProcDLL::KillProc "readyShareFileChecker.exe"
	;Sleep 1000

    ;Push $R1
    ;File /oname=$PLUGINSDIR\spltmp.bmp "Graphics\bmps\welcome_logo.bmp"
    ;advsplash::show 1200 1000 1000 -1 $PLUGINSDIR\spltmp
    ;Pop $R1
    ;Pop $R1
;Language selection dialog
	;-----------------------------
/*	Push ""
	Push ${LANG_ENGLISH}
	Push English
	;Push ${LANG_FRENCH}
	;Push French
	Push ${LANG_GERMAN}
	Push German
	Push ${LANG_SPANISH}
	Push Spanish
	Push ${LANG_PORTUGUESE}
	Push Portuguese
	;Push ${LANG_TRADCHINESE}
	;Push "Traditional Chinese"
	Push ${LANG_SIMPCHINESE}
	Push "Simplified Chinese"
	;Push Simplified Chinese
	Push A ; A means auto count languages
	       ; for the auto count to work the first empty push (Push "") must remain
	LangDLL::LangDialog "$(multilan_title)" "$(multilan_info)"

	Pop $LANGUAGE
	StrCmp $LANGUAGE "cancel" 0 +2
		Abort*/

	; 仅当系统语言未支持时才让用户选择
	IntCmp $LANGUAGE 1033 lang_sel_end
	;IntCmp $LANGUAGE 1031 lang_sel_end
	;IntCmp $LANGUAGE 1053 lang_sel_end
	;IntCmp $LANGUAGE 1036 lang_sel_end
	;IntCmp $LANGUAGE 1043 lang_sel_end
	;IntCmp $LANGUAGE 1040 lang_sel_end
	;IntCmp $LANGUAGE 1034 lang_sel_end
	;IntCmp $LANGUAGE 1030 lang_sel_end
	;IntCmp $LANGUAGE 1035 lang_sel_end
	;IntCmp $LANGUAGE 1044 lang_sel_end
	;IntCmp $LANGUAGE 1032 lang_sel_end
	;IntCmp $LANGUAGE 2070 lang_sel_end ; Portuguese
	;IntCmp $LANGUAGE 1049 lang_sel_end
	;IntCmp $LANGUAGE 1045 lang_sel_end
	;IntCmp $LANGUAGE 1038 lang_sel_end
	;IntCmp $LANGUAGE 1029 lang_sel_end
	;IntCmp $LANGUAGE 1060 lang_sel_end
	;IntCmp $LANGUAGE 1051 lang_sel_end
	;IntCmp $LANGUAGE 1048 lang_sel_end
	;IntCmp $LANGUAGE 1026 lang_sel_end
	;IntCmp $LANGUAGE 1050 lang_sel_end
	IntCmp $LANGUAGE 2052 lang_sel_end
	;IntCmp $LANGUAGE 1042 lang_sel_end
	;IntCmp $LANGUAGE 1041 lang_sel_end
	;IntCmp $LANGUAGE 1025 lang_sel_end
	!insertmacro MUI_LANGDLL_DISPLAY

lang_sel_end:
		
FunctionEnd

!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
;main section
Section Main sec_main
!ifndef INNER
  ;Set Section required
  SectionIn RO

  ; Set Section properties
  SetOverwrite on
  SetOutPath "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  ; Set Section Files and Shortcuts
  File /r /x *.a /x ".svn" "build\*.*"
!endif
SectionEnd

SectionGroup Shortcuts sec_shortcuts
Section  StartMenu SEC0001
!ifndef INNER
  SetOutPath "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\${APPNAME}.lnk" "$INSTDIR\NetgearExtender.exe" "" "$INSTDIR\NetgearExtender.exe" 0
!endif
  
SectionEnd

Section  Desktop SEC0002
!ifndef INNER
  CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\NetgearExtender.exe" "" "$INSTDIR\NetgearExtender.exe" 0
!endif
SectionEnd

Section  Quicklaunchbar SEC0003
!ifndef INNER
  CreateShortCut "$QUICKLAUNCH\${APPNAME}.lnk" "$INSTDIR\NetgearExtender.exe" "" "$INSTDIR\NetgearExtender.exe" 0
!endif
SectionEnd

SectionGroupEnd

Section AutoStart sec_autostart

  CreateShortCut "$SMSTARTUP\${APPNAME}.lnk" "$INSTDIR\bin\NETGEARGenie.exe" "-mini" "$INSTDIR\bin\NETGEARGenie.exe" 0
SectionEnd

Section -FinishSection
!ifndef INNER
  ;DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME_NOSPACE}"
  ;DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME_NOSPACE}"
  ;Delete "$QUICKLAUNCH\${APPNAME_NOSPACE}.lnk"
  
  ;DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME}"
  ;DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME}"
  ;Delete "$QUICKLAUNCH\${APPNAME}.lnk"
  
  ;WriteRegStr HKLM "Software\${APPNAME}" "Install_Dir" "$INSTDIR"
  ;WriteRegStr HKLM "Software\${APPNAME}" "Version" "${VERSION}"
  
  ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
  ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"  
     ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""InstallLocation" "$INSTDIR"
    ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$INSTDIR\genie_install.ico"
    ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "NETGEAR Inc."
    ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${VERSION}"	
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""ModifyPath" "$INSTDIR\Uninstall.exe"
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""InstallSource" "$INSTDIR"
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""ProductID" "(产品ID)" ;产品ID
    ;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "RegOwner" "xxxxx" ;已注册的所有者
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""RegCompany" "${CmpName}" ;已注册的公司
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""HelpLink" "http://" ;技术支持信息
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""HelpTelephone" "800-" ;支持电话
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""URLUpdateInfo" "http://" ;产品更新
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""URLInfoAbout" "http://cmp" ;公司网址

    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""VersionMajor" "2"
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""VersionMinor" "3234"
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""NoModify" 1 ;-1有,1无
    ;WriteRegStrHKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${Name}""NoRepair" 1 ;同上
  
 ; WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\bin\NETGEARGenie.exe" "ELEVATECREATEPROCESS"
  ;WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\bin\NETGEARGenie.exe" "ELEVATECREATEPROCESS"
  ;WriteRegDWORD HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted" "$INSTDIR\bin\NETGEARGenie.exe" 8
  ;WriteRegDWORD HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted" "$INSTDIR\bin\NETGEARGenie.exe" 8

  ;WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
  ;WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
  
  
  
  ;WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME_NOSPACE}" "$\"$INSTDIR\bin\NETGEARGenie.exe$\" -mini -redirect"
!endif
SectionEnd
;--------------------------------------------------
;Uninstall section
;--------------------------------------------------
Section "Uninstall"
  ; Remove program/uninstall regsitry keys
  ;DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
  ;DeleteRegKey HKLM "SOFTWARE\${APPNAME}"
  ;DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\bin\NETGEARGenie.exe"
  ;DeleteRegValue HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\bin\NETGEARGenie.exe"
  ;DeleteRegValue HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted" "$INSTDIR\bin\NETGEARGenie.exe"
  ;DeleteRegValue HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted" "$INSTDIR\bin\NETGEARGenie.exe"

  ;DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME_NOSPACE}"
  ;DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APPNAME_NOSPACE}"

  ; Uninstall service
  ;ExecWait "$INSTDIR\bin\NETGEARGenieDaemon.exe uninstall"
  
  ; Remove files and uninstaller
  Delete $INSTDIR\*.*

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${APPNAME}.lnk"

  ; Remove desktop shortcut
  Delete "$DESKTOP\${APPNAME}.lnk"

  ; Remove Quicklaunch shortcut
  Delete "$QUICKLAUNCH\${APPNAME}.lnk"

  ; Remove Autstart
  ;Delete "$SMSTARTUP\${APPNAME}.lnk"

  ; Remove directories used
  ;RMDir "$SMPROGRAMS\${APPNAME}"
  RMDir /r "$INSTDIR"

SectionEnd

Function  un.onInit
	SetShellVarContext all
	Call un.checkAdmin
	!insertmacro MUI_UNGETLANGUAGE

    FindProcDLL::FindProc "NetgearExtender.exe"
    Pop $R0
    IntCmp $R0 1 0 no_run
    MessageBox MB_OKCANCEL "$(uninstall_info)" IDOK OK IDCANCEL Cancel
	OK:
	KillProcDLL::KillProc "NetgearExtender.exe"
	Sleep 1000
	Goto no_run
	Cancel:
    Quit
    no_run:

	;KillProcDLL::KillProc "genie_tray.exe"
	;KillProcDLL::KillProc "readyShareFileChecker.exe"
	
	
	Sleep 1000
	
FunctionEnd
/*
# Installer Language Strings
# TODO Update the Language Strings with the appropriate translations.

LangString ^UninstallLink ${LANG_ENGLISH} "Uninstall"
LangString ^UninstallLink ${LANG_GERMAN} "Unistall"
LangString ^UninstallLink ${LANG_PORTUGUESE} "Unistall"
LangString ^UninstallLink ${LANG_FRENCH} "Uninstall"
LangString ^UninstallLink ${LANG_SPANISH} "Uninstall"
LangString ^UninstallLink ${LANG_SIMPCHINESE} "卸载"
*/
