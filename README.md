RTL Manager for Notepad+++
--------------------------
This plugin is meant to help you manage the text direction in Notepad++, the plugin will remember for each file/tab whether it was opened in rtl or ltr mode and will change the text direction once you return to the file/tab.

Build and Debug
---------------
This is a VS project, the serialization of the settings uses [boost](http://www.boost.org/) library.
As said [here](http://docs.notepad-plus-plus.org/index.php/Plugin_Development#Debugging), if you want to debug the plugin the best way to do so is to download [NPP source code](https://github.com/notepad-plus-plus/notepad-plus-plus), open it as a VS solution, add this plugin project to the solution and debug them together.
