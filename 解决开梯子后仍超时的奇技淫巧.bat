@echo off
:: 原理不清楚，总之两次偶然的尝试下，发现开代理后有删除代理，结果就能使用CLion的github了
:: 注意！本人挂梯子时并未开任何代理，这个只是个奇技淫巧

echo "开启代理..."
git config --global http.proxy http://127.0.0.1:7890

echo "关闭代理..."
git config --global --unset http.proxy

echo "接下来使用梯子应该可以正常使用CLion的github了..."