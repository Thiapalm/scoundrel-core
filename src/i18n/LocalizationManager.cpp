#include "LocalizationManager.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <functional>
#include <optional>

// Consumers (mobile/TUI) can set a custom asset loader via
// `LocalizationManager::set_asset_loader(...)`. That avoids tying ScoundrelCore
// to SDL or platform-specific APIs.

LocalizationManager &LocalizationManager::instance()
{
    static LocalizationManager inst;
    return inst;
}

bool LocalizationManager::load_locale(const std::string &locale_code, const std::filesystem::path &locales_dir)
{
    std::string path_str = (locales_dir / (locale_code + ".json")).generic_string();
    std::string content;

    // If a consumer provided an asset loader, use it (e.g. SDL_RWFromFile on Android).
    if (LocalizationManager::asset_loader)
    {
        auto loaded = LocalizationManager::asset_loader(path_str);
        if (!loaded.has_value())
        {
            // Try fallback in current directory
            loaded = LocalizationManager::asset_loader(locale_code + ".json");
        }
        if (loaded.has_value())
            content = std::move(*loaded);
    }
    else
    {
        std::ifstream file(path_str, std::ios::binary);
        if (!file.is_open())
        {
            // Fallback: try current directory
            file.open(locale_code + ".json", std::ios::binary);
        }

        if (file.is_open())
        {
            std::stringstream ss;
            ss << file.rdbuf();
            content = ss.str();
        }
    }

    if (content.empty())
    {
        // Only log error if we are on Android or if it's really missing
        // For CLI/TUI we might not care as much if it fails as they don't use it yet
        // Consumer can provide logging if needed; core doesn't depend on Android APIs.
        return false;
    }

    // Basic JSON-like parser for flat structures: {"key": "value"}
    std::stringstream ss(content);
    std::string line;
    std::map<std::string, std::string> locale_translations;

    while (std::getline(ss, line))
    {
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos)
        {
            size_t first_quote = line.find('"', 0);
            size_t second_quote = line.find('"', first_quote + 1);
            size_t third_quote = line.find('"', colon_pos + 1);
            size_t fourth_quote = line.find('"', third_quote + 1);

            if (first_quote != std::string::npos && second_quote != std::string::npos &&
                third_quote != std::string::npos && fourth_quote != std::string::npos)
            {

                std::string key = line.substr(first_quote + 1, second_quote - first_quote - 1);
                std::string value = line.substr(third_quote + 1, fourth_quote - third_quote - 1);

                locale_translations[key] = value;
            }
        }
    }

    if (!locale_translations.empty())
    {
        translations[locale_code] = std::move(locale_translations);
        return true;
    }

    return false;
}

// static member definition for the optional asset loader
LocalizationManager::LoaderFunc LocalizationManager::asset_loader = nullptr;

void LocalizationManager::set_asset_loader(LoaderFunc loader)
{
    LocalizationManager::asset_loader = std::move(loader);
}

bool LocalizationManager::is_locale_loaded(const std::string &locale_code) const
{
    return translations.count(locale_code);
}

void LocalizationManager::set_locale(const std::string &locale_code)
{
    if (is_locale_loaded(locale_code))
    {
        current_locale = locale_code;
    }
    else
    {
        // Fallback to en if the desired locale is not loaded
        if (is_locale_loaded("en"))
        {
            current_locale = "en";
        }
        else
        {
            // If even en is not loaded, current_locale remains as is.
            // This is an unrecoverable state and might lead to showing keys.
        }
    }
}

std::string LocalizationManager::get_string(const std::string &key) const
{
    auto it_locale = translations.find(current_locale);
    if (it_locale != translations.end())
    {
        auto it_key = it_locale->second.find(key);
        if (it_key != it_locale->second.end())
        {
            return it_key->second;
        }
    }

    // Fallback to English if not found in current locale
    if (current_locale != "en")
    {
        auto it_en = translations.find("en");
        if (it_en != translations.end())
        {
            auto it_key = it_en->second.find(key);
            if (it_key != it_en->second.end())
            {
                return it_key->second;
            }
        }
    }

    return key; // Return the key itself as last resort
}

std::string LocalizationManager::get_string(const std::string &key, const std::vector<std::string> &args) const
{
    std::string base = get_string(key);
    return format_string(base, args);
}

std::string LocalizationManager::format_string(std::string fmt, const std::vector<std::string> &args) const
{
    for (size_t i = 0; i < args.size(); ++i)
    {
        std::string placeholder = "{" + std::to_string(i) + "}";
        size_t pos = 0;
        while ((pos = fmt.find(placeholder, pos)) != std::string::npos)
        {
            fmt.replace(pos, placeholder.length(), args[i]);
            pos += args[i].length();
        }
    }
    return fmt;
}
