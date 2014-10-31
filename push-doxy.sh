#!/bin/bash
# assuming that the current branch is master and the shell is in the root directory of CppLambdaTest:

# delete old doc
rm -rf doxygen_tmp/

doxygen # recreate doc

git checkout -f gh-pages
rm -rf doxygen/
mv doxygen_tmp/ doxygen
git add doxygen/
git add -u doxygen/
git commit -m "updated doxygen"



