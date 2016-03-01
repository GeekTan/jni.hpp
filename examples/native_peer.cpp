#include <jni/jni.hpp>

#include <iostream>

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*)
   {
    struct Calculator
       {
        static constexpr auto Name() { return "Calculator"; }

        Calculator() { std::cout << "Native peer initialized" << std::endl; }
        ~Calculator() { std::cout << "Native peer finalized" << std::endl; }

        jni::jlong Add(jni::JNIEnv&, jni::jlong a, jni::jlong b) { return a + b; }
        jni::jlong Subtract(jni::JNIEnv&, jni::jlong a, jni::jlong b) { return a - b; }
       };

    jni::JNIEnv& env { jni::GetEnv(*vm) };

    #define METHOD(MethodPtr, name) jni::NativePeerMethod<decltype(MethodPtr), (MethodPtr)>(name)

    jni::RegisterNativePeer(env, jni::Class<Calculator>::Find(env), "peer",
        std::make_unique<Calculator>,
        "initialize",
        "finalize",
        METHOD(&Calculator::Add, "add"),
        METHOD(&Calculator::Subtract, "subtract"));

    return jni::Unwrap(jni::jni_version_1_2);
   }