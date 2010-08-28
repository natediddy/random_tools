Android ZIP Signing Tool
===============

README
---------

Use this tool on unsigned ZIP files.

After you have extracted a signed ZIP, edited contents, and rezipped, then you can use this script to sign it quick and painlessly.

As many ZIPs as you want to provide on the command line can be signed at once, for example, this is how SignZip could be used to sign 3 separate ZIPs at once:
	cd <DIRECTORY/OF/THIS/TOOL>
	./signzip ~/Desktop/<NAME OF FILE>.zip ~/Downloads/<NAME OF FILE>.zip ~/android/roms/<NAME OF FILE>.zip
If all went well, the 3 new signed zips will be located in DIRECTORY/OF/THIS/TOOL.

For help:
	cd <DIRECTORY/OF/THIS/TOOL>
	./signzip -h (or --help)

Please keep the *signzip* and */tools* directory in the SAME folder (which would be DIRECTORY/OF/THIS/TOOL in above examples). 

This uses aosp signing keys (same keys CyanogenMod is signed with) 
