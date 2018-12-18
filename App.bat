cd %~dp0

xcopy KCommon\Inc\*.h Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Inc\*.inl Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* Common\Bin\ /d /s /y /exclude:ignore.txt
xcopy KCommon\Bin\*.* KNetwork\Bin\ /d /s /y /exclude:ignore.txt

xcopy KPhysics\Inc\*.h Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Inc\*.inl Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* Physics\Bin\ /d /s /y /exclude:ignore.txt
xcopy KPhysics\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt

xcopy KNetwork\Inc\*.h Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Inc\*.inl Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* Network\Bin\ /d /s /y /exclude:ignore.txt
xcopy KNetwork\Bin\*.* KEngine\Bin\ /d /s /y /exclude:ignore.txt

xcopy KEngine\Inc\*.h Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Inc\*.inl Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KClient\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KServer\Bin\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* KEditor\Bin\ /d /s /y /exclude:ignore.txt