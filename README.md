# ReverseResearch
First you need to install Imagine++ http://imagine.enpc.fr/~monasse/Imagine++/ to be able to our c++ code.
Then you have to compile the project two times :
  -The first time you keep the main.cpp, compile ant then rename the output executable in "request" and put it in \mysite\insta\static\images_algorithms
  -The second time you have to modify the main.cpp:
      -comment line 211
      -uncomment line 213
      -uncomment line 228
      -comment line 299
      -compile ant then rename the output executable in "load_db" and put it in \mysite\insta\static\images_algorithms
You have to put all the necessary dll and plugins in \mysite\insta\static\images_algorithms
Then you have to install django and runserver
We have already created profiles in the db so you can login as login: user_test  password : passmopsi
