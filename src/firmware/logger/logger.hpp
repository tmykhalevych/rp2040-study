#pragma once

#include <mutex.hpp>
#include <prohibit_copy_move.hpp>
#include <singleton.hpp>
#include <slice.hpp>

#include <array>
#include <experimental/source_location>
#include <string_view>

namespace logger
{

static constexpr auto MAX_ENTRY_LENGTH = 256;
static constexpr auto ENTRIES_DELIMITER = "\n";

using SourceLoc = std::experimental::source_location;

/// @brief Log message severity
enum class Severity : uint8_t
{
    FATAL = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

/// @brief Firmware logger
class Logger
{
public:
    explicit Logger(Severity sev);

    /// @note C variadic function used on purpose as an alternative to variadic template function,
    ///       just not to have a gazillion different implementations in .text segment
    void log(SourceLoc loc, Severity sev, std::string_view format, ...);

private:
    cmn::Slice<char> append_timestamp(cmn::Slice<char> buff, std::chrono::system_clock::time_point ts);
    void account_entry();

    Severity m_sev;
    fr::Mutex m_mutex;
    std::array<char, MAX_ENTRY_LENGTH> m_entry;
};

/// @brief Creates and starts logger
/// @param sev Minimal severity level to account
void create_and_start(Severity sev = Severity::INFO);

/// @brief Accesses logger
/// @return Pointer to logger instance
/// @note Asserts if logger has not been created
cmn::Singleton<Logger>::Ptr access();

}  // namespace logger

#include <logger_macros.hpp>
