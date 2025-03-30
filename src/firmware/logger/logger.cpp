#include <assert.hpp>
#include <bsp/chrono.hpp>
#include <logger.hpp>
#include <unreachable.hpp>

#include <cstdarg>
#include <ctime>

namespace
{

constexpr std::string_view to_string(logger::Severity sev)
{
    using logger::Severity;
    switch (sev) {
        case Severity::FATAL: return "F";
        case Severity::ERROR: return "E";
        case Severity::WARN: return "W";
        case Severity::INFO: return "I";
        case Severity::DEBUG: return "D";
    }
    UNREACHABLE();
};

}  // namespace

namespace logger
{

Logger::Logger(Severity sev) : m_sev(sev)
{
    log(SourceLoc::current(), Severity::INFO, "init logger");
}

void Logger::log(SourceLoc loc, Severity sev, std::string_view format, ...)
{
    if (m_sev < sev) return;

    const auto ts = bsp::chrono::system_clock::now();
    const std::string_view file_path = loc.file_name();

    std::lock_guard lock(m_mutex);

    const auto entry_body = append_timestamp(cmn::as_slice(m_entry), ts);
    const uint offset =
        std::snprintf(entry_body.data(), entry_body.size(), " [%s] [%s - %s:%u] ", to_string(sev).data(),
                      pcTaskGetName(nullptr), file_path.substr(file_path.find_last_of("/") + 1).data(), loc.line());

    char* message_begin = entry_body.data() + offset;
    const uint max_message_size = entry_body.size() - offset;

    va_list args;
    va_start(args, format);
    std::vsnprintf(message_begin, max_message_size, format.data(), args);
    va_end(args);

    account_entry();
}

cmn::Slice<char> Logger::append_timestamp(cmn::Slice<char> buff, std::chrono::system_clock::time_point ts)
{
    using namespace std::chrono;

    static constexpr auto date_format = "%Y-%m-%d_%H:%M:%S";
    const uint ms = (duration_cast<milliseconds>(ts.time_since_epoch()) % seconds(1)).count();
    std::time_t time = std::chrono::system_clock::to_time_t(ts);

    std::tie(std::ignore, buff) = buff.cut(std::strftime(buff.data(), buff.size(), date_format, std::gmtime(&time)));
    std::tie(std::ignore, buff) = buff.cut(std::snprintf(buff.data(), buff.size(), ".%u", ms));

    return buff;
}

void Logger::account_entry()
{
    // TODO: just print for now, consider saving it somewhere in the future
    std::printf("%s%s", m_entry.data(), ENTRIES_DELIMITER);
}

void create_and_start(Severity sev)
{
    cmn::Singleton<Logger>::emplace(sev);
}

cmn::Singleton<Logger>::Ptr access()
{
    typename cmn::Singleton<Logger>::Ptr logger = cmn::Singleton<Logger>::instance();
    ASSERT(logger);
    return logger;
}

}  // namespace logger
