cd %~dp0

xcopy KPhysics\Inc\*.h _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Inc\*.inl _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt
