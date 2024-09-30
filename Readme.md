 ![language](https://img.shields.io/badge/language-Cpp-green.svg) ![build](https://img.shields.io/badge/build-Windows-blue.svg) ![build](https://img.shields.io/badge/build-linux-FF1010.svg)

# Imgui-Project


### Dependency for Imgui
imgui工程依赖于以下几个库：

* imgui
* opencv  
* pybind11



[imgui 配置](https://blog.csdn.net/qq_51798935/article/details/129904554#:~:text=%E8%BF%99%E7%AF%87%E6%96%87%E7%AB%A0%E4%BB%8B%E7%BB%8D%E4%BA%86%E5%A6%82)
[opencv 配置](https://blog.csdn.net/weixin_44796670/article/details/115900538#:~:text=OpenCV%EF%BC%88O)

[pybind11](https://blog.csdn.net/coco_1998_2/article/details/139514024)


### Compile in Linux
依次执行以下命令：

```bash{.line-numbers}
mkdir build
cd build
cmake..
make -j4
```
依次执行上述命令之后得到 imgui 可执行文件。 

### Run imgui in Linux
```
./imgui
```

　