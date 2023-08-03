#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "admob_private.h"
#include "com_defold_admob_AdmobJNI.h"
#include "admob_callback_private.h"

JNIEXPORT void JNICALL Java_com_defold_admob_AdmobJNI_admobAddToQueue(JNIEnv * env, jclass cls, jint jmsg, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, 0);
    dmAdmob::AddToQueueCallback((dmAdmob::MessageId)jmsg, json);
    env->ReleaseStringUTFChars(jjson, json);
}

namespace dmAdmob {

struct Admob
{
    jobject        m_AdmobJNI;

    jmethodID      m_Initialize;
    jmethodID      m_LoadInterstitial;
    jmethodID      m_ShowInterstitial;
    jmethodID      m_LoadRewarded;
    jmethodID      m_ShowRewarded;
    jmethodID      m_LoadBanner;
    jmethodID      m_DestroyBanner;
    jmethodID      m_ShowBanner;
    jmethodID      m_HideBanner;
    jmethodID      m_IsRewardedLoaded;
    jmethodID      m_IsInterstitialLoaded;
    jmethodID      m_IsBannerLoaded;
    jmethodID      m_SetPrivacySettings;
    jmethodID      m_RequestIDFA;
    jmethodID      m_ShowAdInspector;
    jmethodID      m_UpdateBannerLayout;
    jmethodID      m_RequestUMP;

};

static Admob       g_admob;

static void CallVoidMethod(jobject instance, jmethodID method)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method);
}

static bool CallBoolMethod(jobject instance, jmethodID method)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jboolean return_value = (jboolean)env->CallBooleanMethod(instance, method);
    return JNI_TRUE == return_value;
}

static void CallVoidMethodChar(jobject instance, jmethodID method, const char* cstr)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = env->NewStringUTF(cstr);
    env->CallVoidMethod(instance, method, jstr);
    env->DeleteLocalRef(jstr);
}

static void CallVoidMethodCharInt(jobject instance, jmethodID method, const char* cstr, int cint)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = env->NewStringUTF(cstr);
    env->CallVoidMethod(instance, method, jstr, cint);
    env->DeleteLocalRef(jstr);
}

static void CallVoidMethodInt(jobject instance, jmethodID method, int cint)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method, cint);
}

static void CallVoidMethodBool(jobject instance, jmethodID method, bool cbool)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method, cbool);
}

static void InitJNIMethods(JNIEnv* env, jclass cls)
{
    g_admob.m_Initialize = env->GetMethodID(cls, "initialize", "()V");
    g_admob.m_LoadInterstitial = env->GetMethodID(cls, "loadInterstitial", "(Ljava/lang/String;)V");
    g_admob.m_ShowInterstitial = env->GetMethodID(cls, "showInterstitial", "()V");
    g_admob.m_LoadRewarded = env->GetMethodID(cls, "loadRewarded", "(Ljava/lang/String;)V");
    g_admob.m_ShowRewarded = env->GetMethodID(cls, "showRewarded", "()V");
    g_admob.m_LoadBanner = env->GetMethodID(cls, "loadBanner", "(Ljava/lang/String;I)V");
    g_admob.m_DestroyBanner = env->GetMethodID(cls, "destroyBanner", "()V");
    g_admob.m_ShowBanner = env->GetMethodID(cls, "showBanner", "(I)V");
    g_admob.m_HideBanner = env->GetMethodID(cls, "hideBanner", "()V");
    g_admob.m_SetPrivacySettings = env->GetMethodID(cls, "setPrivacySettings", "(Z)V");
    g_admob.m_RequestIDFA = env->GetMethodID(cls, "requestIDFA", "()V");
    g_admob.m_ShowAdInspector = env->GetMethodID(cls, "showAdInspector", "()V");
    g_admob.m_UpdateBannerLayout= env->GetMethodID(cls, "updateBannerLayout", "()V");

    g_admob.m_IsRewardedLoaded = env->GetMethodID(cls, "isRewardedLoaded", "()Z");
    g_admob.m_IsInterstitialLoaded = env->GetMethodID(cls, "isInterstitialLoaded", "()Z");
    g_admob.m_IsBannerLoaded = env->GetMethodID(cls, "isBannerLoaded", "()Z");

    g_admob.m_RequestUMP = env->GetMethodID(cls, "requestUMP", "()V");
}

void Initialize_Ext()
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();
    jclass cls = dmAndroid::LoadClass(env, "com.defold.admob.AdmobJNI");

    InitJNIMethods(env, cls);

    jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");

    g_admob.m_AdmobJNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
}

void Initialize()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_Initialize);
}

void LoadInterstitial(const char* unitId)
{
    CallVoidMethodChar(g_admob.m_AdmobJNI, g_admob.m_LoadInterstitial, unitId);
}

void ShowInterstitial()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_ShowInterstitial);
}

bool IsInterstitialLoaded()
{
    return CallBoolMethod(g_admob.m_AdmobJNI, g_admob.m_IsInterstitialLoaded);
}

void LoadRewarded(const char* unitId)
{
    CallVoidMethodChar(g_admob.m_AdmobJNI, g_admob.m_LoadRewarded, unitId);
}

void ShowRewarded()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_ShowRewarded);
}

bool IsRewardedLoaded()
{
    return CallBoolMethod(g_admob.m_AdmobJNI, g_admob.m_IsRewardedLoaded);
}

void LoadBanner(const char* unitId, BannerSize bannerSize)
{
    CallVoidMethodCharInt(g_admob.m_AdmobJNI, g_admob.m_LoadBanner, unitId, (int)bannerSize);
}

void DestroyBanner()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_DestroyBanner);
}

void ShowBanner(BannerPosition bannerPos)
{
    CallVoidMethodInt(g_admob.m_AdmobJNI, g_admob.m_ShowBanner, (int)bannerPos);
}

void HideBanner()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_HideBanner);
}

bool IsBannerLoaded()
{
    return CallBoolMethod(g_admob.m_AdmobJNI, g_admob.m_IsBannerLoaded);
}

void SetPrivacySettings(bool enable_rdp)
{
    CallVoidMethodBool(g_admob.m_AdmobJNI, g_admob.m_SetPrivacySettings, enable_rdp);
}

void RequestIDFA()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_RequestIDFA);
}

void ShowAdInspector()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_ShowAdInspector);
}

void ActivateApp()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_UpdateBannerLayout);
}

void RequestUMP()
{
    CallVoidMethod(g_admob.m_AdmobJNI, g_admob.m_RequestUMP);
}

}//namespace dmAdmob

#endif
