
ECHO OFF

REM Nanodesktop script move files

if "%NDCYGWIN_PATH%"=="" goto error1
if "%NDENV_PATH%"==""	 goto error2

copy %NDCYGWIN_PATH%\BIN\cyg*.dll 	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\make.exe 	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\bash.exe 	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\sh.exe   	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\run.exe  	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\cp.exe   	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\mv.exe   	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\rm.exe   	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN
copy %NDCYGWIN_PATH%\BIN\mkdir.exe   	%NDENV_PATH%\PSP\SDK\PSPDEV\BIN\xmkdir.exe
goto end

:error1

echo ERROR: Set NDCYGWIN_PATH environment variable first
goto end

:error2

echo ERROR: Set NDENV_PATH environment variable first
goto end

:end

