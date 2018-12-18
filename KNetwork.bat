cd %~dp0

xcopy KNetwork\Inc\*.h Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Inc\*.inl Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* Network\Bin\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt