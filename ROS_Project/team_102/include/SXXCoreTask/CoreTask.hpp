//
//  CoreTask.hpp
//  libSXXCoreTask 
// 
//  Created by SXX on 06/01/2019
//  Copyright © 2015-2019 SXX. All rights reserved.
//

#ifndef __SXX_CORE_TASK_STRIPPED_HPP__
#define __SXX_CORE_TASK_STRIPPED_HPP__

/*
 *  This is a stripped header file for libSXXCoreTask, which
 *  only contains the essential declaration. Ask C.R. for full 
 *  version. :-)
 */

extern "C" {
    extern char _SCT_queue_attr_concurrent;
#define NSEC_PER_SEC 1000000000ull
#define NSEC_PER_MSEC 1000000ull
#define USEC_PER_SEC 1000000ull
#define NSEC_PER_USEC 1000ull
}

namespace SCT {
    
    using Block = std::function<void(void)>;

    class Time {
    public:
        static const Time NOW;
        static const Time FOREVER;
    };

    class Object {
        void *p;
    public:
        Object(const Object &o) noexcept;
        Object(Object &&o) noexcept;
        ~Object() noexcept;

        void resume() const noexcept;
    };

    class Queue : public Object {
    public:
        constexpr static void *SERIAL = NULL;
        constexpr static void *CONCURRENT = &_SCT_queue_attr_concurrent;
        Queue(const char *label, void *attr) noexcept;
        

        void async(Block &&) const noexcept;
        void async(const Block &) const noexcept;
        void sync(Block &&) const noexcept;
        void sync(const Block &) const noexcept;
        static Queue mainQueue() noexcept;
    };

    class Source : public Object {
    public:
        struct Type {
            void *type;
            static const Type DATA_ADD;
            static const Type DATA_OR;
            static const Type PROC;
            static const Type READ;
            static const Type SIGNAL;
            static const Type TIMER;
            static const Type VNODE;
            static const Type WRITE;
        };

        Source(const SCT::Source::Type type, uintptr_t handler, unsigned long mask, const SCT::Queue &queue) noexcept;
        void setEventHandler(SCT::Block &&handler) const noexcept;

        void setEventHandler(const SCT::Block &handler) const noexcept;
        void setTimer(const SCT::Time &start, uint64_t interval, uint64_t leeway) const noexcept;
        void setCancelHandler(SCT::Block &&handler) const noexcept;

        void setCancelHandler(const SCT::Block &handler) const noexcept;
    };
}

#endif