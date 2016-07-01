#!/bin/bash

################################################################################
# init

CUR_PATH=$PWD
SCRIPT_HOME=$(dirname $(readlink -f $0))
SDK_PATH="$HOME/tizen-sdk"
SDK_TIZEN_BIN="$SDK_PATH/tools/ide/bin/tizen"
TEMPLATE_PATH=$SCRIPT_HOME"/templates"
TEMPLATE_PROJECT="project-template"
TEMPLATE_PACKAGE="org.example.project-template"
TEMPLATE_KEYWORD="HelloWorld"
TEMP_APPASSIST_DIR="_tmp_app-assist-efl"

BUILD_DEFINE_SDK_2_3_RESERVED="WUSING_SDK_2_3_RESERVED"
BUILD_DEFINE_SDK_2_3="WUSING_SDK_2_3"

################################################################################
# functions

# convert carmel case string to upper case and hypen string
# arg : original string
carmel_to_guard()
{
    org_string=$1
    result=""

    while [ -n $org_string ]
    do
        carmel_re="^([A-Z][a-z|0-9]*)(.*)"
        if [[ $org_string =~ $carmel_re ]]; then
            result=$result"_"${BASH_REMATCH[1]^^}
        fi
        org_string=${BASH_REMATCH[2]}

        if [ -z $org_string ]; then
            break;
        fi
    done

    if [ -z $result ]; then
        result="_"${1^^}"_"
    else
        result=$result"_"
    fi

    echo $result
}

# replace keywords
# arg : filepath, original string, new string
replace_keyword()
{
    sed "s/$2/$3/g" $1 > $1".tmp"
    cp $1".tmp" $1
    rm $1".tmp"
}

# delete keyword
delete_keyword()
{
    sed "s/$2//g" $1 > $1".tmp"
    cp $1".tmp" $1
    rm $1".tmp"
}

# delete line at keyword
delete_line_keyword()
{
    sed "/$2/d" $1 > $1".tmp"
    cp $1".tmp" $1
    rm $1".tmp"
}

# print usage
print_help()
{
    echo " "
    echo "Usage : $0 [options] [project name] [package name] [class prefix]"
    echo " "
    echo " ex) $0 -v 2.4 -p mobile -w ./workspace-tizen hello-world com.test.hello-world HelloWorld"
    echo " ex) $0 -a x86 -c gcc-4.6 hello-world com.test.hello-world HelloWorld"
    echo " ex) $0 -v 2.3.1 -p wearable --no-appassist-lib hello-world com.test.hello-world HelloWorld"
    echo " "
    echo "-v, --api-version [ 2.3 | 2.4 (default) ] : platform version"
    echo " "
    echo "-p, --profile [ mobile (default) | wearable ] : profile"
    echo " "
    echo "-w, --workspace [ workspace path (default is current directory ] : A path of Tizen SDK workspace"
    echo " "
    echo "-a, --arch [ arm (default) | x86 ] (This option will be used only for app-assist lib build)"
    echo " "
    echo "-c, --compiler [ gcc-4.9 (2.4 default) | gcc-4.6 (2.3.x default) | llvm-3.4 | llvm-3.6 ]"
    echo "    : refer to cli build toolchains (This option will be used only for app-assist lib build)"
    echo " "
    echo "--no-appassist-lib : The app-assist lib(libapp-assist-efl.so and app-assist-efl.edj) will be excluded in the created project"
    echo " "
    echo "-h, --help Show this message"
    echo " "
}


################################################################################
# parse arguments

# default values
platform_version="2.4"
profile="mobile"
workspace_path="."
arch="arm"
no_appassist_lib=false

# parsing
while :
do
    case "$1" in
        -v|--api-version) shift; platform_version=$1;;
        -p|--profile) shift; profile=$1;;
        -w|--workspace) shift; workspace_path=$1;;
        -a|--arch) shift; arch=$1;;
        -c|--compiler) shift; compiler=$1;;
        --no-appassist-lib) no_appassist_lib=true;;
        -h|--help) print_help; exit 1;;
        (--) shift; break;;
        (-*) echo "$0: unknown option $1" 1>&2; exit 1;;
        (*) project=$1; shift; package=$1; shift; class_prefix=$1; break;;
    esac
    shift
done

# check arguments
if [ -z $project ] || [ -z $package ] || [ -z $class_prefix ]; then
    print_help
    exit 1;
fi

# check project duplication
if [ -d $project ] || [ -f $project ]; then
    echo "$project file or directory aleady exists"
    exit 1;
fi

# check workspace
if [ ! -d $workspace_path ]; then
    echo "$workspace_path path not found"
    exit 1;
fi

# check profile
if [ $profile != "mobile" ] && [ $profile != "wearable" ]; then
    echo "$profile is invalid profile"
    print_help
    exit 1;
fi

