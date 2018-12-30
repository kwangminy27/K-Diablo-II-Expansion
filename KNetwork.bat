cd %~dp0

xcopy KNetwork\Inc\*.h _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Inc\*.inl _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt
