#include "game.h"
#include <jni.h>

#include <android/log.h>

JNIEnv* jniEnv;

char* jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = 0;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "utf-8");
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0)
    {
    rtn = (char*)malloc(alen + 1);

    memcpy(rtn, ba, alen);
    rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    return rtn;
}

JNIEXPORT void JNICALL Java_com_example_myndktest_GameLibJNIWrapper_on_1surface_1created
    (JNIEnv * env, jclass cls) {
    on_surface_created();
}

JNIEXPORT void JNICALL Java_com_example_myndktest_GameLibJNIWrapper_on_1surface_1changed
    (JNIEnv * env, jclass cls, jint width, jint height) {
    on_surface_changed();
}

JNIEXPORT void JNICALL Java_com_example_myndktest_GameLibJNIWrapper_on_1draw_1frame
    (JNIEnv * env, jclass cls) {
    on_draw_frame();
}

JNIEXPORT void JNICALL Java_com_example_myndktest_GameLibJNIWrapper_key_1points_1with_1corners
    (JNIEnv * env, jclass cls, jobjectArray origin_key_points, jobjectArray target_key_points, jstring origin_path, jstring target_path){



    jint i, j;

    //1.¿ÉÓÃÐÔ¼ì²â
    if(jniEnv == 0){
        jniEnv = env;
    }

    //2.»ñÈ¡ÐÐÊýºÍÁÐÊý
    jsize row = (*jniEnv)->GetArrayLength(jniEnv, origin_key_points);
    //__android_
    if(row < 0)
        exit(1);
    jarray myArray = ((*jniEnv)->GetObjectArrayElement(jniEnv, origin_key_points, 0));
    jarray myArray2 = ((*jniEnv)->GetObjectArrayElement(jniEnv, target_key_points, 0));

    int col = (*jniEnv)->GetArrayLength(env, myArray);

    if(col < 0)
        exit(1);

    //3.»ñÈ¡ÊýÖµ
    jfloat * origin_key_points_c = (jfloat * )malloc(sizeof(float) * row * col);
    jfloat * target_key_points_c = (jfloat * )malloc(sizeof(float) * row * col);
//    jfloat target_key_points_c[row * col];
    char * origin_pic_path;
    char * target_pic_path;


    for(i = 0; i < row; i++){
        myArray = ((*jniEnv)->GetObjectArrayElement(jniEnv, origin_key_points, i));
        myArray2 = ((*jniEnv)->GetObjectArrayElement(jniEnv, target_key_points, i));

        jfloat * coldata = (*jniEnv)->GetFloatArrayElements(jniEnv, (jfloatArray)myArray, 0);
        jfloat * coldata2 = (*jniEnv)->GetFloatArrayElements(jniEnv, (jfloatArray)myArray2, 0);

        for(j = 0; j < col; j++){
            origin_key_points_c[i * col + j] = coldata[j];
            target_key_points_c[i * col + j] = coldata2[j];
        }
    }


//    float * a = new float[x]


    origin_pic_path = jstringTostring(jniEnv, origin_path);
    target_pic_path = jstringTostring(jniEnv, target_path);
    int origin_length = row;
    int target_length = row;

//    __android_log_print(ANDROID_LOG_INFO, "JNITag","string From Java To C : %f||||%f", origin_pic_path[k * row + 0], origin_pic_path[k * row + 1]);

    int k;
    for(k = 0; k < row; k++){
        __android_log_print(ANDROID_LOG_INFO, "JNITag","string From Java To C : %d||||%f||||%f", k, origin_key_points_c[k * col + 0], origin_key_points_c[k * col + 1]);
    }



    setUpdate(origin_key_points_c, target_key_points_c, origin_length, target_length, origin_pic_path, target_pic_path);
//    __android_log_print(ANDROID_LOG_INFO, "JNITag","string From Java To C : %s", "run here");
}
