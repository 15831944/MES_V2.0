Set SuccessSign = createobject("scripting.filesystemobject")                 '���ļ�

'Option Explicit 
Dim WshShell, Command,DateCommand,TimeCommand,oExec
Dim RegEx,matches,match,RealDate,RealTime,DTString

'Const TimeServer = "\\10.5.0.8" 
Const TimeServer = "\\10.5.8.129" 
'Const TimeServer = "\\10.5.0.122" 

Set WshShell = WScript.CreateObject("WScript.Shell")
Command = "CMD.exe /c " + " net.exe Time " + TimeServer   
Set oExec    = WshShell.Exec(Command)
DTString = oExec.StdOut.ReadALL
if(((InStr(DTString, "�ɹ�"))<1)and((InStr(DTString, "successfully"))<1))then
	WshShell.Popup "�������ݿ�����������쳣!����������������.",3,"������Ȩ",48
	WScript.Quit
End if
Set RegEx = New RegExp   
RegEx.Pattern = "((\d{2}|\d)(:)(\d{2}|\d))"  		
Set Matches = RegEx.Execute(DTString)   	
For Each Match in Matches   		
	RealTime = Match.Value 
Next
RegEx.Pattern = "((\d{4}|\d{2}|\d)(-|/)(\d{4}|\d{2}|\d)(-|/)(\d{4}|\d{2}|\d))"	
Set Matches = RegEx.Execute(DTString)   	
For Each Match in Matches   		
	RealDate = CStr(CDate(Match.Value))
Next


Set SuccessSignFile = SuccessSign.opentextfile("tem.dll",2,true)
SuccessSignFile.Writeline "[T_SERVER_SETTING]"
SuccessSignFile.Write "T_SERVER="
SuccessSignFile.Writeline RealDate 
'SuccessSignFile.Writeline RealTime


'DateCommand = "CMD.exe /c " + "Date " + RealDate 
'WshShell.Run DateCommand,0,False
'TimeCommand = "CMD.exe /c " + "Time " + RealTime
'WshShell.Run TimeCommand,0,False
