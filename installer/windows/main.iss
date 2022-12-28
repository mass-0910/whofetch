; NOTE: Be sure to encode this file in UTF-8 with BOM.

#define MyAppName "whofetch"
#define MyAppVersion "0.1"
#define MyAppPublisher "mass-0910"
#define MyAppURL "https://github.com/mass-0910/whofetch"
#define MyAppExeName "whofetch.exe"
#define ProjectRoot "..\.."

[Setup]
; NOTE: Do not change the AppId value.
AppId={{D983BB8D-87AB-424C-9821-434B5DBB9A31}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf64}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile={#ProjectRoot}\LICENSE
PrivilegesRequiredOverridesAllowed=commandline
OutputDir={#ProjectRoot}\build
OutputBaseFilename=whofetch-setup-win-x64
Compression=lzma
SolidCompression=yes
WizardStyle=modern
SetupLogging=yes
ChangesEnvironment=yes
DisableProgramGroupPage=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Files]
Source: "{#ProjectRoot}\build\bin\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectRoot}\build\bin\Release\jpeg62.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectRoot}\build\bin\Release\libpng16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ProjectRoot}\build\bin\Release\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion

[CustomMessages]
english.AddPathDesc=Add to PATH environment variable
japanese.AddPathDesc=whofetchインストールフォルダを環境変数PATHに追加する
english.InstallingVC2022redist=Installing VC++ redistributable package...
japanese.InstallingVC2022redist=VC++再配布可能パッケージをインストールしています...

[Tasks]
Name: envPath; Description: {cm:AddPathDesc}

[Run]
Filename: "{tmp}\vc_redist.x64.exe"; StatusMsg: "{cm:InstallingVC2022redist}"; \
    Parameters: "/install /quiet /norestart"; Check: VC2022RedistNeedsInstall; Flags: waituntilterminated

[Code]

const EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure EnvAddPath(Path: string);
var
    Paths: string;
begin
    { Retrieve current path (use empty string if entry not exists) }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Paths := '';

    { Skip if string already found in path }
    if Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';') > 0 then exit;

    { App string to the end of the path variable }
    Paths := Paths + ';'+ Path +';'

    { Overwrite (or create if missing) path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] added to PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while adding the [%s] to PATH: [%s]', [Path, Paths]));
end;

procedure EnvRemovePath(Path: string);
var
    Paths: string;
    P: Integer;
begin
    { Skip if registry entry not exists }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths) then
        exit;

    { Skip if string not found in path }
    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then exit;

    { Update path variable }
    Delete(Paths, P - 1, Length(Path) + 1);

    { Overwrite path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] removed from PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while removing the [%s] from PATH: [%s]', [Path, Paths]));
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and WizardIsTaskSelected('envPath')
    then EnvAddPath(ExpandConstant('{app}'));
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}'));
end;

function OnDownloadProgress(const Url, Filename: string; const Progress, ProgressMax: Int64): Boolean;
begin
    if ProgressMax <> 0 then
        Log(Format('  %d of %d bytes done.', [Progress, ProgressMax]))
    else
        Log(Format('  %d bytes done.', [Progress]));
    Result := True;
end;

function InitializeSetup: Boolean;
begin
    try
        DownloadTemporaryFile('https://aka.ms/vs/17/release/vc_redist.x64.exe', 'vc_redist.x64.exe', '', @OnDownloadProgress);
        Result := True;
    except
        Log(GetExceptionMessage);
        Result := False;
    end;
end;

function VC2022RedistNeedsInstall: Boolean;
var
    Version: String;
begin
    if RegQueryStringValue(HKEY_LOCAL_MACHINE,
        'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version) then
    begin
        Log('VC Redist Version check : found ' + Version);
        Result := (CompareStr(Version, 'v14.34.31931.0')<0);
    end
    else
    begin
        // Not even an old version installed
        Result := True;
    end;
end;