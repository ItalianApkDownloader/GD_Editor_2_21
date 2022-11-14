//
// Created by Marco on 07/02/2022.
//

#ifndef MOD_MENU_REMAKE_HANDLER_H
#define MOD_MENU_REMAKE_HANDLER_H
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <unwind.h>
#include <iomanip>
#include <cxxabi.h>


//https://github.com/KiFilterFiberContext/GD-Editor-Leak/commit/bc9a4e09b1a4bf1b1ea5ed7feea732e4a322bf8c


namespace idk {
    
 struct android_backtrace_state
    {
        void **current;
        void **end;
    };

    static _Unwind_Reason_Code android_unwind_callback( _Unwind_Context* context, void* arg )
    {
        android_backtrace_state* state = (android_backtrace_state *)arg;
        uintptr_t pc = _Unwind_GetIP(context);
        if ( pc ) 
        {
            if (state->current == state->end) 
            {
                return _URC_END_OF_STACK;
            } 
            else 
            {
                *state->current++ = reinterpret_cast<void*>(pc);
            }
        }
        return _URC_NO_REASON;
    }

    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    inline bool file_exists(const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
    }

    //
    // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    //
    template<typename ... Args>
    static std::string string_format( const std::string& format, Args ... args )
    {
        int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        std::unique_ptr<char[]> buf( new char[ size ] ); 
        snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }

    static void dump()
    {
        auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "crash.txt";
        std::ofstream outfile;

		outfile.open( path, std::ios::out );

        const int max = 100;
        void* buffer[max];

        android_backtrace_state state;
        state.current = buffer;
        state.end = buffer + max;

        _Unwind_Backtrace(android_unwind_callback, &state);

        int count = (int)(state.current - buffer);

        for (int idx = 0; idx < count; idx++) 
        {
            const void* addr = buffer[idx];
            const char* symbol = "";

            Dl_info info;
            if (dladdr(addr, &info) && info.dli_sname) 
                symbol = info.dli_sname;

            int status = 0; 

            char *demangled = __cxxabiv1::__cxa_demangle(symbol, 0, 0, &status); 

            auto data = string_format( "[%02d] S: %s", idx, (NULL != demangled && 0 == status) ? demangled : symbol );
           CCLog("DATA: %s", data.c_str() );

            if ( demangled != nullptr )
                outfile << data << std::endl;

            if (NULL != demangled)
                free( demangled );        
        }

        outfile.close( );
    }

}






struct BacktraceStateSTR {
    const ucontext_t*   signal_ucontext;
    size_t              address_count = 0;
    static const size_t address_count_max = 30;
    uintptr_t           addresses[address_count_max] = {};
    int prevAddrCount = -1;
    int countEqual = -1;
    int maxCountEqual = 15;

    BacktraceStateSTR(const ucontext_t* ucontext) : signal_ucontext(ucontext) {}

    bool AddAddress(uintptr_t ip) {
        if(prevAddrCount == address_count){
            countEqual++;
            if(countEqual >= maxCountEqual)
                return false;
        }
        prevAddrCount = address_count;
        LOGD("Adding address! %d",address_count);
        // No more space in the storage. Fail.
        if (address_count >= address_count_max)
            return false;

        // Reset the Thumb bit, if it is set.
        const uintptr_t thumb_bit = 1;
        ip &= ~thumb_bit;

        // Ignore null addresses.
        // They sometimes happen when using _Unwind_Backtrace()
        // with the compiler optimizations,
        // when the Link Register is overwritten by the inner
        // stack frames.
        if (ip == 0)
            return true;

        // Ignore duplicate addresses.
        // They sometimes happen when using _Unwind_Backtrace()
        // with the compiler optimizations,
        // because we both add the second address from the Link Register
        // in ProcessRegisters() and receive the same address
        // in UnwindBacktraceCallback().
        if (address_count > 0 && ip == addresses[address_count - 1])
            return true;

        // Finally add the address to the storage.
        addresses[address_count++] = ip;
        return true;
    }
};

void ProcessRegisters(
        _Unwind_Context* unwind_context, BacktraceStateSTR* state) {
    assert(state);
    assert(unwind_context);

    const ucontext_t* signal_ucontext = state->signal_ucontext;
    assert(signal_ucontext);

    const sigcontext* signal_mcontext = &(signal_ucontext->uc_mcontext);
    assert(signal_mcontext);

    _Unwind_SetGR(unwind_context, REG_R0,  signal_mcontext->arm_r0);
    _Unwind_SetGR(unwind_context, REG_R1,  signal_mcontext->arm_r1);
    _Unwind_SetGR(unwind_context, REG_R2,  signal_mcontext->arm_r2);
    _Unwind_SetGR(unwind_context, REG_R3,  signal_mcontext->arm_r3);
    _Unwind_SetGR(unwind_context, REG_R4,  signal_mcontext->arm_r4);
    _Unwind_SetGR(unwind_context, REG_R5,  signal_mcontext->arm_r5);
    _Unwind_SetGR(unwind_context, REG_R6,  signal_mcontext->arm_r6);
    _Unwind_SetGR(unwind_context, REG_R7,  signal_mcontext->arm_r7);
    _Unwind_SetGR(unwind_context, REG_R8,  signal_mcontext->arm_r8);
    _Unwind_SetGR(unwind_context, REG_R9,  signal_mcontext->arm_r9);
    _Unwind_SetGR(unwind_context, REG_R10, signal_mcontext->arm_r10);
    _Unwind_SetGR(unwind_context, REG_R11, signal_mcontext->arm_fp);
    _Unwind_SetGR(unwind_context, REG_R12, signal_mcontext->arm_ip);
    _Unwind_SetGR(unwind_context, REG_R13, signal_mcontext->arm_sp);
    _Unwind_SetGR(unwind_context, REG_R14, signal_mcontext->arm_lr);
    _Unwind_SetGR(unwind_context, REG_R15, signal_mcontext->arm_pc);

    // Program Counter register aka Instruction Pointer will contain
    // the address of the instruction where the crash happened.
    // UnwindBacktraceCallback() will not supply us with it.
    state->AddAddress(signal_mcontext->arm_pc);

    // UnwindBacktraceCallback() does not always supply us with
    // the return address of the frame where the crash happened.
    // Sometimes Link Register will contain this address
    // (noticed when compiling with Clang without optimization),
    // but LR may also contain address of some previously visitied frame
    // (noticed when compiling with GCC without optimization),
    // or LR may contain null address
    // (noticed when compiling with Clang with optimization).
    // These heuristics are unreliable.
#if __clang__
    state->AddAddress(signal_mcontext->arm_lr);
#endif
}

