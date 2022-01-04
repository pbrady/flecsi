#! /usr/bin/env bash

cwd="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [ -e $cwd/.git ] ; then

  # Check for git on the system
  git=`which git`

  if [ "$git" = "" ] ; then
    echo "ERROR! You must have git installed to use this script!!!"
    exit
  fi

  detached=`git status --short --branch`

  if [[ $detached =~ "no branch" ]]; then
    describe=`cat $cwd/.version`
  fi

  tags=`git tag`
  if [[ -n $tags ]]; then
    describe=`git describe`

    if [[ "$describe" != *"-"* ]]; then
      describe="$describe-0-0"
    fi
  else
    describe=`cat $cwd/.version`
  fi
else
  describe=`cat $cwd/.version`
fi

IFS='-'
read -ra DESCRIBE <<< "$describe"

if [ ${#DESCRIBE[*]} != 3 ]; then
  echo "ERROR: invalid value returned by git describe"
  exit 1
fi

sha=${DESCRIBE[2]}

# Devel branch
if [[ ${describe:0:1} == "d" ]]; then
  version=`echo ${DESCRIBE[0]} | sed 's,d,,'`
  IFS='.'
  read -ra VERSION <<< "$version"
  major=${VERSION[0]}
  merge=${VERSION[1]}
  commits=${DESCRIBE[1]}
  if (( $commits > 0 )); then
    echo "devel $major ($commits commits ahead at $sha,"\
      "with $merge downstream merges)"
  elif (( $merge > 1)); then
    echo "devel $major ($merge downstream merges)"
  else
    echo "devel $major"
  fi

# Feature branch
elif [[ ${describe:0:1} == "f" ]]; then
  version=`echo ${DESCRIBE[0]} | sed 's,f,,'`
  IFS='.'
  read -ra VERSION <<< "$version"
  major=${VERSION[0]}
  minor=${VERSION[1]}
  merge=${VERSION[2]}
  commits=${DESCRIBE[1]}
  if (( $commits > 0 )); then
    echo "feature $major.$minor ($commits commits ahead at $sha,"\
      "with $merge downstream merges)"
  elif (( $merge > 1)); then
    echo "feature $major.$minor ($merge downstream merges)"
  else
    echo "feature $major.$minor"
  fi

# Release branch
elif [[ ${describe:0:1} == "v" ]]; then
  release=${DESCRIBE[0]}
  commits=${DESCRIBE[1]}
  IFS='.'
  read -ra RELEASE <<< "$release"
  major=`echo ${RELEASE[0]} | sed 's,v,,'`
  minor=${RELEASE[1]}
  patch=${RELEASE[2]}
  if (( $commits > 0 )); then
    echo "release $major.$minor.$patch" \
      "($commits commits ahead of tag at $sha)"
  else
    echo "release $major.$minor.$patch"
  fi
fi
