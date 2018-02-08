/**
 * @file    Logger.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-09-23 01:46:19
 * @brief   A header only logger implementation.
 */

#ifndef LOGGER_H 
#define LOGGER_H 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring>

//-----------------------------------------------------------------------------
// Logging macros
//-----------------------------------------------------------------------------
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG  util::Logger<util::LogVerbosity::message>::get()
#define LOGE util::Logger<util::LogVerbosity::error>::get()
#define LOGW util::Logger<util::LogVerbosity::warning>::get()
#define LOGI util::Logger<util::LogVerbosity::info>::get()
#define LOGD util::Logger<util::LogVerbosity::debug>::get().set_file_string(util::FileString(__FILENAME__, __LINE__))

namespace util 
{

/**
 * Log verbosity.
 */
enum class LogVerbosity { message, error, warning, info, debug };


/**
 * For passing the file name and the line number at which the logger is called.
 */
struct FileString
{
    std::string file_name_;
    int line_num_;

    FileString() : file_name_(), line_num_() {}
    FileString(std::string file_name, int line_num) : file_name_(file_name), 
                                                      line_num_(line_num) {}

    friend std::ostream& operator<< (std::ostream& os, const FileString& fs);
};

inline std::ostream& operator<< (std::ostream& os, const FileString& fs)
{   
    os << fs.file_name_ << ":" << fs.line_num_;
    return os;
}


/**
 * An output stream that redirects the data to multiple streams.
 */
class OutStream : public std::ostream
{
private:
    class StreamBuffer : public std::streambuf
    {
    private:
        std::vector<std::streambuf*> bufs_;
    
    public:
        void add_buffer (std::streambuf* buf) { 
            bufs_.push_back(buf); 
        }
        virtual int overflow(int c)
        {
            std::for_each(bufs_.begin(), 
                          bufs_.end(),
                          std::bind2nd(std::mem_fun(&std::streambuf::sputc), c));
            return c;
        }
    };  

    StreamBuffer buffer_;

public: 
    OutStream() : std::ostream(NULL) { 
        std::ostream::rdbuf(&buffer_); 
    }

    void add_stream(std::ostream& os) 
    {
        os.flush();
        buffer_.add_buffer(os.rdbuf());
    }
};


/**
 * Maximum verbosity of the Logger.
 */
class LoggerCtrl 
{
private:
    OutStream os_;
    std::string header_;
    LogVerbosity max_verbosity_;

    LoggerCtrl() : os_(), header_(""), max_verbosity_(LogVerbosity::info) {
        os_.add_stream(std::cout);
    }
    LoggerCtrl(const LoggerCtrl& lc) = delete;
    LoggerCtrl& operator=(const LoggerCtrl& lc) = delete;

public:
    static LoggerCtrl& get() {
        static LoggerCtrl lc;
        return lc;
    }

    static OutStream& get_os()            { return LoggerCtrl::get().os_; }
    static std::string& get_header()      { return LoggerCtrl::get().header_; }
    static LogVerbosity& get_max_verbosity() { return LoggerCtrl::get().max_verbosity_; }

    static void set_header(std::string header) { 
        LoggerCtrl::get().header_ = header; 
    }
    static void set_max_verbosity(const LogVerbosity& v) {
        LoggerCtrl::get().max_verbosity_ = v; 
    }
    static void reset_max_verbosity() {
        LoggerCtrl::get().max_verbosity_ = LogVerbosity::info;
    }
    static void add_stream(std::ostream& os) {
        LoggerCtrl::get().os_.add_stream(os);
    }
};


/**
 * A simple logging class
 */
template <LogVerbosity V>
class Logger
{
private:
    OutStream& os_;
    std::string& header_;
    LogVerbosity verbosity_;
    bool starts_new_line_;

    FileString fs_;

    // std::endl is a template function, and this is the signature of 
    // that function (For std::ostream).
    typedef std::ostream&(endl_type)(std::ostream&);
    // using endl_type = std::ostream&(std::ostream&); 

    Logger<V> () : os_(LoggerCtrl::get_os()),
                   header_(LoggerCtrl::get_header()),
                   verbosity_(V), 
                   starts_new_line_(true) {}
    Logger (const Logger& l) = delete;
    Logger& operator= (const Logger& l) = delete;


public:
    /**
     * @return The logger instance.
     */
    static Logger<V>& get() {
        static Logger<V> l;
        return l;
    }

    /**
     * Pass the file name and the line number at which this logger is called.
     */
    Logger<V>& set_file_string (FileString fs) {
        fs_ = fs;
        return *this;
    }

    /**
     * @return LogVerbosity string.
     */
    inline const std::string get_verbosity_string () {
        switch (verbosity_) {
            case LogVerbosity::message:
                return "";
            case LogVerbosity::error:
                return "(E) ";
            case LogVerbosity::warning:
                return "(W) ";
            case LogVerbosity::info:
                return "(I) ";
            case LogVerbosity::debug:
                return "(D) ";
            default:
                return "";
        }
    }

    /**
     * Deal with std::endl.
     */
    Logger<V>& operator<<(endl_type endl) {
        if (static_cast<int>(V) > static_cast<int>(LoggerCtrl::get_max_verbosity())) {  
            return *this;
        }

        starts_new_line_ = true;
        os_ << endl;
        return *this;
    }

    // For every other
    template<typename T>
    Logger<V>& operator<< (const T& data) {
        if (static_cast<int>(V) > static_cast<int>(LoggerCtrl::get_max_verbosity())) {  
            return *this;
        }

        if( starts_new_line_ ) {
            os_ << header_ << get_verbosity_string();
            if (verbosity_ == LogVerbosity::debug) {
                os_ << fs_ << " ";
            }
        }
        os_ << data;
        starts_new_line_ = false;

        return *this;
    }
};


struct LoggerVerbosityDebug
{
    LoggerVerbosityDebug () {
        util::LoggerCtrl::set_max_verbosity(util::LogVerbosity::debug);
    }
    ~LoggerVerbosityDebug () {
        util::LoggerCtrl::reset_max_verbosity();
    }
};

}   // End of namespace util

#endif 
