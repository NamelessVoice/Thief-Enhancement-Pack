@echo off
set EP=.\EP
set FILE=darkinst.cfg

if exist Thief.exe (
	set EP=.\EP\Thief1+.\EP
	echo Thief 1 / Gold detected.
	set FILE=install.cfg
) else if exist Thief2.exe (
	set EP=.\EP
	echo Thief 2 detected.
) else (
	echo ERROR: Thief not detected.
	echo Please run from Thief / Thief 2 directory
	goto abort
)

echo.

if not exist EP.crf (
	if not exist EP.zip (
		if not exist EP (
			echo EP files not found.
			echo Make sure to place EP.crf into this folder before running.
			goto abort
		)
	)
)

if not exist %file% (
	echo ERROR: The file %file% does not exist.
	goto abort
)

For /F "tokens=1,* delims=+ " %%A in (%file%) Do (
	if "%file%" == "install.cfg" (
		if "%%B+%%C" == "%EP%" (
			echo Thief EP has already been installed!
			goto abort
		)
	) else (
		if "%%B" == "%EP%" (
			echo Thief EP has already been installed!
			goto abort
		)
	)
)

echo Ready to install Thief EP.
echo Resource path in %file% will be modified.
echo Press CTRL+C to abort, or any other key to continue.
pause > nul

echo Applying changes to %file%...
if exist %file%.tmp del %file%.tmp
For /F "tokens=1,*" %%A in (%file%) Do (
	If "%%A"=="resname_base" (
		Echo %%A %EP%+%%B>> %file%.tmp
	) else (
		Echo %%A %%B>>%file%.tmp
	)
)

set objtex=0
set meshtex=0

if exist Dark.cfg (
	For /F "tokens=1" %%A in (Dark.cfg) Do (
		if "%%A" == "ObjTextures16" (
			set objtex=1
		)
		
		if  "%%A" == "MeshTextures16" (
			set meshtex=1
		)
	)
) else (
	echo ERROR: Unable to make changes to Dark.cfg - file not found.
)

if exist Dark.cfg (
	if "%objtex%" == "0" (
		echo Adding ObjTextures16 to Dark.cfg...
		echo ObjTextures16>>Dark.cfg
	)
	
	if "%meshtex%" == "0" (
		echo Adding MeshTextures16 to Dark.cfg...
		echo MeshTextures16>>Dark.cfg
	)
	
)

if exist %file%.tmp (
	echo Modification complete.
	echo.
	if exist %file%.epbak del %file%.epbak
	move %file% %file%.epbak
	move %file%.tmp %file%
	echo Thief EP has been installed successfully.
) else (
	echo Unknown error occured.
	abort
)

goto endhere

:abort
echo Aborting...

:endhere
pause