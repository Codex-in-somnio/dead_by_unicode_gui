# Dead By Unicode GUI

这是一个用于黎明杀机中文聊天的辅助工具。使用此工具可以在游戏中用热键调出支持中文的输入框然后输入中文，原理是通过发送按键事件模拟键盘按下Alt{+Unicode}来输入每个字符。

原Python项目：[Dead By Unicode](https://github.com/k9yyy/dead_by_unicode)

## 使用方法

用法很简单，在游戏运行时确保`DeadByUnicodeGUI.exe`在后台运行即可。在游戏中要输入中文时，先把光标定位到聊天框，然后用设置好的热键（默认是Alt+Shift+D）调出输入框即可输入中文。

如果没有启用十六进制小键盘输入，程序会在启动时询问是否要启用。如果选择启用，程序会将注册表键值`HKEY_CURRENT_USER\Control Panel\Input Method\EnableHexNumpad`的数据设为`1`，然后重新登录Windows后即可生效。不启用无法实现输入。

## 下载发布版

从[Releases](https://github.com/k9yyy/dead_by_unicode_gui/releases)中下载最新版的`DeadByUnicodeGUI_x.x.x.zip`压缩包，其中包含构建好的exe和依赖的dll，解压即可直接使用。

## 从源码构建

项目开发使用的是Qt Open Source 5.4.2框架和MinGW 4.9.1（32位）工具链。

可以从命令行构建：

```
qmake -o Makefile DeadByUnicodeGUI.pro
mingw32-make
```

也可以用QtCreator打开`DeadByUnicodeGUI.pro`构建。

## 贡献翻译

从v0.1.6开始，可以从外部载入语言文件。

将项目中的`DeadByUnicodeGUI/resources/l10n.json`放到`DeadByUnicodeGUI.exe`所在的目录，即可自定义。修改完确认可以使用之后，再通过PR推回去即可贡献翻译。

`l10n.json`的格式：

```
{
    "<两个字母的ISO语言代码>": {
    	"language_name": "<语言的显示名称>",
        "ui_font": "<使用的字体>",
        "messages": {
            "<项目>": "<翻译>",
            ...
        }
    },
    ...
}
```

一些事项：

* 没有翻译的项目会用英文显示；
* 没有指定字体的话会使用系统默认；

* `l10n.json`在程序启动时载入。