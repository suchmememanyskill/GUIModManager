# GUIModManager
A graphical mod manager for atmosphere. Should also work on ReiNX and SXOS but is untested (not like you should be using that anyway, lol)

## Usage
1. Make a mods folder on the root of your sd
2. Make a folder (preferably with the name of the thing you're modding) inside the mods folder
3. Inside this new folder, place another new folder (preferably with the name of the mod)
4. Inside this second new folder, place your mods (anything in this folder, when the mod is enabled, will be placed inside the atmosphere folder. So if you have something with a ContentID, the folder placement should be Contents/(id)/\*) 
5. (Optional) Make a file called DESCRIPTION (no extention) inside the second new folder, open the file with a text editor and write a description for the mod

You would end up with something like `/mods/SystemModules/Sys-Dvr/Contents/00FF0000A53BB665/*`

## Credits
- Uses some source code from [Atmosphere](https://github.com/Atmosphere-NX/Atmosphere/tree/master/troposphere/reboot_to_payload) for the reboot to payload code
- Uses path idea from [SimpleModManager](https://github.com/nadrino/SimpleModManager). If i broke something by using it's path please let me know
