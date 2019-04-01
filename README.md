# Dead By Unicode GUI

Dead By Unicode is a simple, light-weight tool that allows you to enter any Unicode characters in the Basic Multilingual Plane into Dead By Daylight's chat box, which covers almost all modern languages and commonly used symbols. 

Currently available user interface languages:

* English
* Simplified Chinese
* Japanese (Partially)

Readme in other languages: [中文版Readme](https://github.com/k9yyy/dead_by_unicode_gui/blob/master/README_zh.md)

Original Python project: [Dead By Unicode](https://github.com/k9yyy/dead_by_unicode)

## How it works

It works by automating hexadecimal numpad input (Holding `Alt` and typing `+xxxx`).

Hexadecimal numpad input requires a string type value `EnableHexNumpad` under the registry key `HKEY_CURRENT_USER\Control Panel\Input Method` to be set as `1`.

## Usage

To use Dead By Unicode, simply execute `DeadByUnicodeGUI.exe` and make sure it is running in the background while in game. To enter text, locate the cursor at the to the chat box, and press the pre-defined hotkey combination (default is `Alt` + `Shift` + `D`) to bring up the input box provided by this tool that supports normal text input.

By default, the Settings window opens once `DeadByUnicodeGUI.exe` starts. The default hotkey combination can be changed there. 

If `EnableHexNumpad` has not been properly set, the program will prompt whether to set the registry key value automatically. After the registry key value is set, re-login to Windows is required for it to take effect.

### About key event delay

There is a "Key delay" option which specifies how many milliseconds of extra delay to add to each key event to be sent. The shorter extra delay, the faster it can enter text. However, enter too fast with too many characters may cause the keyboard event queue to overflow, and some characters to be lost. ([Check this issue](https://github.com/k9yyy/dead_by_unicode/issues/1))

## Binary releases

Get the newest binary package (named `DeadByUnicodeGUI_x.x.x.zip`) from [Releases](https://github.com/k9yyy/dead_by_unicode_gui/releases). The binary packages contain Dead By Unicode's executable and necessary DLL dependencies. 

## Build from source

Use Qt 5 Open Source framework and MinGW toolchain.

Build from command-line: (make sure these executables are in PATH variable)

```
qmake -o Makefile DeadByUnicodeGUI.pro
mingw32-make
```

Or use QtCreator to open `DeadByUnicodeGUI.pro` and build

## Contribute translations

Since version 0.1.6, it allows external localization files to be loaded into the program, which makes it easy to change the localization strings without having to rebuild.

Copy `DeadByUnicodeGUI/resources/l10n.json` from the project and place it into the directory where `DeadByUnicodeGUI.exe` is located, and it will be used as the localization file instead of the internal one. 

After editing `l10n.json`, you can send a pull request to push it back. 

Format of `l10n.json`:

```
{
    "<two-letter ISO language code>": {
    	"language_name": "<display name of the language>",
        "ui_font": "<font to be used>",
        "messages": {
            "<key>": "<translation>",
            ...
        }
    },
    ...
}
```

Some notes:

* Untranslated messages will use their English version as fallback;
* If `ui_font` is not specified, the system-default font will be used;  
* `l10n.json` is loaded when the program starts.