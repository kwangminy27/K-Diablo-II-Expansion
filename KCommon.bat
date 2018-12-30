cd %~dp0

xcopy KCommon\Inc\*.h _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Inc\*.inl _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KNetwork\Bin\ /d /s /y /exclude:ignore.txt
