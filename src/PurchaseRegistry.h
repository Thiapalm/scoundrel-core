#pragma once

#include <set>
#include <string>

class PurchaseRegistry
{
public:
    static PurchaseRegistry &instance();

    bool is_unlocked(const std::string &sku) const;
    void sync_purchases(const std::set<std::string> &skus);
    void unlock(const std::string &sku);
    void clear();
    void clear_memory();
    void force_reload();

private:
    PurchaseRegistry();
    void load();
    void save();
    std::string obfuscate(const std::string &data) const;

    std::set<std::string> unlocked_skus;
    const std::string key = "scoundrel_key_123";
};
