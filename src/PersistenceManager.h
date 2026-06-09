#pragma once
#include <string>

namespace scoundrel
{

    class PersistenceManager
    {
    public:
        virtual ~PersistenceManager() = default;

        virtual void save_int(const std::string &key, int value) = 0;
        virtual int load_int(const std::string &key, int default_value = 0) const = 0;

        virtual void save_string(const std::string &key, const std::string &value) = 0;
        virtual std::string load_string(const std::string &key, const std::string &default_value = "") const = 0;
    };

} // namespace scoundrel
