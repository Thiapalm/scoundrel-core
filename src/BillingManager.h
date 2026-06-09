#pragma once
#include <string>
#include <map>

enum class BillingState
{
    IDLE,
    BUSY,
    SUCCESS,
    ERROR
};

class BillingManager
{
public:
    // Platform-provided singleton accessor. Implemented by platform (mobile) layer.
    static BillingManager &instance();

    virtual ~BillingManager() = default;

    virtual BillingState get_state() const = 0;
    virtual std::string get_last_error() const = 0;

    virtual void start_purchase(const char *sku) = 0;
    virtual void check_purchases() = 0;
    virtual void silent_check_purchases() = 0;
    virtual void restore_purchases() = 0;
    virtual void query_prices() = 0;

    virtual std::string get_localized_price(const std::string &sku) const = 0;

    // Platform callbacks / mutators used by mobile JNI layer
    virtual void set_localized_price(const std::string &sku, const std::string &price) = 0;
    virtual void set_state(BillingState new_state) = 0;
    virtual void set_error(const std::string &message) = 0;
};
