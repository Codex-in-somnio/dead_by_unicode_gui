#include "localization.h"

Localization::Localization()
{
    // English will be the fallback language.

    m["en"]["language_name"] = "English";
    m["en"]["settings"] = "Settings";
    m["en"]["about"] = "About";
    m["en"]["exit"] = "Exit";
    m["en"]["set_hot_key"] = "Hotkey";
    m["en"]["key_in"] = "Key input";
    m["en"]["press_a_key"] = "Press a key";
    m["en"]["key_delay"] = "Key delay";
    m["en"]["milliseconds"] = "ms";
    m["en"]["auto_enter"] = "Automatically send enter";
    m["en"]["show_settings_on_start"] = "Show settings on start";
    m["en"]["language"] = "Language";
    m["en"]["save"] = "Save";
    m["en"]["enter"] = "Input";
    m["en"]["cancel"] = "Cancel";
    m["en"]["error"] = "Error";
    m["en"]["question"] = "Question";
    m["en"]["info"] = "Information";
    m["en"]["register_hotkey_fail"] = "Fained to register hot key. Maybe the specified key combination has already been registered by another program, or another instance of Dead by Unicode is already running.";
    m["en"]["parse_config_json_fail"] = "Failed to parse JSON while reading configuration file.";
    m["en"]["write_config_fail"] = "Failed to write configuration file.";
    m["en"]["open_hkey_fail"] = "Cannot determine if hexadecimal numpad input is enabled: Failed to open registry key “%1”.";
    m["en"]["query_reg_value_fail"] = "Cannot determine if hexadecimal numpad input is enabled: Failed to query key value “%1”.";
    m["en"]["question_enable_hex_numpad"] = "Looks like hexadecimal numpad input is not enabled. Enable it now?";
    m["en"]["set_reg_value_success"] = "Registry key value has been set. It will take effect after re-login.";
    m["en"]["set_reg_value_fail"] = "Failed to set registry key value.";
    m["en"]["config_saved"] = "Settings has been saved.";
    m["en"]["author"] = "Author";
    m["en"]["release_version"] = "Release version";
    m["en"]["build_datetime"] = "Build timestamp";
    m["en"]["github_project_page"] = "GitHub project";
    m["en"]["github_project_url"] = "https://github.com/k9yyy/dead_by_unicode_gui";
    m["en"]["ui_font"] = "Segoe UI";

    // ------------------------------------------

    m["zh"]["language_name"] = "简体中文";
    m["zh"]["settings"] = "设置";
    m["zh"]["about"] = "关于";
    m["zh"]["exit"] = "退出";
    m["zh"]["set_hot_key"] = "设置热键";
    m["zh"]["key_in"] = "输入按键";
    m["zh"]["press_a_key"] = "请按键";
    m["zh"]["key_delay"] = "按键延时";
    m["zh"]["milliseconds"] = "毫秒";
    m["zh"]["auto_enter"] = "自动输入回车";
    m["zh"]["show_settings_on_start"] = "启动时显示设置";
    m["zh"]["language"] = "显示语言";
    m["zh"]["save"] = "保存";
    m["zh"]["enter"] = "输入";
    m["zh"]["cancel"] = "取消";
    m["zh"]["error"] = "错误";
    m["zh"]["question"] = "提示";
    m["zh"]["info"] = "信息";
    m["zh"]["register_hotkey_fail"] = "绑定热键失败，可能指定的按键组合已经被其他程序绑定。";
    m["zh"]["parse_config_json_fail"] = "读取配置文件时解析JSON失败。";
    m["zh"]["write_config_fail"] = "写入配置文件失败。";
    m["zh"]["open_hkey_fail"] = "无法判断是否已启用十六进制小键盘输入：打开注册表键“%1”失败。";
    m["zh"]["query_reg_value_fail"] = "无法判断是否已启用十六进制小键盘输入:请求键值“%1”失败。";
    m["zh"]["question_enable_hex_numpad"] = "检测到未启用十六进制小键盘输入，是否立即设置启用？";
    m["zh"]["set_reg_value_success"] = "已写入注册表项。需要重新登录Windows账号使设置生效。";
    m["zh"]["set_reg_value_fail"] = "写入注册表项失败。";
    m["zh"]["config_saved"] = "已保存并应用设置。";
    m["zh"]["author"] = "作者";
    m["zh"]["release_version"] = "Release 版本";
    m["zh"]["build_datetime"] = "编译时间";
    m["zh"]["github_project_page"] = "GitHub项目页";
    m["zh"]["github_project_url"] = "https://github.com/k9yyy/dead_by_unicode_gui";
	m["zh"]["ui_font"] = "微软雅黑";

    // ---------------------------------------------

    m["ja"]["language_name"] = "日本語";
    m["ja"]["settings"] = "設定";
    m["ja"]["about"] = "关于"; // ?
    m["ja"]["exit"] = "終了";
    m["ja"]["set_hot_key"] = "ホットキー設定";
    m["ja"]["key_in"] = "キー入力";
    m["ja"]["press_a_key"] = "入力して";
    m["ja"]["key_delay"] = "キー遅延";
    m["ja"]["milliseconds"] = "ミリ秒";
    m["ja"]["auto_enter"] = "自動的にEnterを入力";
    m["ja"]["show_settings_on_start"] = "起動すると設定のダイアログ表示";
    m["ja"]["language"] = "表示言語";
    m["ja"]["save"] = "保存";
    m["ja"]["enter"] = "入力";
    m["ja"]["cancel"] = "キャンセル";
    m["ja"]["error"] = "エラー";
    m["ja"]["question"] = "プロンプト";
    m["ja"]["info"] = "インフォ";
    m["ja"]["ui_font"] = "Meiryo";

}


QString Localization::getTrans(QString msg, QString langCode)
{
    if (langCode == nullptr)
        langCode = curLangCode;
    if (m.count(langCode) > 0 && m[langCode].count(msg) > 0)
        return m[langCode][msg];
    else
        if (m["en"].count(msg) > 0)
            return m["en"][msg];
        else
            return msg;
}

void Localization::setCurLangCode(QString langCode)
{
    curLangCode = langCode;
}

map<QString, QString> Localization::getLanguages()
{
    map<QString, QString> ret;
    for (auto &lang : m)
    {
        QString code = lang.first;
        QString name = lang.second["language_name"];
        ret[code] = name;
    }
    return ret;
}
