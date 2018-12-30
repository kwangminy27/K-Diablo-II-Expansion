cd %~dp0

xcopy KCommon\Inc\*.h _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Inc\*.inl _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KNetwork\Bin\ /d /s /y /exclude:ignore.txt

xcopy KPhysics\Inc\*.h _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Inc\*.inl _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt

xcopy KNetwork\Inc\*.h _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Inc\*.inl _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt

xcopy KEngine\Inc\*.h _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Inc\*.inl _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KClient\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KServer\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KEditor\Bin\ /d /s /y /exclude:ignore.txt
