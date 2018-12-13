cd %~dp0

xcopy KPhysics\Inc\*.h Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Inc\*.inl Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* Physics\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KClient\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KServer\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEditor\Bin\ /d /s /y /exclude:ignore.txt
