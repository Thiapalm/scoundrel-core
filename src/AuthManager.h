#pragma once

#include <string>
#include <memory>

/**
 * @brief Abstract interface for authentication services.
 */
class AuthManager
{
public:
    virtual ~AuthManager() = default;

    /**
     * @brief Checks if the user is currently logged in.
     * @return true if logged in, false otherwise.
     */
    virtual bool is_logged_in() const = 0;

    /**
     * @brief Checks if an authentication process is currently in progress.
     * @return true if authenticating, false otherwise.
     */
    virtual bool is_authenticating() const { return false; }

    /**
     * @brief Triggers the sign-in flow.
     */
    virtual void sign_in() = 0;

    /**
     * @brief Triggers the sign-out flow.
     */
    virtual void sign_out() = 0;

    /**
     * @brief Gets the display name of the logged-in user.
     * @return User name or empty string if not logged in.
     */
    virtual std::string get_user_name() const = 0;

    /**
     * @brief Gets a unique identifier for the logged-in user.
     * @return User ID or empty string if not logged in.
     */
    virtual std::string get_user_id() const = 0;
};

/**
 * @brief Default "No-op" implementation for platforms without authentication support.
 */
class NullAuthManager : public AuthManager
{
public:
    bool is_logged_in() const override { return false; }
    void sign_in() override { /* No-op */ }
    void sign_out() override { /* No-op */ }
    std::string get_user_name() const override { return ""; }
    std::string get_user_id() const override { return ""; }
};
