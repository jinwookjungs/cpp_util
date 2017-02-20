/**
 * @file    Logger.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-02-20 01:07:43
 *
 * Created on Sat Feb 18 20:07:06 2017.
 */

#ifndef LOGGER_H 
#define LOGGER_H 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

//-----------------------------------------------------------------------------
// Logging macros
//-----------------------------------------------------------------------------
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG  my_log::Logger<my_log::Verbosity::message>::get()
#define LOGE my_log::Logger<my_log::Verbosity::error>::get()
#define LOGW my_log::Logger<my_log::Verbosity::warning>::get()
#define LOGI my_log::Logger<my_log::Verbosity::info>::get()
#define LOGD my_log::Logger<my_log::Verbosity::debug>::get().set_file_string(my_log::FileString(__FILENAME__, __LINE__))


namespace my_log {

    /**
     * Log verbosity.
     */
    enum class Verbosity { message, error, warning, info, debug };


    /**
     * For passing the file name and the line number at which the logger is called.
     */
    struct FileString
    {
        std::string file_name_;
        int line_num_;

        FileString() : file_name_(), line_num_() {}
        FileString(std::string file_name, int line_num) : file_name_(file_name), line_num_(line_num) {}

        friend std::ostream& operator<< (std::ostream& os, const FileString fs);
    };

    inline std::ostream& operator<< (std::ostream& os, const FileString fs)
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
            OutStream() : std::ostream(nullptr) { 
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
            Verbosity max_verbosity_;

            LoggerCtrl() : os_(), header_(""), max_verbosity_(Verbosity::debug) {
                os_.add_stream(std::cout);
            }
            LoggerCtrl(const LoggerCtrl& lc) = delete;
            LoggerCtrl& operator=(const LoggerCtrl& lc) = delete;

        public:
            static LoggerCtrl& get()
            {
                static LoggerCtrl lc;
                return lc;
            }

            static OutStream& get_os()            { return LoggerCtrl::get().os_; }
            static std::string& get_header()      { return LoggerCtrl::get().header_; }
            static Verbosity& get_max_verbosity() { return LoggerCtrl::get().max_verbosity_; }

            static void set_header(std::string header) 
            { 
                LoggerCtrl::get().header_ = header; 
            }

            static void set_max_verbosity(const Verbosity& v) 
            {
                LoggerCtrl::get().max_verbosity_ = v; 
            }

            static void add_stream(std::ostream& os)
            {
                LoggerCtrl::get().os_.add_stream(os);
            }
    };


    /**
     * A simple logging class
     */
    template <Verbosity V>
    class Logger
    {
        private:
            OutStream& os_;
            std::string& header_;
            Verbosity verbosity_;
            bool starts_new_line_;

            FileString fs_;

            // std::endl is a template function, and this is the signature of 
            // that function (For std::ostream).
            using endl_type = std::ostream&(std::ostream&); 

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
            static Logger<V>& get()
            {
                static Logger<V> l;
                return l;
            }

            /**
             * Pass the file name and the line number at which this logger is called.
             */
            Logger<V>& set_file_string (FileString fs) 
            {
                fs_ = fs;
                return *this;
            }

            /**
             * @return Verbosity string.
             */
            inline const std::string get_verbosity_string ()
            {
                switch (verbosity_) {
                    case Verbosity::message:
                        return "";
                    case Verbosity::error:
                        return "(E) ";
                    case Verbosity::warning:
                        return "(W) ";
                    case Verbosity::info:
                        return "(I) ";
                    case Verbosity::debug:
                        return "(D) ";
                    default:
                        return "";
                }
            }

            /**
             * Deal with std::endl.
             */
            Logger<V>& operator<<(endl_type endl)
            {
                if (V > LoggerCtrl::get_max_verbosity()) {
                    return *this;
                }

                starts_new_line_ = true;
                os_ << endl;

                return *this;
            }

            // For every other
            template<typename T>
            Logger<V>& operator<< (const T& data)
            {
                if (V > LoggerCtrl::get_max_verbosity()) {
                    return *this;
                }

                if( starts_new_line_ ) {
                    os_ << header_ << get_verbosity_string();

                    if (verbosity_ == Verbosity::debug) {
                        os_ << fs_ << " ";
                    }
                }

                os_ << data;

                starts_new_line_ = false;

                return *this;
            }
    };
}

#endif 
