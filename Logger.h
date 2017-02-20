/**
 * @file    Logger.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-02-19 23:21:50
 *
 * Created on Sat Feb 18 20:07:06 2017.
 */

#ifndef LOGGER_H 
#define LOGGER_H 

#include <iostream>
#include <string>


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
     * Maximum verbosity of the Logger.
     */
    class LoggerCtrl 
    {
            std::ostream& os_;
            std::string header_;
            Verbosity max_verbosity_;

            LoggerCtrl() : os_(std::cout), header_(""), max_verbosity_(Verbosity::debug) {}
            LoggerCtrl(const LoggerCtrl& lc) = delete;
            LoggerCtrl& operator=(const LoggerCtrl& lc) = delete;

        public:
            static LoggerCtrl& get()
            {
                static LoggerCtrl lc;
                return lc;
            }

            static std::ostream& get_os()
            {
                return LoggerCtrl::get().os_;
            }

            static std::string& get_header()
            {
                return LoggerCtrl::get().header_;
            }

            static void set_header(std::string header)
            {
                LoggerCtrl::get().header_ = header;
            }

            static Verbosity& get_max_verbosity()
            {
                return LoggerCtrl::get().max_verbosity_;
            }

            static void set_max_verbosity(const Verbosity& v)
            {
                LoggerCtrl::get().max_verbosity_ = v;
            }
    };


    /**
     * A simple logging class
     */
    template <Verbosity V>
    class Logger
    {
        private:
            std::ostream& os_;
            std::string& header_;
            Verbosity verbosity_;
            bool starts_new_line_;

            FileString fs_;

            // std::endl is a template function, and this is the signature of 
            // that function (For std::ostream).
            using endl_type = std::ostream&(std::ostream&); 

            Logger<V> () : os_(LoggerCtrl::get().get_os()),
                           header_(LoggerCtrl::get().get_header()),
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
