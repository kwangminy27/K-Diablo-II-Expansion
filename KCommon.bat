cd %~dp0

xcopy KCommon\Inc\*.h Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Inc\*.inl Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* Common\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KNetwork\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KClient\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KServer\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KEditor\Bin\ /d /s /y /exclude:ignore.txt
