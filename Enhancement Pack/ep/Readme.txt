- Thief Enhancement Pack -
Release 1
Tuesday the 1st of January, 2008. 

	=    What is it?    =

This project is an effort to improve the quality of the graphics in Thief 2: The Metal Age, and also in Thief: The Dark Project and Thief Gold.
The ultimate goal is to replace all the old, low-quality objects and textures from Thief 1 and 2 with versions that have higher polygon counts and texture resolutions, while keeping as close to the originals as possible.


	=    Installation instructions   =

The Enhancement Pack is compatible with Thief 2: The Metal Age, Thief: The Dark Project and Thief Gold.

To install the Enhancement Pack, you can either use the quick installation method (using the provided batch file to automatically modify any necessary files), or you can modify them manually.
Using the quick installation process is recommended unless you know what you are doing.

 
	--->   Quick installation   <--
Extract this archve (containing EP.crf and InstallEP.bat) into your Thief install folder, be in Thief 2, Thief Gold or Thief 1.
Run InstallEP.bat.

Note: if you are installing to Thief 1, make sure to 'optimise' your install with DarkLoader to make the game FM-ready before installing the Enhancement Pack.

 
	--->   Manual installation   <--

You can also install the Enhancement Pack manually.
First, copy the EP.crf into your Thief install path. Open DarkInst.cfg (Thief 2) or Install.cfg (Thief 1) in your favourite text editor. (Note that Windows Notepad may break the formatting of these files, so you should use a better editor such as Notepad++). Find the line resname_base and add one of the following to it (depending on Thief version), right after the space:

Thief 1: .\EP\Thief1+.\EP+
Thief 2: .\EP+

That line of the file should now read something like this (Thief 2):

resname_base .\EP+.\Res

...or like this (Thief 1):

resname_base .\EP\Thief1+.\EP+.\Crfs


Finally, if you are installing to Thief 1, open Dark.cfg and make sure that both the lines ObjTextures16 and MeshTextures16 are somewhere in that file, on a line of their own. Also make sure that there is one empty line at the end of the file. 

 
	-->   Uninstallation   <--

To uninstall the Enhancement Pack, simply delete EP.crf. You may also want to remove references to it from your DarkInst.cfg / Install.cfg. If you installed using the quick installation instructions, a backup of your old .cfg file was made. Simply replace the modified DarkInst.cfg with the backup, DarkInst.cfg.epbak.


	=    Credits   =

Nameless Voice	Project leader, primary modeller / texture artist
Vigil					Texture artist / modeller
Eshaktaar			Texture artist / modeller, beta tester 
Digital Nightfall	Texture artist
R Soul				Modeller, beta tester
Yandros				Texture artist / modeller 
Wille					Texture artist, beta tester
Targa				Texture artist / modeller 
Schwaa				Modeller
G'len					Modeller
AssiDragon 		One texture (sword blade)
Redface				One texture (Garrett)
DarkDragon		One texture (gambling table)
TheNightTerror	Lead tester and advisor
Nightwalker		Beta tester 
Nicked				Beta tester
ZylonBane			Beta tester


 

 
	=    Special thanks   =

I would like to thanks the following people, for their help, advice or support throughout this project:

TheNightTerror, Kolya, AssiDragon (aka Ravenlord), Spitter, TF, Ajare, ZylonBane, Digital Nightfall, Redface, Nightwalker, Wille, Nicked, Telliamed, Lemog Textures, CGTextures, Mayang Textures, Google Image Search.