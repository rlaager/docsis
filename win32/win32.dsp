# Microsoft Developer Studio Project File - Name="win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=win32 - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak" CFG="win32 - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "win32 - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "win32 - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "win32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "\usr\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib libsnmp.lib /nologo /subsystem:console /machine:I386 /out:"docsis.exe" /libpath:"\usr\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "win32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "\usr\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib libsnmp.lib /nologo /subsystem:console /debug /machine:I386 /out:"docsis_d.exe" /pdbtype:sept /libpath:"\usr\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "win32 - Win32 Release"
# Name "win32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\docsis_decode.c
# End Source File
# Begin Source File

SOURCE=..\docsis_encode.c
# End Source File
# Begin Source File

SOURCE=..\docsis_main.c
# End Source File
# Begin Source File

SOURCE=..\docsis_snmp.c
# End Source File
# Begin Source File

SOURCE=..\docsis_yy.c
# End Source File
# Begin Source File

SOURCE=..\ethermac.c
# End Source File
# Begin Source File

SOURCE=..\hmac_md5.c
# End Source File
# Begin Source File

SOURCE=..\inet_aton.c
# End Source File
# Begin Source File

SOURCE=..\libyywrap.c
# End Source File
# Begin Source File

SOURCE=..\md5.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\docsis.h
# End Source File
# Begin Source File

SOURCE=..\docsis_common.h
# End Source File
# Begin Source File

SOURCE=..\docsis_decode.h
# End Source File
# Begin Source File

SOURCE=..\docsis_encode.h
# End Source File
# Begin Source File

SOURCE=..\docsis_globals.h
# End Source File
# Begin Source File

SOURCE=..\docsis_symtable.h
# End Source File
# Begin Source File

SOURCE=..\ethermac.h
# End Source File
# Begin Source File

SOURCE=..\inet_aton.h
# End Source File
# Begin Source File

SOURCE=..\md5.h
# End Source File
# Begin Source File

SOURCE=..\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\docsis_lex.l

!IF  "$(CFG)" == "win32 - Win32 Release"

# Begin Custom Build
InputPath=..\docsis_lex.l

"../docsis_yy.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o../docsis_yy.c ../docsis_lex.l

# End Custom Build

!ELSEIF  "$(CFG)" == "win32 - Win32 Debug"

# Begin Custom Build
InputPath=..\docsis_lex.l

"../docsis_yy.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o../docsis_yy.c ../docsis_lex.l

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\docsis_yy.y

!IF  "$(CFG)" == "win32 - Win32 Release"

# Begin Custom Build
InputPath=..\docsis_yy.y

"../docsis_yy.tab.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison -t -v ../docsis_yy.y

# End Custom Build

!ELSEIF  "$(CFG)" == "win32 - Win32 Debug"

# Begin Custom Build
InputPath=..\docsis_yy.y

"../docsis_yy.tab.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison -t -v ../docsis_yy.y

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
