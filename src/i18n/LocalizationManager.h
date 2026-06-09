#pragma once
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <functional>
#include <optional>

class LocalizationManager
{
public:
    static LocalizationManager &instance();

    using LoaderFunc = std::function<std::optional<std::string>(const std::string &path)>;

    // Optional: set a custom asset loader (useful for Android/mobile consumers).
    // If not set, LocalizationManager will read files from the filesystem.
    static void set_asset_loader(LoaderFunc loader);
    static LoaderFunc asset_loader;

    // Disable copy/move
    LocalizationManager(const LocalizationManager &) = delete;
    LocalizationManager &operator=(const LocalizationManager &) = delete;

    bool load_locale(const std::string &locale_code, const std::filesystem::path &locales_dir = "locales");
    bool is_locale_loaded(const std::string &locale_code) const;
    void set_locale(const std::string &locale_code);
    std::string get_current_locale() const { return current_locale; }

    std::string get_string(const std::string &key) const;

    // Support for placeholders like {0}, {1}
    std::string get_string(const std::string &key, const std::vector<std::string> &args) const;

private:
    LocalizationManager() = default;

    std::string current_locale = "en";
    std::map<std::string, std::map<std::string, std::string>> translations; // locale -> (key -> value)

    std::string format_string(std::string fmt, const std::vector<std::string> &args) const;
};
