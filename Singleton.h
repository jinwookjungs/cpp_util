/**
 * @file    Singleton.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-02-18 20:26:04
 *
 * Created on Sat Feb 18 20:08:28 2017.
 */

#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
    protected:
        // Hidden
        Singleton ();
        Singleton (const Singleton& s);
        Singleton& operator= (const Singleton& s);
        ~Singleton ();
        
        // C++11
//        Singleton (const Singleton& s) = delete;
//        Singleton& operator= (const Singleton& s) = delete;

    public:
        template <typename T>
        static T& get_instance()
        {
            static T s;
            return s;
        }
};

#endif
