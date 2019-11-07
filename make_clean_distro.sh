#! /bin/sh

#====================================================================
# A few helper functions
#====================================================================

# An small function 'borrowed' from the oomph-lib installation
# script...
OptionPrompt() 
{ 
 printf "%s " "$1" 
}

# Another small function 'borrowed' from the oomph-lib installation
# script...
OptionRead()
{
 read Opt
 if test "$Opt" = "" ; then
  Opt=$1
 fi
 echo $Opt
}

#====================================================================
# Variables
#====================================================================
build_dir=build
# The name of the library
lib_name=cellular_automata

#====================================================================
# The building script
#====================================================================

echo " "
echo "============================================================= "
echo "        "$lib_name" make clean distro script" 
echo "============================================================= "
echo " "

echo ""
echo "============================================================= "
echo ""
echo "I am going to create a clean distro and package it"
echo ""
echo "============================================================= "
echo ""

#====================================================================
# Go one directory up
#====================================================================
current_folder=$(pwd)

#====================================================================
# Make a temporal directory
#====================================================================
tmp_dir=$(mktemp -d -t tachidok-XXXXXXXXXX)
echo $tmp_dir
echo ""
echo ""

echo ""
echo "============================================================= "
echo ""

#====================================================================
# Making a copy
#====================================================================
echo "============================================================= "
echo "Copying the library into" $tmp_dir" folder ..."
echo "============================================================= "
cp -r $current_folder $tmp_dir
echo ""
echo "[COPY DONE]"
echo ""

#====================================================================
# Deleting .git folder
#====================================================================
echo "============================================================= "
echo "I am going to delete .git folder"
echo "============================================================= "
echo ""
rm -rf $tmp_dir/.git
echo ""
echo "[DELETE GIT FOLDER DONE]"
echo ""

#====================================================================
# Deleting build folder
#====================================================================
echo "============================================================= "
echo "I am going to delete" $build_dir " folder"
echo "============================================================= "
echo ""
rm -rf $tmp_dir/$build_dir
echo ""
echo "[DELETE GIT FOLDER DONE]"
echo ""

#====================================================================
# Deleting dat png in folders
#====================================================================
echo "============================================================= "
echo "I am going to delete [dat,png] files, ignoring those in"
echo "[demos] folders"
echo "============================================================= "
echo ""
$tmp_dir/tools/clean_distro.py --root_folder $tmp_dir --ext dat png --ignore_in_path demos
echo ""
echo "[DELETE DAT AND PNG FILES DONE]"
echo ""

#====================================================================
# Create the package of the new distribution
#====================================================================
echo "============================================================= "
echo "I am going to create a package with the new clean"
echo "distribution and delete temporal folder"
echo "============================================================= "
echo ""
tar cvfz cellular_automata.tar.gz $tmp_dir
rm -rf $tmp_dir
echo ""
echo "[PACKAGE/DELETE TMP FOLDER DONE]"
echo ""


#====================================================================
# Copy back the to original project folder
#====================================================================
echo "============================================================= "
echo "Copy the packaged distribution to the project folder"
echo "============================================================= "
echo ""
cp cellular_automata.tar.gz $current_folder
echo ""
echo "[COPY PACKAGE DONE]"
echo ""
