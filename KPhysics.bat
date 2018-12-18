cd %~dp0

xcopy KPhysics\Inc\*.h Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Inc\*.inl Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* Physics\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt
