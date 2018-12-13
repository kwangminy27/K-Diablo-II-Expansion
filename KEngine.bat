cd %~dp0

xcopy KEngine\Inc\*.h Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Inc\*.inl Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KClient\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KServer\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KEditor\Bin\ /d /s /y /exclude:ignore.txt