# check api version
api_ver_re="^[0-9|\.]*$"
if [[ ! $platform_version =~ $api_ver_re ]]; then
    echo "$platform_version is invalid api version"
    print_help
    exit 1;
fi

# check compiler
if [ -z $compiler ]; then
    if [[ $platform_version =~ "2.3" ]]; then
        compiler="gcc-4.6"
        compiler_toolchain_name="gcc46"
    else
        compiler="gcc-4.9"
        compiler_toolchain_name="gcc49"
    fi
else
    if [ $compiler == "gcc-4.9" ]; then
        compiler_toolchain_name="gcc49"
    else
        compiler_toolchain_name="gcc46"
    fi
fi

# make variables
dest_profile=$profile"-"$platform_version


################################################################################
# build app-assist-efl

# copy app-assist-efl to workspace
tmp_appassist="$workspace_path/$TEMP_APPASSIST_DIR"
if [ -d $tmp_appassist ]; then
    rm -rf $tmp_appassist
fi

# Copy command is replaced with following rsync command
# cmd="cp -r $SCRIPT_HOME/app-assist-efl $tmp_appassist"

cmd="rsync -a --exclude=*/Debug $SCRIPT_HOME/app-assist-efl/base/ $tmp_appassist"
$cmd

# change build setting
replace_keyword "$tmp_appassist/project_def.prop" "mobile-2.4" $dest_profile

if [[ "$platform_version" =~ "2.3" ]]; then
    replace_keyword "$tmp_appassist/project_def.prop" $BUILD_DEFINE_SDK_2_3_RESERVED $BUILD_DEFINE_SDK_2_3
fi

if [ $no_appassist_lib == false ]; then
    cd $tmp_appassist
    cmd="$SDK_TIZEN_BIN build-native -a $arch -c $compiler -C Release"
    $cmd
    cd $CUR_PATH
fi


################################################################################
# make project
dest_project="$CUR_PATH/$project"

# copy template project
cmd="cp -R $TEMPLATE_PATH/$TEMPLATE_PROJECT $dest_project"
$cmd

# set version

replace_keyword "$dest_project/project_def.prop" "mobile-2.4" $dest_profile
replace_keyword "$dest_project/.tproject" "mobile-2.4" $dest_profile
replace_keyword "$dest_project/tizen-manifest.xml" "api-version=\"2.4\"" "api-version=\""$platform_version"\""
replace_keyword "$dest_project/tizen-manifest.xml" "mobile" $profile

replace_keyword "$dest_project/.cproject" "mobile-2.4" $dest_profile
replace_keyword "$dest_project/.cproject" "gcc49" $compiler_toolchain_name

# set build defines
if [[ "$platform_version" =~ "2.3" ]]; then
    replace_keyword "$dest_project/.cproject" $BUILD_DEFINE_SDK_2_3_RESERVED $BUILD_DEFINE_SDK_2_3
    replace_keyword "$dest_project/tproject_def.prop" $BUILD_DEFINE_SDK_2_3_RESERVED $BUILD_DEFINE_SDK_2_3
else
    delete_line_keyword "$dest_project/.cproject" $BUILD_DEFINE_SDK_2_3_RESERVED
    delete_keyword "$dest_project/project_def.prop" $BUILD_DEFINE_SDK_2_3_RESERVED
fi

# replace all keywords and rename files
cmd="find $dest_project -follow -type f"
ret=`$cmd`

template_guard=$(carmel_to_guard "$TEMPLATE_KEYWORD")
guard=$(carmel_to_guard "$class_prefix")

for file in $ret
do
    replace_keyword $file $TEMPLATE_KEYWORD $class_prefix
    replace_keyword $file $TEMPLATE_PACKAGE $package
    replace_keyword $file $TEMPLATE_PROJECT $project

    ext=${file##*.}
    if [ "$ext" == "h" ]; then
        replace_keyword $file $template_guard $guard
    fi

    # rename
    if [[ $file =~ "$TEMPLATE_KEYWORD" ]]; then
        dest=${file/$TEMPLATE_KEYWORD/$class_prefix}
        mv $file $dest
    elif [[ $file =~ "$TEMPLATE_PROJECT" ]]; then
        dest=${file/$TEMPLATE_PROJECT/$project}
        mv $file $dest
    fi
done

# copy app-asssit files
cmd="mkdir $dest_project/inc/app-assist"
$cmd
cmd="cp $tmp_appassist/inc/* $dest_project/inc/app-assist/."
$cmd

if [ $no_appassist_lib == false ]; then
    cmd="mkdir $dest_project/lib"
    $cmd
    cmd="cp $tmp_appassist/Release/libapp-assist-efl.so $dest_project/lib/."
    $cmd
    cmd="cp $tmp_appassist/Release/res/edje/app-assist-efl.edj $dest_project/res/edje/."
    $cmd
fi

# release resources
cmd="rm -rf $tmp_appassist"
$cmd

echo "$project project creation complete."
