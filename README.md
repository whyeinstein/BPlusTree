1.安装boost库

2.安装/测试：

```shell
cmake -G Ninja -S . -B ./build
cmake --build ./build
#正常运行
./build/src/main
#测试
./build/test/t
```

3.详细报告见docs