# SVGEX

这是只有一个头文件的项目。对SVG继续了扩展。

## 用途

* 定义户型数据
* 将[户型识别](https://bimpp.cn/docs/planreader)的`svg`结果转换为户型数据
* 算法：
  * 从离散的墙数据获取区域边界

## [使用方法](docs/usage.md)

参见：[src/demo.cpp](src/demo.cpp)

## 关于第三方

本项目使用到了[svgpp]、[rapidxml_ns]和[rapidjson]，另外[svgpp]还依赖[boost]库.

[boost]: https://www.boost.org/
[rapidxml_ns]: https://gitee.com/bimpp/rapidxml_ns
[svgpp]: https://gitee.com/bimpp/svgpp
[rapidjson]: https://gitee.com/bimpp/rapidjson

## 许可

遵循[MIT License](LICENSE)
