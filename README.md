# silo_logging

# Abort
silo_loggingの最小構成だけを抜き出して写経させていただいています。<br>
故にnuma, notifier(?), その他DataDog用の最適化に使用しているものは実装していません。(恐らく研究に使用しないため)

# How to use
```
$ rm -r build // if build folder already exists.

$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./silo.exe
```

# 謝辞
https://github.com/masa16/ccbench/tree/master/silo_logging の大部分(というかほぼ全部)を参考に写経させていただきました。
