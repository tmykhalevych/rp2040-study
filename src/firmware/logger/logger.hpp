#pragma once

#include <FreeRTOS.h>
#include <task.h>

#include <assert.hpp>
#include <bsp/chrono.hpp>
#include <inplace_function.hpp>
#include <mutex.hpp>
#include <prohibit_copy_move.hpp>
#include <singleton.hpp>
#include <slice.hpp>

#include <array>
#include <ctime>
#include <experimental/source_location>
#include <string_view>

namespace logger
{

static constexpr auto MAX_ENTRY_LENGTH = 160;
static constexpr auto ENTRIES_DELIMITER = "\n";

using SourceLoc = std::experimental::source_location;
using get_timestamp_cb_t = cmn::InplaceFunction<std::time_t()>;

enum class Severity : uint8_t
{
    FATAL = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

class Logger
{
public:
    Logger(Severity sev) : m_sev(sev) {}

    template <typename... TArgs>
    void log(SourceLoc loc, Severity sev, std::string_view format, TArgs&&... args);

private:
    cmn::Slice<char> append_timestamp(cmn::Slice<char> buff, std::chrono::system_clock::time_point ts);
    void print_entry();

    Severity m_sev;
    fr::Mutex m_mutex;
    std::array<char, MAX_ENTRY_LENGTH> m_entry;
};

template <typename... TArgs>
void Logger::log(SourceLoc loc, Severity sev, std::string_view format, TArgs&&... args)
{
    if (m_sev < sev) return;
    const auto ts = bsp::chrono::system_clock::now();

    constexpr auto to_string = [](Severity sev) -> std::string_view {
        switch (sev) {
            case Severity::FATAL: return "F";
            case Severity::ERROR: return "E";
            case Severity::WARN: return "W";
            case Severity::INFO: return "I";
            case Severity::DEBUG: return "D";
        }
        ASSERT(false);
        return {};
    };

    std::lock_guard lock(m_mutex);

    const std::string_view file_path = loc.file_name();
    const auto entry_body = append_timestamp(cmn::as_slice(m_entry), ts);

    const auto offset =
        std::snprintf(entry_body.data(), entry_body.size(), " [%s] [%s] [%s:%u] ", to_string(sev).data(),
                      pcTaskGetName(nullptr), file_path.substr(file_path.find_last_of("/") + 1).data(), loc.line());

    std::snprintf(m_entry.data() + offset, m_entry.size() - offset, format.data(), std::forward<TArgs>(args)...);

    print_entry();

    m_entry.fill('\0');
}

inline cmn::Slice<char> Logger::append_timestamp(cmn::Slice<char> buff, std::chrono::system_clock::time_point ts)
{
    using namespace std::chrono;

    static constexpr auto date_format = "%Y-%m-%d_%H:%M:%S";
    const uint ms = (duration_cast<milliseconds>(ts.time_since_epoch()) % seconds(1)).count();
    std::time_t time = std::chrono::system_clock::to_time_t(ts);

    std::tie(std::ignore, buff) = buff.cut(std::strftime(buff.data(), buff.size(), date_format, std::gmtime(&time)));
    std::tie(std::ignore, buff) = buff.cut(std::snprintf(buff.data(), buff.size(), ".%u", ms));

    return buff;
}

inline void Logger::print_entry()
{
    std::printf("%s%s", m_entry.data(), ENTRIES_DELIMITER);
}

inline void create_and_start(Severity sev = Severity::INFO)
{
    cmn::Singleton<Logger>::emplace(sev);
}

inline cmn::Singleton<Logger>::Ptr access()
{
    typename cmn::Singleton<Logger>::Ptr logger = cmn::Singleton<Logger>::instance();
    ASSERT(logger);
    return logger;
}

}  // namespace logger

#include <logger_macros.hpp>