_Unwind_Reason_Code UnwindBacktraceCallback(
        struct _Unwind_Context* unwind_context, void* state_voidp) {
    assert(unwind_context);
    assert(state_voidp);

    BacktraceStateSTR* state = (BacktraceStateSTR*)state_voidp;
    assert(state);

    // On the first UnwindBacktraceCallback() call,
    // set registers to _Unwind_Context and BacktraceState.
    if (state->address_count == 0) {
        ProcessRegisters(unwind_context, state);
        return _URC_NO_REASON;
    }

    uintptr_t ip = _Unwind_GetIP(unwind_context);
    bool ok = state->AddAddress(ip);
    if (!ok)
        return _URC_END_OF_STACK;

    return _URC_NO_REASON;
}

void CaptureBacktrace(BacktraceStateSTR* state) {
    assert(state);
    _Unwind_Backtrace(UnwindBacktraceCallback, state);
}

string getVersionName();
vector<string> getEnabledOptions();

void SigActionHandler(int sig, siginfo_t* info, void* ucontext) {
    LOGD("App crashed!");
    idk::dump();
    exit(0);
 
}






struct sigaction psa, oldPsa;

void Crash_Handler() {
    LOGD("Crash handler started");
    psa.sa_sigaction = SigActionHandler;
    psa.sa_flags = SA_SIGINFO;
    sigaction(SIGHUP, &psa, &oldPsa);   //1
    sigaction(SIGINT, &psa, &oldPsa);   //2
    sigaction(SIGQUIT, &psa, &oldPsa);   //3
    sigaction(SIGILL, &psa, &oldPsa);   //4
    sigaction(SIGTRAP, &psa, &oldPsa);   //5
    sigaction(SIGABRT, &psa, &oldPsa);   //6
    sigaction(SIGIOT, &psa, &oldPsa);   //6
    sigaction(SIGBUS, &psa, &oldPsa);   //7
    sigaction(SIGFPE, &psa, &oldPsa);   //8
    sigaction(SIGKILL, &psa, &oldPsa);   //9
    sigaction(SIGUSR1, &psa, &oldPsa);   //10
    sigaction(SIGSEGV, &psa, &oldPsa);   //11
    sigaction(SIGUSR2, &psa, &oldPsa);   //12
    sigaction(SIGPIPE, &psa, &oldPsa);   //13
    sigaction(SIGALRM, &psa, &oldPsa);   //14
    sigaction(SIGTERM, &psa, &oldPsa);   //15
    sigaction(SIGSTKFLT, &psa, &oldPsa);   //16
    sigaction(SIGCHLD, &psa, &oldPsa);   //17
    sigaction(SIGCONT, &psa, &oldPsa);   //18
    sigaction(SIGSTOP, &psa, &oldPsa);   //19
    sigaction(SIGTSTP, &psa, &oldPsa);   //20
    sigaction(SIGTTIN, &psa, &oldPsa);   //21
    sigaction(SIGTTOU, &psa, &oldPsa);   //22
    sigaction(SIGURG, &psa, &oldPsa);   //23
    sigaction(SIGXCPU, &psa, &oldPsa);   //24
    sigaction(SIGXFSZ, &psa, &oldPsa);   //25
    sigaction(SIGVTALRM, &psa, &oldPsa);   //26
    sigaction(SIGPROF, &psa, &oldPsa);   //27
    sigaction(SIGWINCH, &psa, &oldPsa);   //28
    sigaction(SIGIO, &psa, &oldPsa);   //29
    sigaction(SIGPWR, &psa, &oldPsa);   //30
    sigaction(SIGSYS, &psa, &oldPsa);   //31
    sigaction(SIGUNUSED, &psa, &oldPsa);   //31
    sigaction(__SIGRTMIN, &psa, &oldPsa);   //32
    sigaction(SIGSWI, &psa, &oldPsa);   //32
    sigaction(SA_NOCLDSTOP, &psa, &oldPsa);   //0x00000001
    sigaction(SA_NOCLDWAIT, &psa, &oldPsa);   //0x00000002
    sigaction(SA_SIGINFO, &psa, &oldPsa);   //0x00000004
    sigaction(SA_THIRTYTWO, &psa, &oldPsa);   //0x02000000
    sigaction(SA_RESTORER, &psa, &oldPsa);   //0x04000000
    sigaction(SA_ONSTACK, &psa, &oldPsa);   //0x08000000
    sigaction(SA_RESTART, &psa, &oldPsa);   //0x10000000
    sigaction(SA_NODEFER, &psa, &oldPsa);   //0x40000000
    sigaction(SA_RESETHAND, &psa, &oldPsa);   //0x80000000
    sigaction(MINSIGSTKSZ, &psa, &oldPsa);   //2048
    sigaction(SIGSTKSZ, &psa, &oldPsa);   //819
}



#endif //MOD_MENU_REMAKE_HANDLER_H
