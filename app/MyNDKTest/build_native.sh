if [ -z "${NDK+aaa}" ];then
echo "please define NDK"
exit 1
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PRJ_ROOT="$DIR/../../"
APP_ANDROID_ROOT="$DIR"
"$NDK"/ndk-build -j1 -C "$APP_ANDROID_ROOT" $* "NDK_MODULE_PATH=${PRJ_ROOT}"
