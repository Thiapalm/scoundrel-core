#pragma once
#include "BillingManager.h"

class NullBillingManager : public BillingManager {
public:
    BillingState get_state() const override { return BillingState::IDLE; }
    std::string get_last_error() const override { return ""; }
    
    void start_purchase(const char* sku) override {}
    void check_purchases() override {}
    void silent_check_purchases() override {}
    void restore_purchases() override {}
    void query_prices() override {}

    std::string get_localized_price(const std::string& sku) const override { return ""; }
};
