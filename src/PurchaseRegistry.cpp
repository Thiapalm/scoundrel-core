#include "PurchaseRegistry.h"
#include <fstream>
#if defined(BUILD_WITH_SDL) && BUILD_WITH_SDL
#include <SDL.h>
#else
static void SDL_Log(const char *, ...) {}
static char *SDL_GetPrefPath(const char *, const char *) { return nullptr; }
static void SDL_free(void *) {}
#endif
#include <sstream>

PurchaseRegistry &PurchaseRegistry::instance()
{
    static PurchaseRegistry inst;
    return inst;
}

PurchaseRegistry::PurchaseRegistry()
{
    load();
}

bool PurchaseRegistry::is_unlocked(const std::string &sku) const
{
    if (sku == "scoundrel_founder_combo")
    {
        if (unlocked_skus.count("scoundrel_founder_combo"))
        {
            return true;
        }
        if (unlocked_skus.count("scoundrel_healer_class") &&
            unlocked_skus.count("scoundrel_space_theme") &&
            unlocked_skus.count("scoundrel_no_ads"))
        {
            return true;
        }
    }

    if ((sku == "scoundrel_no_ads" || sku == "scoundrel_healer_class" || sku == "scoundrel_space_theme") &&
        unlocked_skus.count("scoundrel_founder_combo"))
    {
        return true;
    }

    return unlocked_skus.find(sku) != unlocked_skus.end();
}

void PurchaseRegistry::sync_purchases(const std::set<std::string> &skus)
{
    unlocked_skus.clear();
    for (const auto &sku : skus)
    {
        unlocked_skus.insert(sku);
        SDL_Log("PurchaseRegistry: sync_purchases received sku=%s", sku.c_str());
    }

    if (unlocked_skus.count("scoundrel_founder_combo"))
    {
        unlocked_skus.insert("scoundrel_healer_class");
        unlocked_skus.insert("scoundrel_space_theme");
        unlocked_skus.insert("scoundrel_no_ads");
    }

    if (unlocked_skus.count("scoundrel_healer_class") &&
        unlocked_skus.count("scoundrel_space_theme") &&
        unlocked_skus.count("scoundrel_no_ads"))
    {
        unlocked_skus.insert("scoundrel_founder_combo");
    }

    save();
}

void PurchaseRegistry::unlock(const std::string &sku)
{
    unlocked_skus.insert(sku);
    SDL_Log("PurchaseRegistry: unlock called for sku=%s", sku.c_str());

    if (sku != "scoundrel_founder_combo")
    {
        if (unlocked_skus.count("scoundrel_healer_class") &&
            unlocked_skus.count("scoundrel_space_theme") &&
            unlocked_skus.count("scoundrel_no_ads"))
        {
            unlocked_skus.insert("scoundrel_founder_combo");
        }
    }

    save();
}

void PurchaseRegistry::clear()
{
    unlocked_skus.clear();
    save();
}

void PurchaseRegistry::clear_memory()
{
    unlocked_skus.clear();
}

void PurchaseRegistry::force_reload()
{
    load();
}

void PurchaseRegistry::load()
{
    char *pref_path = SDL_GetPrefPath("PalmieriStudios", "Scoundrel");
    if (!pref_path)
        return;

    std::string filename = std::string(pref_path) + "purchases.dat";
    SDL_free(pref_path);

    std::ifstream is(filename, std::ios::binary);
    if (!is)
        return;

    std::stringstream ss;
    ss << is.rdbuf();
    std::string data = obfuscate(ss.str());

    std::stringstream data_ss(data);
    std::string sku;
    while (std::getline(data_ss, sku))
    {
        if (!sku.empty())
        {
            unlocked_skus.insert(sku);
            SDL_Log("PurchaseRegistry: load found sku=%s", sku.c_str());
        }
    }
}

void PurchaseRegistry::save()
{
    char *pref_path = SDL_GetPrefPath("PalmieriStudios", "Scoundrel");
    if (!pref_path)
        return;

    std::string filename = std::string(pref_path) + "purchases.dat";
    SDL_free(pref_path);

    std::string data;
    for (const auto &sku : unlocked_skus)
    {
        SDL_Log("PurchaseRegistry: save will persist sku=%s", sku.c_str());
        data += sku + "\n";
    }

    std::string obfuscated = obfuscate(data);
    std::ofstream os(filename, std::ios::binary);
    os << obfuscated;
}

std::string PurchaseRegistry::obfuscate(const std::string &data) const
{
    std::string result = data;
    for (size_t i = 0; i < result.size(); ++i)
    {
        result[i] ^= key[i % key.size()];
    }
    return result;
}
