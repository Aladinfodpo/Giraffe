; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Giraffe"
#define MyAppVersion "0.0.4"
#define MyAppPublisher "FittingBox"
#define MyAppURL "https://www.fittingbox.com/"
#define MyAppExeName "Giraffe.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{6B06E510-52A2-4805-B946-513C9A74CBC4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Remove the following line to run in administrative install mode (install for all users.)
PrivilegesRequired=lowest
Compression=lzma
SolidCompression=yes
WizardStyle=modern
OutputBaseFilename={#MyAppName}Setup{#MyAppVersion}
SetupIconFile=C:\Users\NicolasBEAUQUIN\source\repos\Girafe\data\giraffe.ico
UninstallDisplayIcon=C:\Users\NicolasBEAUQUIN\source\repos\Girafe\data\giraffe.ico
UninstallDisplayName={#MyAppName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\styles\qwindowsvistastyle.dll"; DestDir: "{app}/styles"; Flags: ignoreversion
Source: "C:\Users\NicolasBEAUQUIN\source\repos\build-Giraffe-Desktop_x86_windows_msvc2019_pe_64bit-Release\release\platforms\qwindows.dll"; DestDir: "{app}/platforms"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
